#include "compiler_exception.h"
#include "compiler.h"
#include "patterns.h"

void Compiler::run(line_vec& lines) {
    for(uint16_t cur_line = 0; cur_line < lines.size(); ++cur_line) {
        Line& line = lines[cur_line];
        std::smatch match;

        if(!std::regex_match(line.cur, match, Patterns::opcode))
            throw compiler_exception("Cannot recognize code line", lines, cur_line);

        string opcode = match[1];
        string params = match[2];


    }
    printLines(lines, true);
}

