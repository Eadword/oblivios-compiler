#include <iostream>
#include <regex>

#include "instruction.h"
#include "parser.h"
#include "patterns.h"

void Instruction::setOPCode(OPCode code) {
    if(type == InsType::DAT)
        throw instruction_error("Cannot set opcode for DAT type");

    data &= 0x03FF; // 0000 0011 1111 1111
    data |= ((uint16_t)code) << 10;
}

OPCode Instruction::getOPCode() {
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

AccessMode Instruction::getDestMode() {
    if(type == InsType::DAT)
        throw instruction_error("Cannot retrieve destination access mode from DAT type");

    return (AccessMode)((data & 0x0200) >> 9);
}

AccessMode Instruction::getSrcMode() {
    if(type == InsType::DAT)
        throw instruction_error("Cannot retrieve source access mode from type DAT type");

    return (AccessMode)((data & 0x0100) >> 8);
}


void Instruction::setRoute(Location dst, Location src) {
    if(type == InsType::DAT)
        throw instruction_error("Cannot set route for DAT type");

    data &= 0xFF00; //1111 1111 0000 0000
    data |= (uint16_t)routeToBinary(dst, src);
}

std::pair<Location, Location> Instruction::getRoute() {
    if(type == InsType::DAT)
        throw instruction_error("Cannot get route for DAT type");

    return binaryToRoute((uint8_t)(data & 0x00FF));
}


void Instruction::setData(uint16_t d) {
    if(type != InsType::DAT)
        throw instruction_error("Data is specific to a DAT type value");

    data = d;
}

void Instruction::setData(ArgVal* d) {
    if(!d || !d->isNum()) {
        setData((uint16_t)0);
        return;
    }

    int64_t val = d->getNum();
    //This may seem stupid, but people can this way specifiy two's compliment on their own
    // or specify a negitive to get an unsigned value. Ultimately it is about the context of
    // where it is used that decides things
    if(val > UINT16_MAX) {
        std::cerr << "Truncating data with value: " << d << std::endl;
        setData(UINT16_MAX);
    } else if(val < INT16_MIN) {
        std::cerr << "Truncating data with value: " << d << std::endl;
        setData((uint16_t)INT16_MIN);
    } else {
        setData((uint16_t)val);
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
