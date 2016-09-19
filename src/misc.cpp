#include <fstream>
#include <iostream>
#include <bitset>

#include "misc.h"
#include "parser.h"

// void applyReplace(line_vec& lines, const std::regex& pattern, string new_val) {
//     for(Line& line : lines)
//         line.cur = std::regex_replace(line.cur, pattern, new_val);
// }

void applyLabels(Argument* arg, const std::map<string, uint32_t>& labels, uint32_t cur_loc) {
    if(!arg || arg->val->isNum()) return;
    uint32_t lbl_loc;
    try {
        lbl_loc = labels.at(*arg->val->getStr());
    } catch(std::out_of_range& e) {
        return; // do nothing, it is not a label
    }

    delete arg->val; // replace the arg
    arg->val = new ArgVal(static_cast<int64_t>(lbl_loc) - static_cast<int64_t>(cur_loc));
}

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
