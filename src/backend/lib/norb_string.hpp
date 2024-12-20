//
// Created by Rayzh on 2024/12/20.
//

#ifndef NORB_STRING_HPP
#define NORB_STRING_HPP

#include <array>
#include <cstring>

namespace norb {
    template <int max_len>
        class string : public std::array<char, max_len> {

    public:
        string() = default;

        string(const char* str) {
            this->fill('\0');
            if (str == nullptr) {
                return;
            }
            std::strncpy(this->data(), str, max_len);
            (*this)[max_len - 1] = '\0';
        }

        string(const std::string str) : string(str.c_str()) {}

        static string fill_string(const char character) {
            string ret;
            ret.fill(character);
            ret[max_len - 1] = '\0';
            return ret;
        }

        friend std::ostream &operator << (std::ostream &os, const string str) {
            for (int i = 0; i < str.size(); i++) {
                if (str[i] == '\0')
                    break;
                os << str[i];
            }
            return os;
        }
    };
}

#endif //NORB_STRING_HPP
