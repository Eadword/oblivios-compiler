#include "instruction.h"

void Instruction::setOPCode(OPCode code) {
    if(type == InstType::STORAGE)
        throw instruction_error("Cannot set opcode for type storage");

    data &= 0x03FFFFFF; // 0000 0011 1111 1111 1111 1111 1111 1111
    data |= ((uint32_t)code) << 26;
}

uint8_t Instruction::getOPCode() {
    if(type == InstType::STORAGE)
        throw instruction_error("Cannot retrieve opcode from type storage");

    //32bits, the first 6 are what we want, so shift right 26, and then convert to
    //an unsigned value and cast to uint8_t
    return (OPCode)(data >> 26);
}


void Instruction::setDestMode(AccessMode mode) {
    if (type == InstType::STORAGE)
        throw instruction_error("Canot set destination access mode for type storage");
    if(mode > 1)
        throw instruction_error("Invalid destination access mode");

    data &= 0xFDFFFFFF; // 1111 1101 1111 1111 1111 1111 1111 1111
    data |= ((uint32_t)mode) << 25;
}

void Instruction::setSrcMode(AccessMode mode) {
    if (type == InstType::STORAGE)
        throw instruction_error("Canot set source access mode for type storage");
    if(mode > 1)
        throw instruction_error("Invalid source access mode");

    data &= 0xFEFFFFFF; // 1111 1110 1111 1111 1111 1111 1111 1111
    data |= ((uint32_t)mode) << 24;
}

AccessMode Instruction::getDestMode() {
    if(type == InstType::STORAGE) throw instruction_error("Cannot retrieve destination access "
                                                        "mode from type storage");
    return (AccessMode)((data << 6) >> 31);
}

AccessMode Instruction::getSrcMode() {
    if(type == InstType::STORAGE) throw instruction_error("Cannot retrieve source access mode "
                                                        "from type storage");
    return (AccessMode)((data << 7) >> 31);
}


uint8_t Instruction::dstSrcToBinary(Location dst, Location src) {
    //Special region, cannot send larger register to smaller one, so we are able to
    // specially define these for more possibilities
    if((dest <= Location::DH) && (src >= Location::AX) && (src <= Location::DX)) {
        throw instruction_error("Invalid pairing, the destination cannot be smaller "
                                        "than the source");
    }
    if(src == Location::IP) {
        if(dst >= Location::AX && dst <= Location::DX)
            return (dst << 1);
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
    return (src << 4) + dst;
}

std::pair<Location, Location> Instruction::binaryToDstSrc(uint8_t binary) {
    std::pair<Location, Location> dstsrc((binary & 0x0F), (binary >> 4));
    Location& dst = dstsrc.first;
    Location& src = dstsrc.second;

    if(dst == src) {
        src = Location::IMD;
        return dstsrc;
    }
    if((dst <= Location::DH) && (src >= Location::AX) && (src <= Location::DX)) {
        if(dst == 0) {
            src = Location::IP;
            return dstsrc;
        }
        if(binary == 0x81) {
            src = dst = Location::IMD;
            return dstsrc;
        }
        throw instruction_error("Unrecognized special dst/src pair");
    }

    //Handle the normal case
    return dstsrc;
}