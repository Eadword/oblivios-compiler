#include "compiler_exception.h"
#include "compiler.h"
#include "patterns.h"

void Compiler::run(line_vec& lines) {
    for(uint16_t cur_line = 0; cur_line < lines.size(); ++cur_line) {
        Line& line = lines[cur_line];
        std::smatch match;

        if(!std::regex_match(line.cur, match, Patterns::opcode))
            throw compiler_exception("Cannot recognize code line", lines, cur_line);

        OPCode opcode; try {
            opcode = OPCodeFromString(match[1]);
        } catch(std::invalid_argument e) {
            throw compiler_exception(e.what(), lines, cur_line);
        }

        switch(opcode) {
            //Special Cases, can't set opcode yet
            case OPCode::DAT: line.ins.type = InsType::STORAGE; break;

            case OPCode::MOV: case OPCode::SWP:
                // TODO: handle MOV and SWP without the specification
                throw compiler_exception("Generic MOV/SWP not yet supported", lines, cur_line);

            default:
                line.ins.type = InsType::IMMED;
                line.ins.setOPCode(opcode);
        }

        // string params = match[2];
    }
    printLines(lines, true);
}