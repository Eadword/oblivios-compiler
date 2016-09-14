#pragma once
#include "misc.h"

/**
 * An unfortunate need for the parser to keep things from being confused,
 * it keeps track of whether the value is a string or integer.
 *
 * @note Takes ownership of the string
 */
struct ArgVal {
    bool number; //if false then it is a string
    union {
        char*  cstring;
        int64_t sint;
    } val;

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
};
