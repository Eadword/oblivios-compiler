#include <cstdio>
#include <iostream>
#include <regex>

#include "preprocessor.h"

//https://www.emacswiki.org/emacs/RegularExpression
inline std::string readData(char* filename) {
    FILE* code_file = fopen(filename, "r");
    if(code_file == nullptr) return "";

    fseek(code_file, 0, SEEK_END);
    size_t file_size = ftell(code_file) * sizeof(char);
    rewind(code_file);

    char* buffer = (char*)malloc(file_size);
    fread(buffer, 1, file_size, code_file);
    fclose(code_file);
    std::string data(buffer, file_size);
    free(buffer);
    data += '\n';
    return data;
}

inline void removeComments(std::string& data) {
    data = std::regex_replace(data, std::regex(" *;.*\n"), "\n");
}



int main(int argc, char** argv) {
    if(argc < 2) return 1;

    std::string data = readData(argv[1]);
    removeComments(data);
    Preprocessor::run(data);
    return 0;
}