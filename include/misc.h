#pragma once

#include <regex>
#include <string>
#include <vector>

class Argument;

#define DEBUG

using std::string;
typedef std::vector<string> str_vec;
typedef std::map<string, string> str_map;
typedef std::map<string, uint16_t> label_map;

void applyLabels(Argument* arg, const std::map<string, uint32_t>& labels, uint32_t cur_loc);
//void applyReplace(line_vec& lines, const std::regex& pattern, string new_val = "");


#ifdef DEBUG
void debugStrMap(const std::map<string, string>&);
void debugLabelMap(const std::map<string, uint16_t>&);
#else
#define debugStrMap(var)
#define debugLabelMap(var)
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
