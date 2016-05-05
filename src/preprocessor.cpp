#include <regex>
#include <iostream>

#include <boost/algorithm/string.hpp>

#include "patterns.h"
#include "preprocessor.h"
#include "compiler_exception.h"

void Preprocessor::run(line_vec& lines) {
    removeComments(lines);
    removeWhiteSpace(lines);
    for(Line& i : lines) boost::to_upper(i.cur);

    str_map macros = getMacros(lines);
    replaceMacros(lines, macros);
}

void Preprocessor::removeComments(line_vec &lines) {
    applyReplace(lines, Patterns::comment);
    printLines(lines);
}

void Preprocessor::removeWhiteSpace(line_vec& lines) {
    applyReplace(lines, Patterns::white_space, " ");
    applyReplace(lines, Patterns::eol_white_space);
    applyReplace(lines, Patterns::bol_white_space);
    for(auto i = lines.begin(); i != lines.end();) {
        if(i->cur.empty()) lines.erase(i);
        else ++i;
    }
    printLines(lines);
}

str_map Preprocessor::getMacros(line_vec& lines) {

    str_map macros;
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

        //All is good, add it to macro list
        macros[macro_match[1]] = macro_match[2];
        lines.erase(lines.begin() + cur_line--);
    }

    printStrMap(macros);
    printLines(lines);
    return macros;
}

void Preprocessor::replaceMacros(line_vec &lines, str_map& macros) {
    for(auto&& i : macros)
        applyReplace(lines, Patterns::findSting(i.first), i.second);
    printLines(lines);
}