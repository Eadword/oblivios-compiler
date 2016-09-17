#pragma once

#include <ostream>
#include <set>

#include "line.h"
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
        int64_t num;
    } val;

    friend std::ostream& operator<<(std::ostream& out, const ArgVal& av);

public:
    ArgVal() : number(true) {
        val.num = 0;
    }

    /**
     * Performs deep copy
     */
    ArgVal(const ArgVal& av) : number(av.number) {
        if(number) val.num = av.val.num;
        else val.str = new string(*(av.val.str));
    }

    ArgVal(int64_t i) : number(true) {
        val.num = i;
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

    bool isNum() const { return number; }
    const string* getStr() const { return !number ? val.str : nullptr; }
    int64_t getNum() const { return number ? val.num : 0; }
};

/// For debugging
inline std::ostream& operator<<(std::ostream& out, const ArgVal& av) {
    if(av.number) return out << av.val.num;
    return out << *av.val.str;
}

struct Argument {
    AccessMode mode;
    ArgVal* val;
    bool pointer;

    Argument(AccessMode m = AccessMode::DIRECT, ArgVal* v = nullptr, bool p = false) :
            mode(m), val(v), pointer(p) {}

    ~Argument() { delete val; }
};

/// For debugging
inline std::ostream& operator<<(std::ostream& out, const Argument& arg) {
    if(arg.mode == AccessMode::RELATIVE) out << "%";
    if(arg.pointer) return out << "[" << *arg.val << "]";
    return out << *arg.val;
}
