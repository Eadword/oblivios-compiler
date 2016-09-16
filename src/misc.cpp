#include <fstream>
#include <iostream>
#include <bitset>

#include "misc.h"

// void applyReplace(line_vec& lines, const std::regex& pattern, string new_val) {
//     for(Line& line : lines)
//         line.cur = std::regex_replace(line.cur, pattern, new_val);
// }

#ifdef DEBUG
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
