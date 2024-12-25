#pragma once

#include <regex>

bool matchRegex(const std::string& str, const std::regex& pattern);

std::smatch groupRegex(const std::string& str, const std::regex& pattern);
