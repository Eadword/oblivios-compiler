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


    // Compiler
    const regex opcode("([A-Z]+) ?(.*)");
    const regex onearg("([0-9A-Z\\[\\]%+\\-]+)");
    const regex twoarg("([0-9A-Z\\[\\]%+\\-]+),([0-9A-Z\\[\\]%+\\-]+)");


    // Arguments
    ///A semi-loose description of an immediate
    const regex is_immed("%?-?[0-9A-F]*?[BODH]");
    const regex is_reg("%?\\[?[A-Z]+\\]?([+-][0-9A-F]*?[BODH])?");
    const regex is_relative("%.*");
    const regex is_pointer("%?\\[[A-Z]+\\]([+-][0-9A-F]*?[BODH])?");
    const regex has_offset("%?\\[?[A-Z]+\\]?[+-][0-9A-F]*?[BODH]");

    /// The ending iterator
    const std::sregex_iterator iterator_end = std::sregex_iterator();
}