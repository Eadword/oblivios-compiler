#include "compiler_exception.h"
#include "compiler.h"
#include "patterns.h"

void Compiler::run(line_vec& lines) {
    for(uint16_t cur_line = 0; cur_line < lines.size(); ++cur_line) {
        Line& line = lines[cur_line];
        std::smatch match;

        if(!std::regex_match(line.cur, match, Patterns::opcode))
            throw compiler_exception("Cannot recognize code line", lines, cur_line);


        //Set the opcode
        OPCode opcode; try {
            opcode = OPCodeFromString(match[1]);
        } catch(std::invalid_argument e) {
            throw compiler_exception(e.what(), lines, cur_line);
        }

        switch(opcode) {
            //Special Cases, can't set opcode yet
            case OPCode::DAT: line.ins.type = InsType::STORAGE; break;

            case OPCode::MOV: case OPCode::SWP:
                //TODO: handle MOV and SWP without the specification
                throw compiler_exception("Generic MOV/SWP not yet supported", lines, cur_line);

            default:
                line.ins.type = InsType::IMMED;
                line.ins.setOPCode(opcode);
        }


        //Get the parameters (only 1 or 2 atm)
        std::string arg_dst = match[2], arg_src = "";
        match = std::smatch();

        switch(getOPCodeParams(opcode)) {
            case 1:
                if(!std::regex_match(arg_dst, match, Patterns::onearg))
                    throw compiler_exception("Invalid argument", lines, cur_line);

                arg_dst = match[1];
                break;

            case 2:
                if(!std::regex_match(arg_dst, match, Patterns::twoarg))
                    throw compiler_exception("Invalid argument(s)", lines, cur_line);

                arg_dst = match[1];
                arg_src = match[2];
                break;

            default:
                throw std::invalid_argument("Only opcodes with 1 or 2 arguments are supported");
        }



    }
    printLines(lines, true);
}