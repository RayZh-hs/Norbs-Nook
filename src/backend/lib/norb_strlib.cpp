#include "norb_strlib.hpp"

#include <iomanip>
#include <regex>

int norb::stringToInteger(const std::string &str) {
    std::istringstream stream(str);
    int value;
    stream >> value;
    if (!stream.eof()) stream >> std::ws;
    if (stream.fail() || !stream.eof()) {
        throw std::runtime_error("norb_strlib: INVALID INTEGER");
    }
    return value;
}

double norb::stringToDouble(const std::string &str) {
    std::istringstream stream(str);
    double value;
    stream >> value;
    if (!stream.eof()) stream >> std::ws;
    if (stream.fail() || !stream.eof()) {
        throw std::runtime_error("norb_strlib: INVALID DOUBLE");
    }
    return value;
}

std::string norb::integerToString(const int &value) {
    std::ostringstream stream;
    stream << value;
    if (stream.fail()) {
        throw std::runtime_error("norb_strlib: INVALID VALUE");
    }
    return stream.str();
}

std::string norb::doubleToString(const double &value, const int precision) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision);
    stream << value;
    if (stream.fail()) {
        throw std::runtime_error("norb_strlib: INVALID VALUE");
    }
    return stream.str();
}

// "abc"
// abcc"

bool norb::unquote(std::string &s) {
    const std::size_t siz = s.size();
    if (s[0] == '\"' && s[siz - 1] == '\"') {
        for (int i = 0; i < siz - 2; i++)
            s[i] = s[i + 1];
        s[siz - 2] = s[siz - 1] = '\0';
        return true;
    }
    return false;
}


bool norb::matchRegex(const std::string& str, const std::regex& pattern) {
    std::smatch res;
    return std::regex_match(str, res, pattern);
}

std::smatch norb::groupRegex(const std::string& str, const std::regex& pattern) {
    try {
        std::smatch ret;
        std::regex_search(str, ret, pattern);
        return ret;
    }
    catch (...) {
        return {};
    }
}

std::map<std::string, std::string> norb::extractUniqueKeyValuePairs(const std::string& text) {

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