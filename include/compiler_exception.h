#pragma once

#include <exception>
#include <string>
#include <stdexcept>

#include "misc.h"

/**
 * A generic error to be thrown by the compiler.
 */
struct compiler_exception : public std::runtime_error {

    static string composeError(string error, uint32_t line) {
        return error + " (" + std::to_string(line) + ")";
    }

    /**
     * @param error The error incurred, e.g. "Invalid Macro"
     * @param line  The current line where the error occurred
     */
    compiler_exception(string error, uint32_t line) :
            std::runtime_error(composeError(error, line)) {}
};

struct identifier_exception : public compiler_exception {

    identifier_exception(const string &error, const string &identifier,
                         uint32_t line) :
            compiler_exception(error + " \"" + identifier + "\"", line) {}
};
