#include <fstream>
#include <iostream>

#include "misc.h"

line_vec readFile(const string& filename) {
    std::ifstream code_file(filename);
    if(!code_file) throw std::invalid_argument("Could not open file: " + filename);

    line_vec lines;
    unsigned int line_num = 1;
    std::string line;

    while(std::getline(code_file, line)) {
        lines.push_back( Line(line_num++, line) );
    }

    code_file.close();
    printLines(lines);
    return lines;
}

void applyReplace(line_vec& lines, const std::regex& pattern, string new_val) {
    for(Line& line : lines)
        line.cur = std::regex_replace(line.cur, pattern, new_val);
}

#ifdef DEBUG
void printLines(const line_vec& lines) {
    const static char seperator[] = "====================================";
    std::cout << seperator << std::endl;
    for(const Line& line: lines)
        std::cout << line.num << ":\t" << line.cur << std::endl;
    std::cout << seperator << std::endl;
}

void debugStrMap(const std::map<string, string>& map) {
    const static char seperator[] = "------------------------------------";
    std::cout << seperator << std::endl;
    for(auto&& i : map)
        std::cout << i.first << ": " << i.second << std::endl;
    std::cout << seperator << std::endl;
}

void debugLabelMap(const std::map<string, uint16_t>& map) {
    const static char seperator[] = "------------------------------------";
    std::cout << seperator << std::endl;
    for(auto&& i : map)
        std::cout << i.first << ": " << i.second << std::endl;
    std::cout << seperator << std::endl;
}
#endif