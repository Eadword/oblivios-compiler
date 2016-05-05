#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "line.h"

#define DEBUG

using std::string;
typedef std::vector<string> str_vec;
typedef std::map<string, string> str_map;
typedef std::vector<Line> line_vec;

inline void printLines(const line_vec& lines) {
#ifdef DEBUG
    const static char seperator[] = "====================================";
    std::cout << seperator << std::endl;
    for(const Line& line: lines)
        std::cout << line.num << ":\t" << line.cur << std::endl;
    std::cout << seperator << std::endl;
#endif
}

inline void printStrMap(const str_map& map) {
#ifdef DEBUG
    const static char seperator[] = "------------------------------------";
    std::cout << seperator << std::endl;
    for(auto&& i : map)
        std::cout << i.first << ": " << i.second << std::endl;
    std::cout << seperator << std::endl;
#endif
}

inline void applyReplace(line_vec& lines, const std::regex& pattern, string new_val = "") {
    for(Line& line : lines)
        line.cur = std::regex_replace(line.cur, pattern, new_val);
}

// Example of regex_iterator
//    for(std::sregex_iterator i = std::sregex_iterator(data.begin(), data.end(), Patterns::macro_line); i != Patterns::iterator_end; ++i)  {
//        string line = (*i)[1];
//        std::smatch macro;
//
//        if(std::regex_match(line, macro, Patterns::macro)) {
//            if(macros.find(macro[1]) == macros.end())
//                macros[macro[1]] = macro[2];
//            else std::cout << "Redefinition of macro identifier \"" << macro[1] << '\"' << std::endl;
//        }
//        else {
//            std::cout << "Could not recognize macro definition \"" << line << "\"" << std::endl;
//        }
//    }