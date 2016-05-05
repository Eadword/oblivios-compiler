#pragma  once

namespace Patterns { // Eventually this could read from a config file
    const std::regex comment(";.*");
    const std::regex white_space("[ \\t\\v]+");
    const std::regex eol_white_space("[ \\t\\v]+$");
    const std::regex bol_white_space("^[ \\t\\v]+");

    inline std::regex findSting(std::string str) {
        return std::regex("\\b" + str + "\\b");
    }

    /// Recognizes entire line macro
    const std::regex macro_line("#(.*)");
    /// Matches sub parts of the macro
    const std::regex macro("([A-Z_a-z]\\w*)[ ]+(.*)");

    /// The ending iterator
    const std::sregex_iterator iterator_end = std::sregex_iterator();
}