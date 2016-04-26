#include <regex>
#include <iostream>

#include "preprocessor.h"

void Preprocessor::run(std::string& data) {
    getMacros(data);
}

std::map<std::string, std::string> Preprocessor::getMacros(std::string data) {
    std::map<std::string, std::string> macros;
    std::regex macro("#(.*)\n");

    std::smatch result;
    while(std::regex_search(data, result, macro)) {
        std::cout << "[" << result[1] << "]\n";
        data = result.suffix().str();
    }

    return macros;
}