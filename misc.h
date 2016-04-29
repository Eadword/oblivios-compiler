#pragma once

#include <cstdio>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#define DEBUG

typedef std::vector<std::string> str_vec;

inline void printLines(const str_vec& lines) {
#ifdef DEBUG
    const static char seperator[] = "====================================";
    std::cout << seperator << std::endl;
    for(const std::string& line : lines)
        std::cout << line << std::endl;
    std::cout << seperator << std::endl;
#endif
}

inline void applyReplace(str_vec& lines, const std::regex& pattern) {
    for(std::string& line : lines)
        line = std::regex_replace(line, pattern, "");
}