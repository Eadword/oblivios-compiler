#pragma once

enum class OPCode : uint8_t {
    #define X(val) val
    #include "opcodes"
    #undef X
};

const std::vector<std::string> OPCode_Strings {
    #define X(val) #val
    #include "opcodes"
    #undef X
};

inline std::string OPCodeToString(OPCode op) {
    return OPCode_Strings[(uint8_t)op];
}

inline OPCode OPCodeFromString(std::string op) {
    auto loc = std::find(OPCode_Strings.begin(), OPCode_Strings.end(), op);
    if(loc == OPCode_Strings.end()) throw std::invalid_argument("OPCode " + op + " is not valid");

    return (OPCode)(loc - OPCode_Strings.begin());
}