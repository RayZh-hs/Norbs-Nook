#pragma once

#include "norb_string.hpp"

namespace norb {
    template<int max_len>
    string<max_len>::string(const char *str) {
        this->fill('\0');
        if (str == nullptr) {
            return;
        }
        std::strncpy(this->data(), str, max_len);
        (*this)[max_len - 1] = '\0';
    }

    template<int max_len>
    string<max_len>::string(const std::string str) : string(str.c_str()) {
    }

    template<int max_len>
    string<max_len> string<max_len>::fill_string(const char character) {
        string ret;
        ret.fill(character);
        ret[max_len - 1] = '\0';
        return ret;
    }

    template<int max_len>
    std::ostream &operator <<(std::ostream &os, const string<max_len> str) {
        for (int i = 0; i < str.size(); i++) {
            if (str[i] == '\0')
                break;
            os << str[i];
        }
        return os;
    }

    template<int max_len>
    std::istream &operator >>(std::istream &is, const string<max_len> &str) {
        std::string str_beta;
        is >> str_beta;
        str(str_beta);
        return is;
    }

    template<int len>
    lld hash(const string<len> str) {
        lld ans = 0;
        for (auto i: str) {
            ans = (ans * norb_constants_hash_mul + i - norb_constants_hash_offset) % norb_constants_hash_mod;
        }
        return ans;
    }

    inline lld hash(const char *str) {
        lld ans = 0;
        for (auto i = str; (*i) != '\0'; i++) {
            ans = (ans * norb_constants_hash_mul + (*i) - norb_constants_hash_offset) % norb_constants_hash_mod;
        }
        return ans;
    }

    inline lld hash(const std::string &str) {
        lld ans = 0;
        for (const auto i: str) {
            ans = (ans * norb_constants_hash_mul + i - norb_constants_hash_offset) % norb_constants_hash_mod;
        }
        return ans;
    }
}