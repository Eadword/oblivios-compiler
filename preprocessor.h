#pragma once

#include <map>

typedef std::vector<std::string> str_vec;

namespace Preprocessor {
    void run(std::string& data);
    std::map<std::string, std::string> getMacros(std::string data);
}