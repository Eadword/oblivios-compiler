#pragma once

#include "instruction.h"

using std::string;

/**
 * This represents a line of code. It is useful for error purposes to store the original information about
 * the line so if/when there is an error, the user knows what it is from, and does not have to decipher it
 * from the partially compiled line.
 *
 * It also store the line number, because as things progress, blank lines will be removed and so on,
 * allowing easier debugging down the road.
 *
 * A line num of 0 indicates that it was generated for compilation purposes.
 *
 * Compiled instructions are stored in this to further help with debugging, by keeping everything
 * together, makes it easy to see what lead to what and find mistakes.
 */
struct Line {
    /// Original line number
    unsigned int num;
    /// Original string representing the line
    string org;
    /// Current string representing the line
    string cur;
    /// Compiled instruction
    Instruction ins;

    Line() {}

    /**
     * Constructs a valid line structure.
     * @note if cur is the empty string, it will be initialized to org.
     */
    Line(unsigned int num, string org, string cur = "") :
            num(num), org(org), cur( (cur == "" ? org : cur) ) {}

    ~Line() {}
};