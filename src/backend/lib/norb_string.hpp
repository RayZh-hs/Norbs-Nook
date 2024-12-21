//
// Created by Rayzh on 2024/12/20.
//

#ifndef NORB_STRING_HPP
#define NORB_STRING_HPP

#include <array>
#include <cstring>

#include "constants.hpp"
#include "norb_blocklist.hpp"
#include "norb_alias.hpp"

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

        friend std::istream &operator >> (std::istream &is, const string &str) {
            std::string str_beta;
            is >> str_beta;
            str(str_beta);
            return is;
        }
    };

    constexpr lld norb_constants_hash_offset = 0;
    constexpr lld norb_constants_hash_mul = 1007;
    constexpr lld norb_constants_hash_mod = 1E9 + 7;

    template <int len>
    lld hash(const string<len> str) {
        lld ans = 0;
        for (auto i : str) {
            ans = (ans * norb_constants_hash_mul + i - norb_constants_hash_offset) % norb_constants_hash_mod;
        }
        return ans;
    }

    inline lld hash(const char* str) {
        lld ans = 0;
        for (auto i = str; (*i) != '\0'; i++) {
            ans = (ans * norb_constants_hash_mul + (*i) - norb_constants_hash_offset) % norb_constants_hash_mod;
        }
        return ans;
    }

    inline lld hash(const std::string &str) {
        lld ans = 0;
        for (const auto i : str) {
            ans = (ans * norb_constants_hash_mul + i - norb_constants_hash_offset) % norb_constants_hash_mod;
        }
        return ans;
    }
}

#endif //NORB_STRING_HPP
