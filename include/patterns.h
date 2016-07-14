#pragma  once

namespace Patterns { // Eventually this could read from a config file
    using std::regex;

    // Generic
    inline regex findSting(std::string str) {
        return regex("\\b" + str + "\\b");
    }


    // Preprocessor
    /// A comment starting at a ; and going to EOL
    const regex comment(";.*");
    /// Non-line-ending whitespace
    const regex white_space("[ \\t\\v]+");
    /// Spaces at the end of a line
    const regex eol_white_space(" +$");
    /// Spaces leading in a line
    const regex bol_white_space("^ +");

    /// A line beginning with a '#'
    const regex macro_line("#(.*)");
    /// A macro broken into identifier and replacement value; valid macro_lines must be matchable by a macro
    const regex macro("([A-Z_a-z]\\w*)[ ]+(.*)");
    /// A line containing at least one ':'
    //const regex label_line("(.*?):.*");
    //const regex label("(?:^|:)([A-Z_a-z]\\w*)");
    const regex label("([A-Z_a-z]\\w*):");

    ///Loose definition of a number
    const regex loose_number("([+-]?)(0X)?([0-9A-F_]+?)([BODH])?\\b");


    //TODO: expressions like 0x05 * (3d + 10o)
    //TODO: negatives with ops, e.g. "+-" -> "-" and "--" -> "+"


    // Compiler
    const regex opcode("([A-Z]+) ?(.*)");
    const regex onearg("([0-9A-Z\\[\\]%+\\-]+)");
    const regex twoarg("([0-9A-Z\\[\\]%+\\-]+),([0-9A-Z\\[\\]%+\\-]+)");


    // Arguments
    const regex get_location("%?\\[?(-?[A-Z0-9]+)\\]?([+-][0-9]+)?");
    const regex is_relative("%.*");
    const regex is_pointer("%?\\[[A-Z]+\\]([+-]\\d+)?");
    const regex has_offset("%?\\[?[A-Z]+\\]?[+-]\\d+");

    /// The ending iterator
    const std::sregex_iterator iterator_end = std::sregex_iterator();
}