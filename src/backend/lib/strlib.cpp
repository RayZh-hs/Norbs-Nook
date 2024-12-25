#include "strlib.hpp"

#include <regex>

bool matchRegex(const std::string& str, const std::regex& pattern) {
    std::smatch res;
    return std::regex_match(str, res, pattern);
}

std::smatch groupRegex(const std::string& str, const std::regex& pattern) {
    try {
        std::smatch ret;
        std::regex_search(str, ret, pattern);
        return ret;
    }
    catch (...) {
        return {};
    }
}
