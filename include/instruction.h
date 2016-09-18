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
    /// Type of instruction
    InsType type;
    /// Binary data of the instruction
    uint16_t data;

    /// Number of immediates being stored
    uint8_t imds;
    /// Destination immediate value
    uint16_t imd_dst;
    /// Source immediate value
    uint16_t imd_src;


    Instruction(InsType type = InsType::DAT, uint32_t data = 0) :
            type(type), data(data), imds(0), imd_dst(0), imd_src(0) {}


    void setOPCode(OPCode);
    OPCode getOPCode() const;

    void setDestMode(AccessMode);
    void setSrcMode(AccessMode);
    AccessMode getDestMode() const;
    AccessMode getSrcMode() const;

    void setRoute(Location dst, Location src);
    std::pair<Location, Location> getRoute() const;

    void setData(uint16_t);

    /**
     * Converts the int64_t in argval
     */
    void setData(const ArgVal*);

    /// Checks route to decide if/what to set
    void setImds(const ArgVal* dst, const ArgVal* src);
    void setImdDst(const ArgVal*);
    void setImdSrc(const ArgVal*);

    void write(FILE*) const;


    static uint8_t routeToBinary(Location dst, Location src);
    static std::pair<Location, Location> binaryToRoute(uint8_t binary);
    static uint16_t convertInt(int64_t);
};
