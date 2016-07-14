#pragma once

/**
 * Some useful number interpreting/handing things
 */
namespace Numbers {
    std::string removeUscores(const std::string&);

    uint64_t readBin(const std::string&);
    uint64_t readOct(const std::string&);
    uint64_t readDec(const std::string&);
    uint64_t readHex(const std::string&);
}