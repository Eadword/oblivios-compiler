#pragma once

#include <cstdint>
#include <stdexcept>
#include <cstdint>
#include <string>
#include <utility>

struct instruction_error : public std::runtime_error {
    /**
     * @param error The error incurred, e.g. "Cannot retrieve opcode from type storage"
     */
    instruction_error(std::string error) : std::runtime_error(error) {}
};

enum class InsType : uint8_t { STORAGE, IMMED, OFFSET };
enum class OPCode : uint8_t {
    INT,  MOVC, MOVW, MOVD,
    SWPC, SWPW, SWPD, ADD,
    SUB,  MUL,  IMUL, DIV,
    IDIV, SHL,  SHR,  NEG,
    NOT,  AND,  OR,   XOR,
    INC,  DEC,  CMP,  JMP,
    JG,   JGE,  JE,   JNE,
    JL,   JLE,  JC,   JNC,
    JO,   JNO
};
enum class AccessMode : uint8_t { DIRECT = 0, INDIRECT = 1 };
enum class Location : uint8_t {
    AL = 0x00, AH = 0x01, BL = 0x02, BH = 0x03,
    CL = 0x04, CH = 0x05, DL = 0x06, DH = 0x07,
    AX = 0x08, BX = 0x09, CX = 0x0A, DX = 0x0B,
    IAX= 0x0C, IBX= 0x0D, ICX= 0x0E, IDX= 0x0F,
    // Special Stuff
    IP, IMD
};
enum class DstSrc : uint8_t { DST = 0x00, SRC = 0x01 };


/**
 * This is a specialized bitset of 32bits which wraps it providing easy access to the
 * different stored parts like the opcode or access mode.
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
};