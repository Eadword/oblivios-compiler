#include <regex>
#include <iostream>

#include "patterns.h"
#include "preprocessor.h"
#include "compiler_exception.h"

void Preprocessor::run(line_vec& lines) {
    str_map macros = getMacros(lines);
    //TODO: replace the instances of the macros which are found
}

str_map Preprocessor::getMacros(const line_vec& lines) {

    str_map macros;
    //for(const Line& line : lines) {
    for(unsigned int cur_line = 0; cur_line < lines.size(); ++cur_line) {
        const Line& line = lines[cur_line];
        std::smatch line_match, macro_match;

        //Attempt to match entire line to vague description, i.e. a line starting with a '#'
        if(!std::regex_match(line.cur, line_match, Patterns::macro_line)) continue;

        //We found line which should represent a macro; attempt to read...
        string tmp = line_match[1];
        if(!std::regex_match(tmp, macro_match, Patterns::macro))
            throw compiler_exception("Invalid Macro", lines, cur_line);

        //It is valid formatting, now verify the name is unique
        if(macros.find(macro_match[1]) != macros.end())
            throw identifier_exception("Non-unique Identifier", macro_match[1], lines, cur_line);

        //All is good, at it to macro list
        macros[macro_match[1]] = macro_match[2];
    }

    printStrMap(macros);
    return macros;
}