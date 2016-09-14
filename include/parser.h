#pragma once

#include <set>
#include "misc.h"

/**
 * An unfortunate need for the parser to keep things from being confused,
 * it keeps track of whether the value is a string or integer.
 *
 * Opted to make this a class to force adherence to intended use.
 *
 * @note Takes ownership of the cstring
 */
class ArgVal {
    bool number; //if false then it is a string
    union {
        char*  cstring;
        int64_t sint;
    } val;

public:
    ArgVal() {
        number = true;
        val.sint = 0;
    }

    ArgVal(int64_t i) {
        number = true;
        val.sint = i;
    }

    ArgVal(char* c) {
        number = false;
        val.cstring = c;
    }

    ~ArgVal() { if(!number) free(val.cstring); }

    bool isNumber() { return number; }
    const char* getString() { return !number ? val.cstring : nullptr; }
    int64_t getSInt() { return number? val.sint : 0; }
};

struct Argument {
    AccessMode mode;
    ArgVal* val;
    bool pointer;

    Argument(AccessMode m = AccessMode::DIRECT, ArgVal* v = nullptr, bool p = false) :
            mode(m), val(v), pointer(p) {}

    ~Argument() { delete val; }
};

typedef std::set<string> Labels;
