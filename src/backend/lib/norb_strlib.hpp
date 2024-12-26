#pragma once

#include <regex>

namespace norb {
    // Checks whether a string matches a regex pattern.
    bool matchRegex(const std::string& str, const std::regex& pattern);

    // Extracts the groups using a regex from a string.
    std::smatch groupRegex(const std::string& str, const std::regex& pattern);

    //If keys overlap, then an empty map will be returned.
    std::map<std::string, std::string> extractUniqueKeyValuePairs(const std::string& text);
}
