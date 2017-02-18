#include <iostream>

#include "line.h"
#include "parser.h"

extern void yyerror(const char*);

Line::Line() : org_line(0), opcode(OPCode::NONE), labels(nullptr), dst(nullptr), src(nullptr) {}
Line::Line(uint32_t org_line, Labels* labels, Argument* dst, Argument* src) :
        org_line(org_line), opcode(OPCode::NONE), labels(labels), dst(dst), src(src) {}

Line::~Line() {
    delete labels;
    delete dst;
    delete src;
}

void Line::setOPCode(ArgVal* val) {
    if(val == nullptr || val->isNum()) {
        opcode = OPCode::NONE;
        yyerror("Invalid opcode");
        return;
    }

    try {
        opcode = OPCodeFromString(*val->getStr());
    } catch(std::invalid_argument& e) {
        yyerror(e.what());
    }
}

void Line::compile() {
    // set type of instruction
    // opcode is already set
    if(opcode == OPCode::DAT) {
        ins = Instruction(InsType::DAT);
        ins.setData(dst->val);
        return;
    }

    ins = Instruction(InsType::OP);


    // set the arguments
    std::pair<Location, Location> route(LocationFromArg(dst), LocationFromArg(src));
    ins.setRoute(route.first, route.second);
    ins.setImds(dst ? dst->val : nullptr, src ? src->val : nullptr);

    if(dst) ins.setArg1Mode(dst->mode);
    if(src) ins.setArg2Mode(src->mode);

    // set opcode and verfiy correcness of parameters for operator
    uint8_t expected_params = getOPCodeParams(opcode);
    const char* incorrect_params = "Incorrect number of parameters for operator";
    if(expected_params == 2) {
        if(route.first == Location::NONE || route.second == Location::NONE)
            throw std::invalid_argument(incorrect_params);
    } else if(expected_params == 1) {
        if(route.first == Location::NONE)
            throw std::invalid_argument(incorrect_params);
    } else if(expected_params == 0) {
        // not used at the time of writing
    } else {
        throw std::invalid_argument(incorrect_params);
    }
    ins.setOPCode(opcode);
}

std::ostream& operator<<(std::ostream& out, const Line& line) {
    out << line.org_line << "| ";

    if(line.labels != nullptr)
        for(Labels::iterator i = line.labels->begin(); i != line.labels->end(); ++i)
            out << *i << ": ";

    out << line.opcode << " ";
    out << *line.dst;
    if(line.src != nullptr)
        out << ", " << *line.src;
    return out;
}
