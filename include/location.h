#pragma once

//TODO: use namespace?
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

inline Location LocationFromString(const std::string& l) {
    if(l.empty()) return Location::NONE;
    auto loc = std::find(Location_Strings.begin(), Location_Strings.end(), l);

    if(loc != Location_Strings.end())
        return (Location)(loc - Location_Strings.begin());

    std::smatch match;
    const std::regex imd("(-?\\d+)");
    const std::regex pimd("(\\[-?\\d+\\])");
    if(std::regex_match(arg, match, imd))
        return Location::IMD;
    else if(std::regex_match(arg, match, pimd))
        return Location::PIMD;

    throw std::invalid_argument("Location " + l + " is not valid");
}