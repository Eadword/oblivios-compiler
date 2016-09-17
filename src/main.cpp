#include <cstdio>
#include <iostream>

#include "compiler_exception.h"
#include "parser.hpp"

extern FILE* yyin;
extern int yyparse(void);
extern std::map<string, ArgVal*> macros;
extern std::vector<Line*> lines;

int main(int argc, char** argv) {
    if(argc < 2) return 1;
    FILE* fd = fopen(argv[1], "r");
    if(!fd) {
        std::cout << "Could not open file." << std::endl;
        return -1;
    }
    yyin = fd;
    yyparse();
    fclose(fd);

    for(auto&& i : macros) {
        #ifdef DEBUG
        std::cout << i.first << ": " << *i.second << std::endl;
        #endif

        delete i.second;
    }
    for(auto&& i: lines) {
        #ifdef DEBUG
        std::cout << *i << std::endl;
        #endif
        delete i;
    }
    return 0;
}
