#include "location.h"
#include "parser.h"

Location LocationFromArg(const Argument* arg) {
    if(!arg || !arg->val) return Location::NONE;
    if(arg->val->isNum()) {
        if(arg->pointer) return Location::PIMD;
        return Location::IMD;
    }

    string str(*arg->val->getStr());
    if(arg->pointer) str = "[" + str + "]";

    auto loc = std::find(Location_Strings.begin(), Location_Strings.end(), str);

    if(loc != Location_Strings.end())
        return (Location)(loc - Location_Strings.begin());

    throw std::invalid_argument("Location " + str + " is not valid");
}
