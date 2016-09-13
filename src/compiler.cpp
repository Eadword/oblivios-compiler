#include "compiler_exception.h"
#include "compiler.h"
#include "patterns.h"

#define CEXCP(msg) throw compiler_exception(msg, lines, cur_line)

struct Arg {
    std::string val;
    Location loc;       //NONE if false
    bool pointer;       //Anything can be a pointer
    AccessMode mode;    //relative is valid always when pointer or a jmp, otherwise has no effect

    Arg() : val(""), loc(Location::NONE), pointer(false), mode(AccessMode::DIRECT) {}

//    void setVals(std::string val) {
//        this->val = val;
//        loc = Instruction::getLocation(val);
//
//        if(loc != Location::IMD) offset = Instruction::hasOffset(val);
//        pointer = Instruction::isPointer(val);
//        mode = Instruction::getMode(val);
//    }
};

//void Compiler::run(line_vec& lines) {
//    for(uint16_t cur_line = 0; cur_line < lines.size(); ++cur_line) {
//        Line& line = lines[cur_line];
//        std::smatch match;
//
//        if(!std::regex_match(line.cur, match, Patterns::opcode))
//            CEXCP("Cannot recognize code line");
//
//
//        //Set the opcode
//        OPCode opcode; try {
//            opcode = OPCodeFromString(match[1]);
//        } catch(std::invalid_argument e) {
//            throw compiler_exception(e.what(), lines, cur_line);
//        }
//
//        switch(opcode) {
//            //Special Cases, can't set opcode yet
//            case OPCode::DAT: line.ins.type = InsType::STORAGE; break;
//
//            case OPCode::MOV: case OPCode::SWP:
//                //TODO: handle MOV and SWP without the specification
//                CEXCP("Generic MOV/SWP not yet supported");
//
//            default:
//                line.ins.type = InsType::IMMED; //this can become an OFFSET type later
//                line.ins.setOPCode(opcode);
//        }
//
//
//        //Get the parameters (only 1 or 2 atm)
//        std::string args = match[2];
//        Arg arg_dst;
//        Arg arg_src;
//        match = std::smatch();
//
//        //TODO: if arg_src and arg_dst are both NONE it is invalid and if src is NONE and dst is IMD change to RIMD
//        //TODO: Set this up where it will always read two parameters, and if it is an empty string it will be read
//        // Location::NONE. That will get rid of the annoying problems of two cases and what not.
//
//        switch(getOPCodeParams(opcode)) {
//            case 1:
//                if(!std::regex_match(args, match, Patterns::onearg))
//                    CEXCP("Invalid argument");
//
//                try {
//                    arg_dst = new Arg(match[1]);
//                } catch(instruction_error e) {
//                    CEXCP(e.what());
//                }
//
//                if(opcode == OPCode::DAT) {
//                    //DAT is unfortunately picky, could shorten to one error, but
//                    // that would not give useful feedback for fixing the problem
//                    if(arg_dst->offset)
//                        CEXCP("DAT command cannot have an offset");
//                    if(arg_dst->pointer)
//                        CEXCP("DAT command cannot be a pointer");
//                    if(arg_dst->loc != Location::IMD)
//                        CEXCP("DAT command must be an immediate");
//                }
//
//                if(arg_dst->offset) {
//                    line.ins.type = InsType::OFFSET;
//                }
//
//                break;
//
//            case 2:
//                if(!std::regex_match(args, match, Patterns::twoarg))
//                    CEXCP("Invalid argument(s)");
//
//                try {
//                    arg_dst = new Arg(match[1]);
//                    arg_src = new Arg(match[2]);
//                } catch(instruction_error e) {
//                    CEXCP(e.what());
//                }
//
//                if(arg_dst->loc == Location::IMD && arg_src->loc == Location::IMD)
//                    CEXCP("Instructions may only have one immediate value");
//                if(arg_dst->offset && arg_src->offset)
//                    CEXCP("Instructions may only have one offset");
//                if((arg_dst->loc == Location::IMD || arg_src->loc == Location::IMD) &&
//                        (arg_dst->offset || arg_src->offset))
//                    CEXCP("Instructions cannot have both an offest and an immediate");
//
//                if(arg_dst->offset || arg_src->offset) line.ins.type = InsType::OFFSET;
//                break;
//
//            default:
//                throw std::invalid_argument("Only opcodes with 1 or 2 arguments are supported");
//        }
//    }
//    printLines(lines, true);
//}