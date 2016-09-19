#include <iostream>
#include <regex>

#include <boost/endian/conversion.hpp>

#include "instruction.h"
#include "parser.h"

void Instruction::setOPCode(OPCode code) {
    if(type == InsType::DAT)
        throw instruction_error("Cannot set opcode for DAT type");

    data &= 0x03FF; // 0000 0011 1111 1111
    data |= ((uint16_t)code) << 10;
}

OPCode Instruction::getOPCode() const {
    if(type == InsType::DAT)
        throw instruction_error("Cannot retrieve opcode from DAT type");

    //16bits, the first 6 are what we want, so shift right 10, and then convert to
    //an unsigned value and cast to uint8_t
    return (OPCode)(data >> 10);
}


void Instruction::setArg1Mode(AccessMode mode) {
    if (type == InsType::DAT)
        throw instruction_error("Canot set destination access mode for DAT type");
    if((uint8_t)mode > 1)
        throw instruction_error("Invalid destination access mode");

    data &= 0xFDFF; // 1111 1101 1111 1111
    data |= ((uint16_t)mode) << 9;
}

void Instruction::setArg2Mode(AccessMode mode) {
    if (type == InsType::DAT)
        throw instruction_error("Canot set source access mode for DAT type");
    if((uint8_t)mode > 1)
        throw instruction_error("Invalid source access mode");

    data &= 0xFEFF; // 1111 1110 1111 1111
    data |= ((uint16_t)mode) << 8;
}

AccessMode Instruction::getArg1Mode() const {
    if(type == InsType::DAT)
        throw instruction_error("Cannot retrieve destination access mode from DAT type");

    return (AccessMode)((data & 0x0200) >> 9);
}

AccessMode Instruction::getArg2Mode() const {
    if(type == InsType::DAT)
        throw instruction_error("Cannot retrieve source access mode from type DAT type");

    return (AccessMode)((data & 0x0100) >> 8);
}


void Instruction::setRoute(Location arg1, Location arg2) {
    if(type == InsType::DAT)
        throw instruction_error("Cannot set route for DAT type");

    data &= 0xFF00; //1111 1111 0000 0000
    data |= (uint16_t)routeToBinary(arg1, arg2);

    //update imd_count
    imd_count = 0;
    if(arg1 == Location::IMD || arg1 == Location::PIMD) ++imd_count;
    if(arg2 == Location::IMD || arg2 == Location::PIMD) ++imd_count;
}

std::pair<Location, Location> Instruction::getRoute() const {
    if(type == InsType::DAT)
        throw instruction_error("Cannot get route for DAT type");

    return binaryToRoute((uint8_t)(data & 0x00FF));
}


void Instruction::setData(uint16_t d) {
    if(type != InsType::DAT)
        throw instruction_error("Data is specific to a DAT type value");

    data = d;
}

void Instruction::setData(const ArgVal* d) {
    if(!d || !d->isNum()) {
        setData((uint16_t)0);
        return;
    }
    setData(convertInt(d->getNum()));
}

void Instruction::setImds(const ArgVal* arg1, const ArgVal* arg2) {
    std::pair<Location, Location> route = getRoute();

    if(arg1) {
        switch(route.first) {
        case Location::IMD:
        case Location::PIMD:
            setImd1(arg1);
            break;
        default:
            break;
            //Do nothing
        }
    }
    if(arg2) {
        switch(route.second) {
        case Location::IMD:
        case Location::PIMD:
            if(imd_count < 2) setImd1(arg2);
            else setImd2(arg2);
            break;
        default:
            break;
            //Do nothing
        }
    }
}

void Instruction::setImd1(const ArgVal* d) {
    if(!d || !d->isNum()) return;
    imd1 = convertInt(d->getNum());
}

void Instruction::setImd2(const ArgVal* d) {
    if(!d || !d->isNum()) return;
    imd2 = convertInt(d->getNum());
}

void Instruction::write(FILE* fd) const {
    //while it would be faster to just go through all of them in one call,
    //  there is a potential of the class being changed in a way which causes
    //  problems

    uint16_t t = boost::endian::native_to_big(data);
    fwrite((void*)&t, 2, 1, fd);

    //don't need to do anything more
    if(type == InsType::DAT) return;
    if(imd_count > 2) throw instruction_error("Arg count cannot be greater than 2");

    //make sure to write only what is actually relevent
    if(imd_count > 0) {
        t = boost::endian::native_to_big(imd1);
        fwrite((void*)&t, 2, 1, fd);
    }
    if(imd_count > 1) {
        t = boost::endian::native_to_big(imd2);
        fwrite((void*)&t, 2, 1, fd);
    }
}



uint8_t Instruction::routeToBinary(Location arg1, Location arg2) {
    int8_t arg1v = Location_Vals.at(arg1).first;
    int8_t arg2v = Location_Vals.at(arg2).second;
    if(arg1v < -1) throw instruction_error("Invalid destination argument");
    if(arg2v < -1) throw instruction_error("Invalid source argument");

    uint8_t route = 0x00;
    if(arg1v == -1) { //special cases
        // currently no special cases
    }
    else route |= (uint8_t)arg1v;

    if(arg2v == -1) { //special cases
        //NONE is currently the only thing that qualifies
        // so leave arg2 as 0 since it does not matter
    }
    else if(arg2v == -2) {/* D o  n o t h i n g*/}
    else route |= ((uint8_t)arg2v) << 4;

    return route;
}

std::pair<Location, Location> Instruction::binaryToRoute(uint8_t binary) {
    std::pair<Location, Location> dstsrc(Location::NONE, Location::NONE);
    Location& arg1 = dstsrc.first;
    Location& arg2 = dstsrc.second;

    //Normal case
    int8_t arg1v = (int8_t)(binary & 0x0F);
    int8_t arg2v = (int8_t)(binary >> 4);

    //Find the destination and source, invalid if not found
    for(const auto& loc : Location_Vals) {
        if(loc.second.first  == arg1v) arg1 = loc.first;
        if(loc.second.second == arg2v) arg2 = loc.first;
    }
    if(arg1 == Location::NONE && arg2 == Location::NONE)
        throw instruction_error("Invalid route " + std::bitset<8>(binary).to_string());

    return dstsrc;
}

uint16_t Instruction::convertInt(int64_t d) {
    //This may seem stupid, but people can this way specifiy two's compliment on their own
    // or specify a negitive to get an unsigned value. Ultimately it is about the context of
    // where it is used that decides things
    if(d > UINT16_MAX) {
        std::cerr << "Truncating data with value: " << d << std::endl;
        return UINT16_MAX;
    } else if(d < INT16_MIN) {
        std::cerr << "Truncating data with value: " << d << std::endl;
        return (uint16_t)INT16_MIN;
    } else {
        return (uint16_t)d;
    }
}
