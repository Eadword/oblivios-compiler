#pragma once

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

struct instruction_error : public std::runtime_error {
    /**
     * @param error The error incurred, e.g. "Cannot retrieve opcode from type storage"
     */
    instruction_error(std::string error) : std::runtime_error(error) {}
};

enum class InsType : uint8_t { STORAGE, IMMED, OFFSET };
enum class AccessMode : uint8_t { DIRECT, RELATIVE };
enum class DstSrc : uint8_t { DST, SRC };

#include "opcode.h"
#include "location.h"


/**
 * This is a specialized bitset of 32bits which wraps it providing easy access to the
 * different stored parts like the opcode or access mode.
 *
 * While many of the functions may not be necessary, they provide a level of security
 * as they verify the data is of the right type, and keep the code together if there
 * are changes to instruction representation.
 *
 * @see the language guide for specifics
 */
struct Instruction {
    InsType type;
    uint32_t data;

    Instruction(InsType type = InsType::STORAGE, uint32_t data = 0) :
            type(type), data(data) {}


    void setOPCode(OPCode);
    OPCode getOPCode();

    void setDestMode(AccessMode);
    void setSrcMode(AccessMode);
    AccessMode getDestMode();
    AccessMode getSrcMode();

    static uint8_t routeToBinary(Location dst, Location src);
    static std::pair<Location, Location> binaryToRoute(uint8_t binary);

    void setRoute(Location dst, Location src);
    std::pair<Location, Location> getRoute();

    void setImmediate(uint16_t);
    uint16_t getImmediate();

    void setOffsetDst(DstSrc);
    DstSrc getOffsetDst();

    void setData(uint32_t);
    uint32_t getData();


    static bool isImmediate(const std::string& arg);
    static bool isRegister(const std::string& arg);

    ///Can be relevant even if it is not a pointer, e.g. jmp
    static AccessMode getMode(const std::string& arg);
    ///Is it surrounded with []
    static bool isPointer(const std::string& arg);

    ///Does it have a '+' or '-' a num; only relevant if isRegister
    static bool hasOffset(const std::string& arg);
};