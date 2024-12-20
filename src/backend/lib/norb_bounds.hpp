#ifndef NORB_BOUNDS_HPP
#define NORB_BOUNDS_HPP

#include <limits>
#include "lib/norb_string.hpp"
#include "lib/norb_alias.hpp"

namespace norb {
    // A wrapper for common bounding systems (i.e. inf and -inf)
    template <typename T_>
    struct Bounds {
        T_ neg_inf;
        T_ pos_inf;
        T_ minor_neg_inf;
        T_ minor_pos_inf;
    };

    // Bounds for commonly used objects
    namespace bounds {
        Bounds<int> int_bounds {std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), std::numeric_limits<int>::min() / 2, std::numeric_limits<int>::max() / 2};
        Bounds<lld> lld_bounds {std::numeric_limits<lld>::min(), std::numeric_limits<lld>::max(), std::numeric_limits<lld>::min() / 2, std::numeric_limits<lld>::max() / 2};
        // The bounds for norb::string is generated dynamically according to the length of the string.
        template <int len>
        Bounds<norb::string<len>> str_bounds {norb::string<len>::fill_string('\0'), norb::string<len>::fill_string('~'), norb::string<len>::fill_string('!'), norb::string<len>::fill_string('}')};
    }

}

#endif //NORB_BOUNDS_HPP
