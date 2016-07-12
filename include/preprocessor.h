#pragma once

#include <map>
#include "misc.h"

namespace Preprocessor {
    void run(line_vec& lines);
    void removeComments(line_vec& lines);
    void removeWhiteSpace(line_vec& lines);
    str_map getMacros(line_vec& lines);
    void replaceMacros(line_vec& lines, const str_map& macros);
    label_map getLabels(line_vec& lines);
    void replaceLabels(line_vec& lines, const label_map& labels);
    void formatNumbers(line_vec& lines);
    //TODO: recognize and evaluate expressions
}