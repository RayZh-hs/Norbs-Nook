#ifndef NORB_NAIVELIST_HPP
#define NORB_NAIVELIST_HPP
#include <string>
#include <exception>
#include "norb_utils.hpp"

// Namespace norb for Norb.
namespace norb {
    // Classes and functions directly related to algorithms.
    namespace algo {
        template<typename T_>
        class FiledNaiveList {
        public:
            FiledNaiveList(const std::string f_name_) {
                f_name = f_name_;
                utils::fAssert(f_name);
                f_stream.open(f_name, file_open_mode);
                assert(f_stream.good());
                f_stream.seekg(0);
                if (!utils::fEmpty(f_stream)) {
                    // Read the size from the disk.
                    utils::bRead(f_stream, size_);
                }
                else {
                    // Write a zero to the disk.
                    f_stream.seekp(0);
                    utils::bWrite(f_stream, size_);
                }
                assert(f_stream.good());
            }

            ~FiledNaiveList() {
                f_stream.close();
            }

            T_ get(const int index) {
                assert(f_stream.good());
                if (index >= size_ || index < 0) {
                    throw std::range_error("norb::FiledNaiveList: INDEX OUT OF RANGE!");
                }
                f_stream.seekg(getPos(index), std::ios::beg);
                T_ ret;
                utils::bRead(f_stream, ret);
                assert(f_stream.good());
                return ret;
            }

            T_ set(const int index, T_ to) {
                assert(f_stream.good());
                if (index < 0) {
                    throw std::range_error("norb::FiledNaiveList: INDEX OUT OF RANGE!");
                }
                if (index + 1 > size_) {
                    size_ = std::max(size_, index + 1);
                    f_stream.seekp(0);
                    utils::bWrite(f_stream, size_);
                }
                f_stream.seekp(getPos(index), std::ios::beg);
                utils::bWrite(f_stream, to);
                assert(f_stream.good());
                return to;
            }

            int size() const {
                return size_;
            }

        private:

            static constexpr auto file_open_mode = std::ios::binary | std::ios::in | std::ios::out;
            static constexpr int sizeof_t = sizeof(T_);

            int size_ = 0;
            std::string f_name;
            std::fstream f_stream;

            int getPos(const int index) const {
                return sizeof(int) + sizeof_t * index;
            }
        };
    }
}

#endif //NORB_NAIVELIST_HPP
