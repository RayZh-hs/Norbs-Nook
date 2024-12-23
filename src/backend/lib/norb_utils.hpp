#pragma once

#include <fstream>
#include <list>
#include <vector>
#include <set>

namespace norb {
    // Extensions to std, stl and custom auxiliary functions reside in `utils`.
    namespace utils {
        // Check whether a fstream is empty.
        bool fEmpty(std::fstream &f);

        // Create file when it doesn't exist.
        // ! Can only be called when no other fstreams are pointing to that file !
        void fAssert(const std::string &path);

        // Binary read from a file.
        template<typename T_>
        void bRead(std::fstream &f, T_ &item);

        // Binary write into a file.
        template<typename T_>
        void bWrite(std::fstream &f, T_ &item);

        // Binary write into a file.
        template<typename T_>
        void bWrite(std::fstream &f, T_ &item, int size_);

        // Clear all texts within a stream and reopen it.
        void trunc(std::fstream &f, const std::string &file_name, std::_Ios_Openmode mode);

        // Returns whether the three given variables are in ascending order, non-strictly.
        template<typename T_>
        bool ascend(T_ begin, T_ val, T_ end);

        bool ascend(const char *begin, const char *val, const char *end);

        // Inserts object after a node (if it is not the last node).
        template<typename T_>
        typename std::list<T_>::iterator insert_after(std::list<T_> &list, typename std::list<T_>::iterator pos,
                                                      T_ object);

        // Allows std::vectors to be printed via std::ostream.
        template<typename T_>
        std::ostream &operator <<(std::ostream &os, std::vector<T_> list);

        template<typename T_>
        bool isSubsetOf(std::vector<T_> a, std::vector<T_> b);

        template<typename T_>
        bool isSubsetOf(std::set<T_> a, std::set<T_> b);
    }
}

#include "norb_utils.tpp"