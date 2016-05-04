#pragma once

#include <map>
#include "misc.h"

namespace Preprocessor {
    void run(line_vec& lines);
    str_map getMacros(const line_vec& data);
}