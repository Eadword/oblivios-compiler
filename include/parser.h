#pragma once

#include <set>
#include "misc.h"

/**
 * An unfortunate need for the parser to keep things from being confused,
 * it keeps track of whether the value is a string or integer.
 *
 * Opted to make this a class to force adherence to intended use.
 */
class ArgVal {
    bool number; //if false then it is a string
    union {
        string*  str;
        int64_t sint;
    } val;

public:
    ArgVal() : number(true) {
        val.sint = 0;
    }

    /**
     * Performs deep copy
     */
    ArgVal(const ArgVal& av) : number(av.number) {
        if(number) val.sint = av.val.sint;
        else val.str = new string(*(av.val.str));
    }

    ArgVal(int64_t i) : number(true) {
        val.sint = i;
    }

    /**
     * @note makes a copy of the cstring
     */
    ArgVal(char* c) : number(false) {
        val.str = new string(c);
    }

    /**
     * @note does not make a copy of the string
     */
    ArgVal(string* s) : number(false) {
        val.str = s;
    }

    ~ArgVal() { if(!number) delete(val.str); }

    bool isNum() { return number; }
    const string* getStr() { return !number ? val.str : nullptr; }
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
