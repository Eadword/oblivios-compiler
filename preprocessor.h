#pragma once

#include <map>

namespace Preprocessor {
    void run(std::string& data);
    std::map<std::string, std::string> getMacros(std::string data);
}