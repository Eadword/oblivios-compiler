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
//    for(const Line& line : lines) {
//        std::smatch line_match;
//        if(std::regex_match(line.cur, line_match, Patterns::macro_line)) {
//            std::smatch macro_match;
//            if(std::regex_match(line_match.str(), macro_match, Patterns::macro)) {
//
//            }
//            else {
//                //throw compiler_exception("Invalid Macro", lines[cur], cur); //TODO: finish this, and have an error type and the actual instance of the error passed to the constructor instead of line number
//            }
//
//        }
//        //else continue (no match found)
//    }

#ifdef DEBUG
    for(auto i = macros.begin(); i != macros.end(); ++i)
        std::cout << "Found macro: #" << i->first << ' ' << i->second << std::endl;
#endif

    return macros;
}