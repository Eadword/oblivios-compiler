#include <fstream>
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