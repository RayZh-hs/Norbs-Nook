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

std::map<std::string, std::string> extractUniqueKeyValuePairs(const std::string& text) {

    std::map<std::string, std::string> keyValuePairs;
    const std::regex modifyRegex(R"(^\s*((?:-\S+=\S+\s*)+)\s*$)");
    std::smatch modifyMatch;

    if (std::regex_search(text, modifyMatch, modifyRegex)) {
        std::string keyValueString = modifyMatch[1].str();
        const std::regex keyValueRegex(R"(-(\S+)=(\S+))");
        std::smatch keyValueMatch;

        auto words_begin = std::sregex_iterator(keyValueString.begin(), keyValueString.end(), keyValueRegex);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            keyValueMatch = *i;
            if (keyValuePairs.count(keyValueMatch[1].str())) {
                // Then there are overlapping keywords!
                return {};
            }
            keyValuePairs[keyValueMatch[1].str()] = keyValueMatch[2].str();
        }
    }

    return keyValuePairs; // Returns an empty map if validation fails
}