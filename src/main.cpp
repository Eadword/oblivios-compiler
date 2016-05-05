#include <fstream>

#include "preprocessor.h"
#include "patterns.h"
#include "compiler_exception.h"

inline line_vec readLines(const string& filename) {
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


int main(int argc, char** argv) {
    if(argc < 2) return 1;

    line_vec code; // Store the unmodified for error printouts
    try {
        code = readLines(argv[1]);
    } catch(std::invalid_argument e) {
        std::cout << e.what() << std::endl;
        return -1;
    }

    try {
        Preprocessor::run(code);
    } catch(compiler_exception e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}