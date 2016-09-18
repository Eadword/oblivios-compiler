#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "location.h"
#include "opcode.h"

struct instruction_error : public std::runtime_error {
    /**
     * @param error The error incurred, e.g. "Cannot retrieve opcode from type storage"
     */
    instruction_error(std::string error) : std::runtime_error(error) {}
};

enum class InsType : uint8_t { DAT, OP };
enum class AccessMode : uint8_t { DIRECT, RELATIVE };
enum class DstSrc : uint8_t { DST, SRC };

class ArgVal;

/**
 * This is a specialized bitset of 16bits which provides easy access to the different
 * stored parts like the opcode or access mode.
 *
 * While many of the functions may not be necessary, they provide a level of security
 * as they verify the data is of the right type, and keep the code together if there
 * are changes to instruction representation.
 *
 * @see the language guide for specifics
 */
struct Instruction {
    InsType type;
    uint16_t data;

    uint16_t imd_dst; //A ptr would take more space, so just store immediate
    uint16_t imd_src; // value regardless of whether it is needed or not


    Instruction(InsType type = InsType::DAT, uint32_t data = 0) :
            type(type), data(data), imd_dst(0), imd_src(0) {}


    void setOPCode(OPCode);
    OPCode getOPCode();

    void setDestMode(AccessMode);
    void setSrcMode(AccessMode);
    AccessMode getDestMode();
    AccessMode getSrcMode();

    void setRoute(Location dst, Location src);
    void setRout(ArgVal* dst, ArgVal* src);
    std::pair<Location, Location> getRoute();

    void setData(uint16_t);

    /**
     * Converts the int64_t in argval
     */
    void setData(ArgVal*);

    void setImdDst(ArgVal*);
    void setImdSrc(ArgVal*);

    void write(FILE*);


    static uint8_t routeToBinary(Location dst, Location src);
    static std::pair<Location, Location> binaryToRoute(uint8_t binary);
    static uint16_t convertInt(int64_t);
};
