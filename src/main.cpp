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

    // don't need macros any more
    for(auto&& macro : macros) {
        #ifdef DEBUG
        std::cout << macro.first << ": " << *macro.second << std::endl;
        #endif

        delete macro.second;
    }

    std::map<string, uint32_t> labels;
    // create list of all labels and what true line they point to
    for(uint32_t x = 0; x < lines.size(); ++x) {
        const Labels* lbls = lines[x]->labels;
        if(!lbls) continue;

        for(Labels::iterator lbl = lbls->begin(); lbl != lbls->end(); ++lbl) {
            if(labels.find(*lbl) != labels.end())
                std::cerr << "Multiple definitions of label: "  << *lbl << std::endl;
            else
                labels[*lbl] = x;
        }
    }

    // actually compile the program
    for(uint32_t x = 0; x < lines.size(); ++x) {
        const Line* line = lines[x];

        // convert labels to offsets
        if(line->dst && !line->dst->val->isNum()) {
            try {
                uint32_t loc = labels.at(*line->dst->val->getStr());
                if(line->dst->pointer) std::cerr << "Labels should not be pointers: (" << *line << ")" << std::endl;
                delete line->dst->val;
                line->dst->val = new ArgVal(static_cast<int64_t>(loc) - static_cast<int64_t>(x));
                line->dst->mode = AccessMode::RELATIVE;
            } catch(std::out_of_range& e) {
                // do nothing
            }
        }
        if(line->src && !line->src->val->isNum()) {
            try {
                uint32_t loc = labels.at(*line->src->val->getStr());
                if(line->src->pointer) std::cerr << "Labels should not be pointers: (" << *line << ")" << std::endl;
                delete line->src->val;
                line->src->val = new ArgVal(static_cast<int64_t>(loc) - static_cast<int64_t>(x));
                line->src->mode = AccessMode::RELATIVE;
                if(line->dst->pointer) std::cerr << "Labels cannot be pointers" << "\n";
            } catch(std::out_of_range& e) {
                // do nothing
            }
        }

        // convert to binary

        // export the data to the binary file

        // free up memory
        #ifdef DEBUG
        std::cout << *line << std::endl;
        #endif
        delete line;
    }

    return 0;
}
