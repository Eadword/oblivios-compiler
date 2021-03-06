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
    //TODO: allow for offsets (may not be possible)

    /// Type of instruction
    InsType type;
    /// Binary data of the instruction
    uint16_t data;

    /// Number of immediates being stored
    uint8_t imd_count;
    /// Destination immediate value
    uint16_t imd1;
    /// Source immediate value
    uint16_t imd2;


    Instruction(InsType type = InsType::DAT, uint32_t data = 0) :
            type(type), data(data), imd_count(0), imd1(0), imd2(0) {}


    void setOPCode(OPCode);
    OPCode getOPCode() const;

    void setArg1Mode(AccessMode);
    void setArg2Mode(AccessMode);
    AccessMode getArg1Mode() const;
    AccessMode getArg2Mode() const;

    void setRoute(Location arg1, Location arg2);
    std::pair<Location, Location> getRoute() const;

    void setData(uint16_t);

    /**
     * Converts the int64_t in argval
     */
    void setData(const ArgVal*);

    /// Checks route to decide if/what to set
    void setImds(const ArgVal* arg1, const ArgVal* arg2);
    void setImd1(const ArgVal*);
    void setImd2(const ArgVal*);

    void write(FILE*) const;


    static uint8_t routeToBinary(Location arg1, Location arg2);
    static std::pair<Location, Location> binaryToRoute(uint8_t binary);
    static uint16_t convertInt(int64_t);
};
