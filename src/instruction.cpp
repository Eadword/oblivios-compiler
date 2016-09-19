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


void Instruction::setDestMode(AccessMode mode) {
    if (type == InsType::DAT)
        throw instruction_error("Canot set destination access mode for DAT type");
    if((uint8_t)mode > 1)
        throw instruction_error("Invalid destination access mode");

    data &= 0xFDFF; // 1111 1101 1111 1111
    data |= ((uint16_t)mode) << 9;
}

void Instruction::setSrcMode(AccessMode mode) {
    if (type == InsType::DAT)
        throw instruction_error("Canot set source access mode for DAT type");
    if((uint8_t)mode > 1)
        throw instruction_error("Invalid source access mode");

    data &= 0xFEFF; // 1111 1110 1111 1111
    data |= ((uint16_t)mode) << 8;
}

AccessMode Instruction::getDestMode() const {
    if(type == InsType::DAT)
        throw instruction_error("Cannot retrieve destination access mode from DAT type");

    return (AccessMode)((data & 0x0200) >> 9);
}

AccessMode Instruction::getSrcMode() const {
    if(type == InsType::DAT)
        throw instruction_error("Cannot retrieve source access mode from type DAT type");

    return (AccessMode)((data & 0x0100) >> 8);
}


void Instruction::setRoute(Location dst, Location src) {
    if(type == InsType::DAT)
        throw instruction_error("Cannot set route for DAT type");
    if(src == Location::RIMD)
        throw instruction_error("Source cannot be RIMD");

    data &= 0xFF00; //1111 1111 0000 0000
    data |= (uint16_t)routeToBinary(dst, src);

    //have to do this because it does checks
    std::pair<Location, Location> route = getRoute();
    dst = route.first; src = route.second;
    //update argc
    argc = 0;
    if(dst == Location::RIMD || dst == Location::PIMD) ++argc;
    if(src == Location::IMD || src == Location::PIMD) ++argc;
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

void Instruction::setImds(const ArgVal* dst, const ArgVal* src) {
    std::pair<Location, Location> route = getRoute();

    if(dst) {
        switch(route.first) {
        case Location::RIMD:
        case Location::PIMD:
            setImdDst(dst);
            break;
        default:
            break;
            //Do nothing
        }
    }
    if(src) {
        switch(route.second) {
        case Location::IMD:
        case Location::PIMD:
            if(argc < 2) setImdDst(src);
            else setImdSrc(src);
            break;
        default:
            break;
            //Do nothing
        }
    }
}

void Instruction::setImdDst(const ArgVal* d) {
    if(!d || !d->isNum()) return;
    arg1 = convertInt(d->getNum());
}

void Instruction::setImdSrc(const ArgVal* d) {
    if(!d || !d->isNum()) return;
    arg2 = convertInt(d->getNum());
}

void Instruction::write(FILE* fd) const {
    //while it would be faster to just go through all of them in one call,
    //  there is a potential of the class being changed in a way which causes
    //  problems

    uint16_t t = boost::endian::native_to_big(data);
    fwrite((void*)&t, 2, 1, fd);

    //don't need to do anything more
    if(type == InsType::DAT) return;

    std::pair<Location, Location> route = getRoute();
    if(route.first == Location::RIMD) {
        t = boost::endian::native_to_big(arg1);
        fwrite((void*)&t, 2, 1, fd);
        return;
    }

    if(argc > 2) throw instruction_error("Arg count cannot be greater than 2");

    //make sure to write only what is actually relevent
    if(argc > 0) {
        t = boost::endian::native_to_big(arg1);
        fwrite((void*)&t, 2, 1, fd);
    }
    if(argc > 1) {
        t = boost::endian::native_to_big(arg2);
        fwrite((void*)&t, 2, 1, fd);
    }
}



uint8_t Instruction::routeToBinary(Location dst, Location src) {
    int8_t dstv = Location_Vals.at(dst).first;
    int8_t srcv = Location_Vals.at(src).second;
    if(dstv < -1) throw instruction_error("Invalid destination argument");
    if(srcv < -1) throw instruction_error("Invalid source argument");

    uint8_t route = 0x00;
    if(dstv == -1) { //special cases
        if(dst == Location::IMD && src == Location::NONE) dst = Location::RIMD;
        else if(dst == Location::IMD) throw instruction_error("Destination cannot be an immediate");

        if(dst == Location::RIMD) {
            route = 0xFF;
            srcv = -2;
        }
    }
    else route |= (uint8_t)dstv;

    if(srcv == -1) { //special cases
        //NONE is currently the only thing that qualifies
        // so leave src as 0 since it does not matter
    }
    else if(srcv == -2) {/* D o  n o t h i n g*/}
    else route |= ((uint8_t)srcv) << 4;

    return route;
}

std::pair<Location, Location> Instruction::binaryToRoute(uint8_t binary) {
    std::pair<Location, Location> dstsrc(Location::NONE, Location::NONE);
    Location& dst = dstsrc.first;
    Location& src = dstsrc.second;

    //Special cases
    if(binary == 0xFF) {
        dst = Location::RIMD;
        src = Location::NONE;
        return dstsrc;
    }

    //Normal case
    int8_t dstv = (int8_t)(binary & 0x0F);
    int8_t srcv = (int8_t)(binary >> 4);

    //Find the destination and source, invalid if not found
    for(const auto& loc : Location_Vals) {
        if(loc.second.first  == dstv) dst = loc.first;
        if(loc.second.second == srcv) src = loc.first;
    }
    if(dst == Location::NONE && src == Location::NONE)
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
