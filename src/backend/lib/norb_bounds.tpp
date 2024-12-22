#pragma once

#include <limits>
#include "norb_bounds.hpp"

namespace norb {
    template<int len>
    struct Bounds<norb::string<len> > {
        static norb::string<len> neg_inf;
        static norb::string<len> pos_inf;
        static norb::string<len> minor_neg_inf;
        static norb::string<len> minor_pos_inf;
    };

    template<>
    struct Bounds<int> {
        static int neg_inf;
        static int pos_inf;
        static int minor_neg_inf;
        static int minor_pos_inf;
    };

    template<>
    struct Bounds<lld> {
        static lld neg_inf;
        static lld pos_inf;
        static lld minor_neg_inf;
        static lld minor_pos_inf;
    };

    template<int len>
    norb::string<len> Bounds<norb::string<len> >::neg_inf = norb::string<len>::fill_string('\0');
    template<int len>
    norb::string<len> Bounds<norb::string<len> >::pos_inf = norb::string<len>::fill_string('~');
    template<int len>
    norb::string<len> Bounds<norb::string<len> >::minor_neg_inf = norb::string<len>::fill_string('!');
    template<int len>
    norb::string<len> Bounds<norb::string<len> >::minor_pos_inf = norb::string<len>::fill_string('}');

    int Bounds<int>::neg_inf = std::numeric_limits<int>::lowest();
    int Bounds<int>::pos_inf = std::numeric_limits<int>::max();
    int Bounds<int>::minor_neg_inf = std::numeric_limits<int>::lowest() + 1;
    int Bounds<int>::minor_pos_inf = std::numeric_limits<int>::max() - 1;

    lld Bounds<lld>::neg_inf = std::numeric_limits<lld>::lowest();
    lld Bounds<lld>::pos_inf = std::numeric_limits<lld>::max();
    lld Bounds<lld>::minor_neg_inf = std::numeric_limits<lld>::lowest() + 1;
    lld Bounds<lld>::minor_pos_inf = std::numeric_limits<lld>::max() - 1;
}
