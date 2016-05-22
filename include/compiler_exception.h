#pragma once

#include <exception>
#include <string>
#include <stdexcept>

#include "misc.h"

/**
 * A generic error to be thrown by the compiler.
 */
struct compiler_exception : public std::runtime_error {

    static string composeError(string error, const line_vec& lines, unsigned int line) {
        while(lines[line].num == 0) ++line; //find next line which was not generated
        return error + " (" + std::to_string(lines[line].num) + "): " + lines[line].org;
    }

    /**
     * @param error The error incurred, e.g. "Invalid Macro"
     * @param lines The lines of code within which the error occurred within
     * @param line  The current line where the error occurred (i.e. position in vector)
     */
    compiler_exception(string error, const line_vec& lines, unsigned int line) :
            std::runtime_error(composeError(error, lines, line)) {}
};

struct identifier_exception : public compiler_exception {

    identifier_exception(const string &error, const string &identifier,
                         const line_vec &lines, unsigned int line) :
            compiler_exception(error + " \"" + identifier + "\"", lines, line) {}
};