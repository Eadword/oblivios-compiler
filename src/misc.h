#pragma once

#include <regex>
#include <string>
#include <vector>

#include "line.h"

#define DEBUG

using std::string;
typedef std::vector<string> str_vec;
typedef std::map<string, string> str_map;
typedef std::map<string, uint16_t> label_map;
typedef std::vector<Line> line_vec;
typedef std::vector<uint64_t> code_vec;


line_vec readFile(const string& filename);
void applyReplace(line_vec& lines, const std::regex& pattern, string new_val = "");


#ifdef DEBUG
void printStrMap(const str_map& map);
void printLines(const line_vec& lines);
#else
#define printStrMap(var)
#define printLines(var)
#endif

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