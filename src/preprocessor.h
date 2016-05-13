#pragma once

#include <map>
#include "misc.h"

namespace Preprocessor {
    void run(line_vec& lines);
    void removeComments(line_vec& lines);
    void removeWhiteSpace(line_vec& lines);
    str_map getMacros(line_vec& lines);
    void replaceMacros(line_vec& lines, str_map& macros);
    std::map<string, uint16_t> getLabels(line_vec& lines);
    void replaceLabels(line_vec& lines, std::map<string, uint16_t>& labels);
    //TODO: recognize and evaluate expressions
}