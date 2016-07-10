#pragma once

enum class Location : uint8_t {
#define X(val) val
#include "locations"
#undef X
};

const std::vector<std::string> Location_Strings {
#define X(val) #val
#include "locations"
#undef X
};

inline std::string LocationToString(Location l) {
    return Location_Strings[(uint8_t)l];
}

inline Location LocationFromString(const std::string& l) {
    auto loc = std::find(Location_Strings.begin(), Location_Strings.end(), l);
    if(loc == Location_Strings.end()) throw std::invalid_argument("Location " + l + " is not valid");

    return (Location)(loc - Location_Strings.begin());
}