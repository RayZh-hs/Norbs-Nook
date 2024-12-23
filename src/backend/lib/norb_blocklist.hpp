#pragma once

#include <fstream>
#include <utility>
#include <vector>
#include <list>
#include <functional>

// #include "norb_string.hpp"
// #include "norb_utils.hpp"

// Namespace norb for Norb.
namespace norb {
    // Classes and functions directly related to algorithms.
    namespace algo {
        extern int _filed_block_list_counter;

        // A Block List implementation based on disk allocation. All operations are $O(\sqrt{n})$.
        // template<typename T_Key, typename T_Val, const Bounds<T_Key> &key_bounds_, const Bounds<T_Val> &val_bounds_>
        template<typename T_Key, typename T_Val>
        class FiledBlockList {
        public:
            // static constexpr int cell_size = 320;
            static constexpr int cell_size = 2;
            static constexpr int cell_break_threshold = cell_size * 2;

            // In this revised version, the global_reader_ fstream is externally managed.
            FiledBlockList(std::string f_head_name_, std::string f_body_name_,
                           const std::reference_wrapper<std::fstream> &global_reader_);

            ~FiledBlockList();

            // Inserts a new object into the list.
            void insert(T_Key key, T_Val val);

            // Returns all entries through the given key, in ascending order.
            std::vector<T_Val> find(T_Key key);

            // Deletes a (entry, key) pair, if it exists.
            void del(T_Key key, T_Val val);

            // Returns whether a list is empty.
            bool empty() const;

            // Returns the number of elements within the list.
            int size() const;

            // Counts the number of occurrences for a key.
            int count(T_Key key);

            // Counts the number of occurrences for a key-val pair.
            // DONE Needs testing.
            int count(T_Key key, T_Val val);

            std::vector<T_Val> valAsVector();

        private:
            static constexpr int npos = -1;
            static constexpr int global_size = sizeof(int) * 2;

            // Structure for storing a single chain head. Acts like a node for a chained list.
            struct FiledBlockHeadNode {
                static constexpr int head_size = sizeof(T_Key) * 2 + sizeof(T_Val) * 2 + sizeof(int) * 2;

                T_Key key_min, key_max;
                T_Val key_min_val, key_max_val;
                int len = 0;
                int pointer = 0;

                FiledBlockHeadNode();
            };

            typedef std::list<FiledBlockHeadNode> head_list;

            // Structure for saving a single chain body. Acts like a static array.
            struct FiledBlockBodyNode {
                static constexpr int body_size =
                        (sizeof(T_Key) * cell_break_threshold + sizeof(T_Val)) * cell_break_threshold;

                T_Key name[cell_break_threshold]{};
                T_Val val[cell_break_threshold]{};

                // Fetches a body node from the disk.
                void read(std::fstream &f, int serial);

                // Writes the body node into the disk, overwriting old version if it exists.
                void write(std::fstream &f, int serial);

                // The same write function but with an offset. Only those after offset (including itself) are written to the file.
                void write(std::fstream &f, int serial, int offset);
            };

            static constexpr auto file_open_mode = std::ios::binary | std::ios::in | std::ios::out;

            std::string f_head_name;
            std::string f_body_name;
            std::fstream f_head, f_body;
            std::fstream &f_global;
            int f_global_id;
            int body_count = 0;
            int item_count = 0;
            head_list head{};

            // Converts serial number of a body node to its offset relative to the beginning of the file.
            static int getBodyOffset(int num);

            // Loads the global data from the disk, if it exists.
            void loadGlobalData();

            // Saves the global data into the disk.
            void saveGlobalData();

            // Loads the head from the disk.
            void loadHead();

            // Saves the head to the disk.
            void saveHead();

            // Find the head into which the new pair should be inserted into.
            typename head_list::iterator seekHead(T_Key key, T_Val val);

            // Find the head with the earliest appearance of key, or where key should be inserted if it does not exist.
            typename head_list::iterator seekHead(T_Key key);

            // Split a cell that has reached cell_break_threshold.
            void splitCell(typename head_list::iterator iter, FiledBlockBodyNode &original_body_node);

            // Insert a new entry to an existing body. Call with the original length of body as len.
            static void insertIntoBody(FiledBlockBodyNode &body, T_Key key, T_Val val, int len);
        };

        // Explicit instantiation.
        // template class FiledBlockList<norb::string<10>, int>;
        // template class FiledBlockList<int, norb::string<10>>;
    }
}

#include "norb_blocklist.tpp"