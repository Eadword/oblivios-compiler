#include "line.h"
#include "parser.h"

Line::Line() : org_line(0), labels(nullptr), dst(nullptr), src(nullptr) {}
Line::Line(uint32_t org_line, Labels* labels, Argument* dst, Argument* src) :
        org_line(org_line), labels(labels), dst(dst), src(src) {}

Line::~Line() {
    delete labels;
    delete dst;
    delete src;
}

void Line::compile() {
    //TODO: this
}
