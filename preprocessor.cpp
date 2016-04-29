#include <regex>
#include <iostream>

#include "preprocessor.h"

void Preprocessor::run(std::string& data) {
    getMacros(data);
}

std::map<std::string, std::string> Preprocessor::getMacros(std::string data) {
    const std::regex macro_line_regex("#(.*)\\n");
    const std::regex macro_regex("([A-Z_a-z]\\w*)[ \\t\\v]+(.*)");
    const std::sregex_iterator iterator_end = std::sregex_iterator();

    std::map<std::string, std::string> macros;

    for(std::sregex_iterator i = std::sregex_iterator(data.begin(), data.end(), macro_line_regex);
        i != iterator_end; ++i)
    {
        std::string line = (*i)[1];
        std::smatch macro;

        if(std::regex_match(line, macro, macro_regex)) {
            if(macros.find(macro[1]) == macros.end())
                macros[macro[1]] = macro[2];
            else std::cout << "Redefinition of macro identifier \"" << macro[1] << '\"' << std::endl;
        }
        else {
            std::cout << "Could not recognize macro definition \"" << line << "\"" << std::endl;
        }
    }

#ifdef DEBUG
    for(auto i = macros.begin(); i != macros.end(); ++i)
        std::cout << "Found macro: #" << i->first << ' ' << i->second << std::endl;
#endif

    return macros;
}