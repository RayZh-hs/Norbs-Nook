#pragma once

#include "lib/norb_string.hpp"

namespace norb {
    // A wrapper for common bounding systems (i.e. inf and -inf)
    template<typename T_>
    struct Bounds {
        static T_ neg_inf;
        static T_ pos_inf;
        static T_ minor_neg_inf;
        static T_ minor_pos_inf;
    };


    // template<int len>
    // struct Bounds<norb::string<len> > {
    //     static norb::string<len> neg_inf;
    //     static norb::string<len> pos_inf;
    //     static norb::string<len> minor_neg_inf;
    //     static norb::string<len> minor_pos_inf;
    // };
    //
    // template<>
    // struct Bounds<int> {
    //     static int neg_inf;
    //     static int pos_inf;
    //     static int minor_neg_inf;
    //     static int minor_pos_inf;
    // };
}

#include "norb_bounds.tpp"