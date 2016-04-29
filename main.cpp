#include <cstdio>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "preprocessor.h"

//https://www.emacswiki.org/emacs/RegularExpression
inline std::vector<std::string> readLines(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream code_file(filename);
    if(!code_file) throw std::invalid_argument("Could not open file: " + filename);

    std::string line;
    while(std::getline(code_file, line)) {
        lines.push_back(line);
    }

    code_file.close();

#ifdef DEBUG
    for(auto&& i : lines)
        std::cout << i << std::endl;
#endif

    return lines;
}

inline void removeComments(std::string& data) {
    data = std::regex_replace(data, std::regex(" *;.*\n"), "\n");
}


int main(int argc, char** argv) {
    if(argc < 2) return 1;

    str_vec origional_code, code; // Store the unmodified for error printouts
    try {
        origional_code = readLines(argv[1]);
    } catch(std::invalid_argument e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    code = origional_code;

    //removeComments(code);
    //Preprocessor::run(data);
    return 0;
}