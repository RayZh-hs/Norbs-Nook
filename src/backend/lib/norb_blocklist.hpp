#pragma once

#include <cassert>
#include <fstream>
#include <utility>
#include <vector>
#include <list>
#include <functional>

#include "norb_bounds.hpp"
#include "norb_string.hpp"
#include "norb_utils.hpp"

// Namespace norb for Norb.
namespace norb {
    // Classes and functions directly related to algorithms.
    namespace algo {
        int _filed_block_list_counter = 0;

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
                           const std::reference_wrapper<std::fstream> &global_reader_)
                : f_global(global_reader_) {
                assert(f_global.good());
                // Write down the names.
                f_head_name = std::move(f_head_name_);
                f_body_name = std::move(f_body_name_);
                // Open the corresponding files. Create them if they do not exist.
                utils::fAssert(f_head_name);
                utils::fAssert(f_body_name);
                f_head.open(f_head_name, file_open_mode);
                f_body.open(f_body_name, file_open_mode);
                // Load all the global data and then the head for use.
                loadGlobalData();
                loadHead();
                assert(f_head.good());
                assert(f_body.good());
                assert(f_global.good());
                if (body_count == 0) {
                    // This means that the system has not yet been initialized.
                    // Create new empty head.
                    body_count = 1;
                    head.emplace_back();
                    // Write the empty block so that later all reads don't fail.
                    FiledBlockBodyNode dummy_body;
                    dummy_body.write(f_body, 0);
                    assert(f_body.good());
                }
            }

            ~FiledBlockList() {
                // Saves all the global data and the head for the next session.
                saveHead();
                saveGlobalData();
                // Close all the fstreams.
                f_head.close();
                f_body.close();
            }

            // Inserts a new object into the list.
            void insert(const T_Key key, const T_Val val) {
                auto put_head = seekHead(key, val);
                FiledBlockBodyNode body;
                body.read(f_body, put_head->pointer);
                assert(f_body.good());
                insertIntoBody(body, key, val, put_head->len);
                ++(put_head->len);
                body.write(f_body, put_head->pointer);
                // TODO Test
                // FiledBlockBodyNode tmp;
                // tmp.read(f_body, put_head->pointer);
                if (put_head->len >= cell_break_threshold) {
                    splitCell(put_head, body);
                }
            }

            // Returns all entries through the given key, in ascending order.
            std::vector<T_Val> find(const T_Key key) {
                auto cur_head = seekHead(key, Bounds<T_Val>::minor_neg_inf);
                bool pass_on = true; // pass on to the next round
                std::vector<T_Val> ret;
                while (pass_on && cur_head != head.end()) {
                    pass_on = false;
                    bool found_in_cycle = false;
                    FiledBlockBodyNode body;
                    body.read(f_body, cur_head->pointer);
                    for (int i = 0; i < cur_head->len; i++) {
                        // if (strcmp(key, body.name[i]) == 0) {
                        if (key == body.name[i]) {
                            found_in_cycle = true;
                            ret.push_back(body.val[i]);
                        } else if (found_in_cycle) {
                            // This means that the entire span has been covered. We directly return ret.
                            return ret;
                        }
                    }
                    // If found in cycle but hasn't yet returned, we must traverse the next node.
                    ++cur_head;
                    if (cur_head != head.end()) {
                        pass_on = true;
                    }
                }
                return ret;
            }

            // Deletes a (entry, key) pair, if it exists.
            void del(const T_Key key, const T_Val val) {
                auto cur_head = seekHead(key, val);
                FiledBlockBodyNode body;
                body.read(f_body, cur_head->pointer);
                int del_pos = npos;
                for (int i = 0; i < cur_head->len; i++) {
                    // if (strcmp(body.name[i], key) == 0
                    if (body.name[i] == key && body.val[i] == val) {
                        del_pos = i;
                        break;
                    }
                }
                if (del_pos == npos) {
                    // Then nothing should be deleted.
                    return;
                }
                // Deleting the node is equivalent to moving all after it forward one step.
                for (int i = del_pos + 1; i < cur_head->len; i++) {
                    // strcpy(body.name[i - 1], body.name[i]);
                    body.name[i - 1] = body.name[i];
                    body.val[i - 1] = body.val[i];
                }
                // Update the length in head.
                --(cur_head->len);
                // Write the block back to disk.
                body.write(f_body, cur_head->pointer);
                // Conduct merging. Deprecated.
            }

        private:
            static constexpr int npos = -1;
            static constexpr int global_size = sizeof(int);

            // Structure for storing a single chain head. Acts like a node for a chained list.
            struct FiledBlockHeadNode {
                static constexpr int head_size = sizeof(T_Key) * 2 + sizeof(T_Val) * 2 + sizeof(int) * 2;

                T_Key key_min, key_max;
                T_Val key_min_val, key_max_val;
                int len = 0;
                int pointer = 0;

                FiledBlockHeadNode() {
                    // This will ensure that at the start of the system, the first default node to be filled in will contain all possible names.
                    //// memset(key_min, '\0', sizeof(key_min));
                    // memset(key_max, '~', sizeof(key_max));
                    // key_max[limit_str_len - 1] = '\0';
                    // ! Warn here ! Untested bounds system !
                    key_min     = Bounds<T_Key>::neg_inf;
                    key_max     = Bounds<T_Key>::pos_inf;
                    key_min_val = Bounds<T_Val>::neg_inf;
                    key_max_val = Bounds<T_Val>::pos_inf;
                }
            };

            typedef std::list<FiledBlockHeadNode> head_list;

            // Structure for saving a single chain body. Acts like a static array.
            struct FiledBlockBodyNode {
                static constexpr int body_size =
                        (sizeof(T_Key) * cell_break_threshold + sizeof(T_Val)) * cell_break_threshold;

                T_Key name[cell_break_threshold]{};
                T_Val val[cell_break_threshold]{};

                // Fetches a body node from the disk.
                void read(std::fstream &f, const int serial) {
                    f.seekg(getBodyOffset(serial), std::ios::beg);
                    utils::bRead(f, name);
                    utils::bRead(f, val);
                }

                // Writes the body node into the disk, overwriting old version if it exists.
                void write(std::fstream &f, const int serial) {
                    assert(f.good());
                    f.seekp(getBodyOffset(serial), std::ios::beg);
                    utils::bWrite(f, name);
                    utils::bWrite(f, val);
                    assert(f.good());
                }

                // The same write function but with an offset. Only those after offset (including itself) are written to the file.
                void write(std::fstream &f, const int serial, const int offset) {
                    // TODO Major changes in this block of code!
                    f.seekg(getBodyOffset(serial), std::ios::beg);
                    for (int i = offset; i < cell_break_threshold; i++)
                        utils::bWrite(f, name[i]);
                    // DONE does this work?
                    f.seekg(getBodyOffset(serial) + sizeof(name), std::ios::beg);
                    for (int i = offset; i < cell_break_threshold; i++)
                        utils::bWrite(f, val[i]);
                    // Something must be written at the end so that the block can be read.
                    char appendix = '\0';
                    f.seekg(getBodyOffset(serial + 1) - sizeof(char), std::ios::beg);
                    f.write(&appendix, sizeof(appendix));
                    assert(f.good());
                }
            };

            static constexpr auto file_open_mode = std::ios::binary | std::ios::in | std::ios::out;

            std::string f_head_name;
            std::string f_body_name;
            std::fstream f_head, f_body;
            std::fstream &f_global;
            int f_global_id;
            int body_count = 0;
            head_list head{};

            // Converts serial number of a body node to its offset relative to the beginning of the file.
            static int getBodyOffset(const int num) {
                return FiledBlockBodyNode::body_size * num;
            }

            // Loads the global data from the disk, if it exists.
            void loadGlobalData() {
                // Check whether the stream is empty. If so, retain the default settings.
                f_global_id = _filed_block_list_counter++;
                if (utils::fEmpty(f_global)) {
                    return;
                }
                assert(f_global.good());
                // Revised: This is used to read sequentially the global data from the file.
                utils::bRead(f_global, body_count);
                assert(f_global.good());
            }

            // Saves the global data into the disk.
            void saveGlobalData() {
                // utils::trunc(f_global, f_global_name, file_open_mode);
                f_global.seekp(f_global_id * global_size, std::ios::beg);
                // Revised: This is used to write sequentially the global data from the file.
                // Revised: This is used to ensure that the sequence of destruction does not affect the system.
                // The pointer will be stepped forward.
                utils::bWrite(f_global, body_count);
            }

            // Loads the head from the disk.
            void loadHead() {
                // Return to the beginning of the file.
                f_head.seekg(0);
                FiledBlockHeadNode head_node;
                for (int i = 0; i < body_count; i++) {
                    utils::bRead(f_head, head_node.len);
                    utils::bRead(f_head, head_node.pointer);
                    utils::bRead(f_head, head_node.key_min);
                    utils::bRead(f_head, head_node.key_max);
                    utils::bRead(f_head, head_node.key_min_val);
                    utils::bRead(f_head, head_node.key_max_val);
                    head.push_back(head_node);
                }
            }

            // Saves the head to the disk.
            void saveHead() {
                // Clear the file.
                utils::trunc(f_head, f_head_name, file_open_mode);
                // Return to the beginning of the file.
                f_head.seekg(0);
                for (auto head_node: head) {
                    utils::bWrite(f_head, head_node.len);
                    utils::bWrite(f_head, head_node.pointer);
                    utils::bWrite(f_head, head_node.key_min);
                    utils::bWrite(f_head, head_node.key_max);
                    utils::bWrite(f_head, head_node.key_min_val);
                    utils::bWrite(f_head, head_node.key_max_val);
                }
            }

            // Find the head into which the new pair should be inserted into.
            typename head_list::iterator seekHead(const T_Key key, const T_Val val) {
                for (auto iter = head.begin(); iter != head.end(); ++iter) {
                    if (utils::ascend(iter->key_min, key, iter->key_max)) {
                        // if (strcmp(iter->key_min, key) == 0 && iter->key_min_val > val) {
                        if (iter->key_min == key && iter->key_min_val > val) {
                            continue;
                        }
                        // if (strcmp(iter->key_max, key) == 0 && iter->key_max_val < val) {
                        if (iter->key_max == key && iter->key_max_val <= val) {
                            continue;
                        }
                        // Find the node that accommodates the given (key, val) pair.
                        return iter;
                    }
                }
                // This means that the corresponding pair cannot be inserted into the list.
                return head.end();
            }

            // Split a cell that has reached cell_break_threshold.
            void splitCell(const typename head_list::iterator iter, FiledBlockBodyNode &original_body_node) {
                // When a cell reaches the size of cell_break_threshold, it breaks into two separate cells.
                // A new head is created and appended to the chained list, and the latter half of the body is copied to the end of the file.
                body_count++;
                auto next = utils::insert_after(head, iter, *iter);
                // The critical element is body[cell_size]. Update the two heads accordingly.
                // strcpy(iter->key_max, original_body_node.name[cell_size]);
                // strcpy(next->key_min, original_body_node.name[cell_size]);
                iter->key_max = original_body_node.name[cell_size];
                next->key_min = original_body_node.name[cell_size];
                next->len = iter->len - cell_size;
                iter->len = cell_size;
                next->key_min_val = original_body_node.val[cell_size];
                iter->key_max_val = original_body_node.val[cell_size];
                next->pointer = body_count - 1;
                // Append the new body to the file.
                original_body_node.write(f_body, body_count - 1, cell_size);
            }

            // Insert a new entry to an existing body. Call with the original length of body as len.
            static void insertIntoBody(FiledBlockBodyNode &body, const T_Key key, const T_Val val, const int len) {
                int cur = 0;
                for (; cur < len; cur++) {
                    // if (strcmp(body.name[cur], key) > 0
                    // || (strcmp(body.name[cur], key) == 0 && body.val[cur] >= val)) {
                    if (body.name[cur] > key
                        || (body.name[cur] == key && body.val[cur] >= val)) {
                        break;
                    }
                }
                // Insert before cursor.
                for (int rear_p = len - 1; rear_p >= cur; rear_p--) {
                    // strcpy(body.name[rear_p + 1], body.name[rear_p]);
                    body.name[rear_p + 1] = body.name[rear_p];
                    body.val[rear_p + 1] = body.val[rear_p];
                }
                // strcpy(body.name[cur], key);
                body.name[cur] = key;
                body.val[cur] = val;
            }
        };

        // Explicit instantiation.
        template class FiledBlockList<norb::string<10>, int>;
        template class FiledBlockList<int, norb::string<10>>;
    }
}
