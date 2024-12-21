#ifndef NORB_BOUNDS_HPP
#define NORB_BOUNDS_HPP

#include <limits>
#include "lib/norb_string.hpp"
#include "lib/norb_alias.hpp"

namespace norb {
    // A wrapper for common bounding systems (i.e. inf and -inf)
    template<typename T_>
    struct Bounds {
        static T_ neg_inf;
        static T_ pos_inf;
        static T_ minor_neg_inf;
        static T_ minor_pos_inf;
    };

    template<int len>
    struct Bounds<norb::string<len> > {
        static norb::string<len> neg_inf;
        static norb::string<len> pos_inf;
        static norb::string<len> minor_neg_inf;
        static norb::string<len> minor_pos_inf;
    };

    template<int len>
    norb::string<len> Bounds<norb::string<len> >::neg_inf = norb::string<len>::fill_string('\0');
    template<int len>
    norb::string<len> Bounds<norb::string<len> >::pos_inf = norb::string<len>::fill_string('~');
    template<int len>
    norb::string<len> Bounds<norb::string<len> >::minor_neg_inf = norb::string<len>::fill_string('!');
    template<int len>
    norb::string<len> Bounds<norb::string<len> >::minor_pos_inf = norb::string<len>::fill_string('}');

    template <>
    struct Bounds<int> {
        static int neg_inf;
        static int pos_inf;
        static int minor_neg_inf;
        static int minor_pos_inf;
    };

    int Bounds<int>::neg_inf = std::numeric_limits<int>::lowest();
    int Bounds<int>::pos_inf = std::numeric_limits<int>::max();
    int Bounds<int>::minor_neg_inf = std::numeric_limits<int>::lowest() + 1;
    int Bounds<int>::minor_pos_inf = std::numeric_limits<int>::max() - 1;

}

#endif //NORB_BOUNDS_HPP
