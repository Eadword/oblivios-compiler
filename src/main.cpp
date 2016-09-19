#include <cstdio>
#include <iostream>

#include "compiler_exception.h"
#include "parser.hpp"

extern FILE* yyin;
extern int yyparse(void);
extern std::map<string, ArgVal*> macros;
extern std::vector<Line*> lines;

int main(int argc, char** argv) {
    if(argc < 3) { std::cerr<< "Correct usage: compile src_file out_file" << std::endl; return 1; }
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

    // actually compile the program
    fd = fopen(argv[2], "w");
    //TODO: allow a label + offset for things which are part way though a line (and other reasons)
    std::map<string, uint32_t> labels;
    uint32_t offset = 0; // additional offset to account for immediates
    for(uint32_t x = 0; x < lines.size(); ++x) {
        Line* line = lines[x];
        const Labels* lbls = lines[x]->labels;

        // create list of all labels and what byte they point to
        if(lbls) {
            for(Labels::iterator lbl = lbls->begin(); lbl != lbls->end(); ++lbl) {
                if(labels.find(*lbl) != labels.end())
                    std::cerr << "Multiple definitions of label: "  << *lbl << std::endl;
                else
                    labels[*lbl] = offset;
            }
        }

        // convert labels to offsets
        applyLabels(line->dst, labels, offset);
        applyLabels(line->src, labels, offset);

        // convert to binary
        try {
            line->compile();
        } catch(std::invalid_argument& e) {
            std::cerr << e.what() << " (" << *line << ")" << std::endl;
        } catch(instruction_error& e) {
            std::cerr << e.what() << " (" << *line << ")" << std::endl;
        }
        //two bytes for the instruction and two bytes for every immediate value
        offset += line->ins.imd_count * 2 + 2;

        // export the data to the binary file
        line->ins.write(fd);

        // free up memory
        #ifdef DEBUG
        std::cout << *line << std::endl;
        #endif
        delete line;
    }
    fclose(fd);
    return 0;
}
