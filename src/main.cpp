#include <iostream>

#include "compiler_exception.h"
#include "parser.hpp"

extern FILE* yyin;
extern FILE* yyout;

int main() {
//    yydebug = 1;
    return yyparse();
}

/*int main(int argc, char** argv) {
    if(argc < 2) return 1;

    line_vec lines; // Store the unmodified for error printouts
    try {
        lines = readFile(argv[1]);
    } catch(std::invalid_argument e) {
        std::cout << e.what() << std::endl;
        return -1;
    }

    try {
        Preprocessor::run(lines);
    } catch(compiler_exception e) {
        std::cout << e.what() << std::endl;
        return -1;
    }

    try {
        Compiler::run(lines);
    } catch(compiler_exception e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}*/
