#pragma once

enum class OPCode : uint8_t {
    #define X(val, b) val
    #include "opcodes"
    #undef X
};

//TODO: use a set or map
const std::vector<std::string> OPCode_Strings {
    #define X(val, b) #val
    #include "opcodes"
    #undef X
};

const std::vector<uint8_t> OPCode_NumParams {
    #define X(b, val) val
    #include "opcodes"
    #undef X
};

inline std::string OPCodeToString(OPCode op) {
    return OPCode_Strings[(uint8_t)op];
}

inline OPCode OPCodeFromString(const std::string& op) {
    auto loc = std::find(OPCode_Strings.begin(), OPCode_Strings.end(), op);
    if(loc == OPCode_Strings.end()) throw std::invalid_argument("OPCode " + op + " is not valid");

    return (OPCode)(loc - OPCode_Strings.begin());
}

inline uint8_t getOPCodeParams(OPCode op) { return OPCode_NumParams[(uint8_t)op]; }
