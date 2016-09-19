#pragma once

#include <map>
#include <regex>

class Argument;

enum class Location : uint8_t {
    #define X(name, str, dstval, srcval) name
    #include "locations"
    #undef X
};

//TODO: convert to map?
const std::vector<std::string> Location_Strings {
    #define X(name, str, dstval, srcval) str
    #include "locations"
    #undef X
};

const std::map<Location, std::pair<int8_t, int8_t>> Location_Vals {
    #define X(name, str, dstval, srcval) {Location::name, {dstval, srcval}}
    #include "locations"
    #undef X
};

inline std::string LocationToString(Location l) {
    return Location_Strings[(uint8_t)l];
}

Location LocationFromArg(const Argument* arg);
