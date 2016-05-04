#pragma once

#include <exception>
#include <string>
#include <stdexcept>

// This can be modified as needed to allow for better error handling
struct compiler_exception : public std::runtime_error {
    compiler_exception(std::string error, std::string code, unsigned int line) :
            std::runtime_error(error + " (" + std::to_string(line) + "): " + code) {}
};