#include "preprocessor.h"
#include "compiler.h"
#include "compiler_exception.h"

int main(int argc, char** argv) {
    if(argc < 2) return 1;

    line_vec code; // Store the unmodified for error printouts
    try {
        code = readFile(argv[1]);
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
    printLines(code);
    return 0;
}