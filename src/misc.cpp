#include <fstream>
#include <iostream>
#include <bitset>

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
void printLines(const line_vec& lines, bool bin) {
    const static char seperator[] = "====================================";
    std::cout << seperator << std::endl;
    unsigned int cur_line = 0;
    for(const Line& line: lines) {
        if(!bin) printf("%3u|%3u: %s\n", cur_line++, line.num, line.cur.c_str());
        else
            printf("%3u|%3u: %-20s=> %01u %16s, %16s, %16s\n", cur_line++, line.num, line.cur.c_str(),
                   (uint8_t)line.ins.type,
                   std::bitset<16>(line.ins.data).to_string().c_str(),
                   std::bitset<16>(line.ins.imd_dst).to_string().c_str(),
                   std::bitset<16>(line.ins.imd_src).to_string().c_str()
            );
    }
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