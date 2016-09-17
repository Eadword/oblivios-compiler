#pragma once
#include <set>
#include <string>

#include "instruction.h"
#include "opcode.h"

typedef std::set<std::string> Labels;
class Argument;
class ArgVal;


using std::string;

/**
 * This represents a line of code. It is useful for error purposes to store the original information about
 * the line so if/when there is an error, the user knows what it is from, and does not have to decipher it
 * from the partially compiled line.
 *
 * It also store the line number, because as things progress, blank lines will be removed and so on,
 * allowing easier debugging down the road.
 *
 * A org_line value of 0 indicates that it was generated for compilation purposes. (Though this may
 * not ever happen depending on my implementation)
 *
 * Compiled instructions are stored in this to further help with debugging, by keeping everything
 * together, makes it easy to see what lead to what and find mistakes.
 *
 * @note takes ownership of every pointer.
 */
struct Line {
    friend std::ostream& operator<<(std::ostream&, const Line&);

    /// Original line number
    const uint32_t org_line; //TODO: Figure out why this is not correct or stop storing it

    //Set of all labels that point to this line
    Labels* labels;
    OPCode opcode;
    Argument* dst;
    Argument* src;
    /// Compiled instruction
    Instruction ins;

    Line();
    Line(uint32_t org_line, Labels* labels, Argument* dst, Argument* src);

    ~Line();

    /**
     * Sets the opcode to the value.
     * @param val Must not be a number, otherwise it calls an error
     */
    void setOPCode(ArgVal* val);
    void compile();
};
