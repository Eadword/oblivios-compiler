#pragma  once

namespace Patterns { // Eventually this could read from a config file
    using std::regex;

    const regex comment(";.*");
    const regex white_space("[ \\t\\v]+");
    const regex afc_white_space(", ");
    const regex eol_white_space(" $");
    const regex bol_white_space("^ ");

    inline regex findSting(std::string str) {
        return regex("\\b" + str + "\\b");
    }

    /// Recognizes entire line macro
    const regex macro_line("#(.*)");
    /// Matches sub parts of the macro
    const regex macro("([A-Z_a-z]\\w*)[ ]+(.*)");

    /// The ending iterator
    const std::sregex_iterator iterator_end = std::sregex_iterator();
}