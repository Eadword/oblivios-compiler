#include <cstdio>
#include <iostream>
#include <boost/xpressive/xpressive.hpp>

namespace regex = boost::xpressive;

std::string readData(char* filename) {
    FILE* code_file = fopen(filename, "r");
    if(code_file == nullptr) return "";

    fseek(code_file, 0, SEEK_END);
    size_t fsize = ftell(code_file) * sizeof(char);
    rewind(code_file);

    char* buffer = (char*)malloc(fsize);
    fread(buffer, 1, fsize, code_file);
    fclose(code_file);
    std::string data(buffer, fsize);
    free(buffer);

    return data;
}

int main(int argc, char** argv) {
    if(argc < 2) return 1;

    std::string data = readData(argv[1]);
    std::cout << data << "\n";

    return 0;
}