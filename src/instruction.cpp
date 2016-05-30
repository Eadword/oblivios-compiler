#include "instruction.h"

void Instruction::setOPCode(OPCode code) {
    if(type == InsType::STORAGE)
        throw instruction_error("Cannot set opcode for type storage");

    data &= 0x03FFFFFF; // 0000 0011 1111 1111 1111 1111 1111 1111
    data |= ((uint32_t)code) << 26;
}

OPCode Instruction::getOPCode() {
    if(type == InsType::STORAGE)
        throw instruction_error("Cannot retrieve opcode from type storage");

    //32bits, the first 6 are what we want, so shift right 26, and then convert to
    //an unsigned value and cast to uint8_t
    return (OPCode)(data >> 26);
}


void Instruction::setDestMode(AccessMode mode) {
    if (type == InsType::STORAGE)
        throw instruction_error("Canot set destination access mode for type storage");
    if((uint8_t)mode > 1)
        throw instruction_error("Invalid destination access mode");

    data &= 0xFDFFFFFF; // 1111 1101 1111 1111 1111 1111 1111 1111
    data |= ((uint32_t)mode) << 25;
}

void Instruction::setSrcMode(AccessMode mode) {
    if (type == InsType::STORAGE)
        throw instruction_error("Canot set source access mode for type storage");
    if((uint8_t)mode > 1)
        throw instruction_error("Invalid source access mode");

    data &= 0xFEFFFFFF; // 1111 1110 1111 1111 1111 1111 1111 1111
    data |= ((uint32_t)mode) << 24;
}

AccessMode Instruction::getDestMode() {
    if(type == InsType::STORAGE) throw instruction_error("Cannot retrieve destination access "
                                                        "mode from type storage");
    return (AccessMode)((data << 6) >> 31);
}

AccessMode Instruction::getSrcMode() {
    if(type == InsType::STORAGE) throw instruction_error("Cannot retrieve source access mode "
                                                        "from type storage");
    return (AccessMode)((data << 7) >> 31);
}


uint8_t Instruction::routeToBinary(Location dst, Location src) {
    //Special region, cannot send larger register to smaller one, so we are able to
    // specially define these for more possibilities
    if((dst <= Location::DH) && (src >= Location::AX) && (src <= Location::DX)) {
        throw instruction_error("Invalid pairing, the destination cannot be smaller "
                                        "than the source");
    }
    if(src == Location::IP) {
        if(dst >= Location::AX && dst <= Location::DX)
            return ((uint8_t)dst << 1);
        //else, it is invalid
        throw instruction_error("IP can only be sent to the 16-bit registers");
    }
    if(dst == Location::IMD) {
        if(src == Location::IMD) return 0x81;
        throw instruction_error("Destination cannot be an immediate");
    }


    //We have handled special cases, so clamp to AL through IDX
    if(dst > Location::IDX) throw instruction_error("Invalid destination");
    if(src == Location::IMD) src = dst; // When src==dst, then load immediate to location
    if(src > Location::IDX) throw instruction_error("Invalid source");


    //Handle the normal cases
    return ((uint8_t)src << 4) + (uint8_t)dst;
}

std::pair<Location, Location> Instruction::binaryToRoute(uint8_t binary) {
    std::pair<Location, Location>
            dstsrc((Location)(binary & 0x0F), (Location)(binary >> 4));
    Location& dst = dstsrc.first;
    Location& src = dstsrc.second;

    if(dst == src) {
        src = Location::IMD;
        return dstsrc;
    }
    if((dst <= Location::DH) && (src >= Location::AX) && (src <= Location::DX)) {
        if((uint8_t)dst == 0) {
            src = Location::IP;
            return dstsrc;
        }
        if(binary == 0x81) {
            src = dst = Location::IMD;
            return dstsrc;
        }
        throw instruction_error("Unrecognized special routing value");
    }

    //Handle the normal case
    return dstsrc;
}

void Instruction::setRoute(Location dst, Location src) {
    if(type == InsType::STORAGE)
        throw instruction_error("Cannot set route for type storage");

    uint32_t bin = routeToBinary(dst, src);
    bin <<= 16;
    data &= 0xFF00FFFF; //1111 1111 0000 0000 1111 1111 1111 1111
    data |= bin;
}

std::pair<Location, Location> Instruction::getRoute() {
    if(type == InsType::STORAGE)
        throw instruction_error("Cannot get route for type storage");

    return binaryToRoute((uint8_t)((data << 8) >> 24));
}


void Instruction::setImmediate(uint16_t val) {
    if(type != InsType::IMMED)
        throw instruction_error("Immediates must be stored in an Immediate type instruction");

    data &= 0xFFFF0000;
    data |= (uint32_t)val;
}

uint16_t Instruction::getImmediate() {
    if(type != InsType::IMMED)
        throw instruction_error("There is no immediate to retrieve");

    return (uint16_t)data & 0x0000FFFF;
}


void Instruction::setOffsetDst(DstSrc dst) {
    if(type != InsType::OFFSET)
        throw instruction_error("Offsets are specific to offset type instructions");

    data &= 0xFFFFEFFF //1111 1111 1111 1111 0111 1111 1111 1111
}