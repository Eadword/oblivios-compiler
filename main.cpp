#include "preprocessor.h"
#include "misc.h"

//https://www.emacswiki.org/emacs/RegularExpression
inline str_vec readLines(const std::string& filename) {
    str_vec lines;
    std::ifstream code_file(filename);
    if(!code_file) throw std::invalid_argument("Could not open file: " + filename);

    std::string line;
    while(std::getline(code_file, line))
        lines.push_back(line);

    code_file.close();
    printLines(lines);
    return lines;
}

void removeComments(str_vec& lines) {
    const std::regex comment(";.*");
    applyReplace(lines, comment);
    //printLines(lines);
}


int main(int argc, char** argv) {
    if(argc < 2) return 1;

    str_vec code; // Store the unmodified for error printouts
    try {
        code = readLines(argv[1]);
    } catch(std::invalid_argument e) {
        std::cout << e.what() << std::endl;
        return -1;
    }

    removeComments(code);
    //Preprocessor::run(data);
    return 0;
}