#pragma once

#include "norb_utils.hpp"
#include <cassert>
#include <cstring>
#include <set>

namespace norb {
    // Extensions to std, stl and custom auxiliary functions reside in `utils`.
    namespace utils {
        // Check whether a fstream is empty.
        inline bool fEmpty(std::fstream &f) {
            const std::streampos pos = f.tellg();
            // Change Note: using seek(0) on an empty file will disrupt the fstream f.
            f.seekg(0, std::ios::beg);
            const auto begin = f.tellg();
            f.seekg(0, std::ios::end);
            const auto end = f.tellg();
            f.seekg(pos);
            assert(f.good());
            return begin == end;
        }

        // Create file when it doesn't exist.
        // ! Can only be called when no other fstreams are pointing to that file !
        inline void fAssert(const std::string &path) {
            std::fstream f;
            f.open(path, std::ios::app);
            f.close();
        }

        // Binary read from a file.
        template<typename T_>
        void bRead(std::fstream &f, T_ &item) {
            f.read(reinterpret_cast<char *>(&item), sizeof(item));
        }

        // Binary write into a file.
        template<typename T_>
        void bWrite(std::fstream &f, T_ &item) {
            f.write(reinterpret_cast<char *>(&item), sizeof(item));
        }

        // Binary write into a file.
        template<typename T_>
        void bWrite(std::fstream &f, T_ &item, const int size_) {
            f.write(reinterpret_cast<char *>(&item), size_);
        }

        // Clear all texts within a stream and reopen it.
        inline void trunc(std::fstream &f, const std::string &file_name, const std::_Ios_Openmode mode) {
            f.close();
            remove(file_name.c_str());
            fAssert(file_name);
            f.open(file_name, mode);
        }

        // Returns whether the three given variables are in ascending order, non-strictly.
        template<typename T_>
        bool ascend(T_ begin, T_ val, T_ end) {
            return begin <= val && val <= end;
        }

        inline bool ascend(const char *begin, const char *val, const char *end) {
            return strcmp(begin, val) <= 0 && strcmp(val, end) <= 0;
        }

        // Inserts object after a node (if it is not the last node).
        template<typename T_>
        typename std::list<T_>::iterator insert_after(std::list<T_> &list, typename std::list<T_>::iterator pos,
                                                      T_ object) {
            if (pos != list.end()) {
                ++pos;
            }
            return list.insert(pos, object);
        }

        // Allows std::vectors to be printed via std::ostream.
        template<typename T_>
        std::ostream &operator <<(std::ostream &os, std::vector<T_> list) {
            for (int i = 0; i < list.size(); i++) {
                if (i != 0)
                    os << ' ';
                os << list[i];
            }
            return os;
        }

        // The time complexity is $O(n^2)$
        template<typename T_>
        bool isSubsetOf(std::vector<T_> a, std::vector<T_> b) {
            for (auto i : a) {
                bool identified = false;
                for (auto j : b) {
                    if (i == j) {
                        identified = true;
                        break;
                    }
                }
                if (!identified)
                    return false;
            }
            return true;
        }

        // The time complexity is $O(n\log{n})$
        template<typename T_>
        bool isSubsetOf(std::set<T_> a, std::set<T_> b) {
            for (auto i: a) {
                if (!b.count(i))
                    return false;
            }
            return true;
        }
    }
}
