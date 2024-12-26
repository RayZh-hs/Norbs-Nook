#pragma once

#include <regex>

bool matchRegex(const std::string& str, const std::regex& pattern);

std::smatch groupRegex(const std::string& str, const std::regex& pattern);

//If keys overlap, then an empty map will be returned.
std::map<std::string, std::string> extractUniqueKeyValuePairs(const std::string& text);