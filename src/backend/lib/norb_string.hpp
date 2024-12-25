#pragma once

#include <array>
#include <cstring>

#include "norb_alias.hpp"

namespace norb {
    template <int max_len>
        class string : public std::array<char, max_len> {

    public:
        string() = default;

        string(const char* str);

        string(std::string str);

        static string fill_string(char character);

        template <int N>
        friend std::ostream &operator << (std::ostream &os, string<N> str);

        template <int N>
        friend std::istream &operator >> (std::istream &is, string<N> &str);

        [[nodiscard]] std::vector<std::string> split (char separator) const;

        [[nodiscard]] std::vector<lld> split_and_hash(char separator) const;

        inline operator std::string() const {
            std::string s;
            for (auto i : *this) {
                if (i == '\0')
                    break;
                s += i;
            }
            return s;
        }
    };

    constexpr lld norb_constants_hash_offset = 0;
    constexpr lld norb_constants_hash_mul = 1007;
    constexpr lld norb_constants_hash_mod = 1E9 + 7;

    template <int len>
    lld hash(string<len> str);

    inline lld hash(const char* str);

    inline lld hash(const std::string &str);
}

#include "norb_string.tpp"