#pragma once

#include <cassert>

#include "norb_blocklist.hpp"
#include "norb_utils.hpp"

// Namespace norb for Norb.
namespace norb {
    // Classes and functions directly related to algorithms.
    namespace algo {
        // int _filed_block_list_counter = 0;

        // A Block List implementation based on disk allocation. All operations are $O(\sqrt{n})$.
        // template<typename T_Key, typename T_Val, const Bounds<T_Key> &key_bounds_, const Bounds<T_Val> &val_bounds_>
        // In this revised version, the global_reader_ fstream is externally managed.
        template<typename T_Key, typename T_Val>
        FiledBlockList<T_Key, T_Val>::FiledBlockList(std::string f_head_name_, std::string f_body_name_,
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

        template<typename T_Key, typename T_Val>
        FiledBlockList<T_Key, T_Val>::~FiledBlockList() {
            // Saves all the global data and the head for the next session.
            saveHead();
            saveGlobalData();
            // Close all the fstreams.
            f_head.close();
            f_body.close();
        }

        // Inserts a new object into the list.
        template<typename T_Key, typename T_Val>
        void FiledBlockList<T_Key, T_Val>::insert(const T_Key key, const T_Val val) {
            item_count++;
            auto put_head = seekHead(key, val);
            FiledBlockBodyNode body;
            body.read(f_body, put_head->pointer);
            assert(f_body.good());
            insertIntoBody(body, key, val, put_head->len);
            ++(put_head->len);
            body.write(f_body, put_head->pointer);
            // DONE Test
            // FiledBlockBodyNode tmp;
            // tmp.read(f_body, put_head->pointer);
            if (put_head->len >= cell_break_threshold) {
                splitCell(put_head, body);
            }
        }

        // Returns all entries through the given key, in ascending order.
        template<typename T_Key, typename T_Val>
        std::vector<T_Val> FiledBlockList<T_Key, T_Val>::find(const T_Key key) {
            auto cur_head = seekHead(key);
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
        template<typename T_Key, typename T_Val>
        void FiledBlockList<T_Key, T_Val>::del(const T_Key key, const T_Val val) {
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
            item_count--;
            // Write the block back to disk.
            body.write(f_body, cur_head->pointer);
            // Conduct merging. Deprecated.
        }

        // Returns whether a list is empty.
        template<typename T_Key, typename T_Val>
        bool FiledBlockList<T_Key, T_Val>::empty() const {
            return item_count == 0;
        }

        // Returns the number of elements within the list.
        template<typename T_Key, typename T_Val>
        int FiledBlockList<T_Key, T_Val>::size() const {
            return item_count;
        }

        // Counts the number of occurrences for a key.
        template<typename T_Key, typename T_Val>
        int FiledBlockList<T_Key, T_Val>::count(const T_Key key) {
            return find(key).size();
        }

        // Counts the number of occurrences for a key-val pair.
        // DONE Needs testing.
        template<typename T_Key, typename T_Val>
        int FiledBlockList<T_Key, T_Val>::count(const T_Key key, const T_Val val) {
            auto cur_head = seekHead(key);
            bool pass_on = true; // pass on to the next round
            int ret = 0;
            while (pass_on && cur_head != head.end()) {
                pass_on = false;
                bool found_in_cycle = false;
                FiledBlockBodyNode body;
                body.read(f_body, cur_head->pointer);
                for (int i = 0; i < cur_head->len; i++) {
                    // if (strcmp(key, body.name[i]) == 0) {
                    if (key == body.name[i]) {
                        found_in_cycle = true;
                        if (val == body.val[i])
                            ret++;
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

        template<typename T_Key, typename T_Val>
        std::vector<T_Val> FiledBlockList<T_Key, T_Val>::valAsVector() {
            std::vector<T_Val> ret;
            for (auto h : head) {
                FiledBlockBodyNode body;
                body.read(f_body, h.pointer);
                for (int i = 0; i < h.len; i++) {
                    ret.push_back(body.val[i]);
                }
            }
            return ret;
        }

        template<typename T_Key, typename T_Val>
        T_Val FiledBlockList<T_Key, T_Val>::findFirst(T_Key key) {
            auto cur_head = seekHead(key);

            while (cur_head != head.end()) {
                FiledBlockBodyNode body;
                body.read(f_body, cur_head->pointer);
                for (int i = 0; i < cur_head->len; i++) {
                    if (key == body.name[i]) {
                        return body.val[i];
                    }
                }
                // If found in cycle but hasn't yet returned, we must traverse the next node.
                ++cur_head;
                if (cur_head->key_min > key)
                    break;
            }
            return {};
        }


        // template<typename T_Key, typename T_Val>
        //         FiledBlockList<T_Key, T_Val>::

        // Structure for storing a single chain head. Acts like a node for a chained list.

        template<typename T_Key, typename T_Val>
        FiledBlockList<T_Key, T_Val>::FiledBlockHeadNode::FiledBlockHeadNode() {
            // This will ensure that at the start of the system, the first default node to be filled in will contain all possible names.
            //// memset(key_min, '\0', sizeof(key_min));
            // memset(key_max, '~', sizeof(key_max));
            // key_max[limit_str_len - 1] = '\0';
            key_min     = {};
            key_max     = {};
            key_min_val = {};
            key_max_val = {};
        }

        // Fetches a body node from the disk.
        template<typename T_Key, typename T_Val>
        void FiledBlockList<T_Key, T_Val>::FiledBlockBodyNode::read(std::fstream &f, const int serial) {
            f.seekg(getBodyOffset(serial), std::ios::beg);
            utils::bRead(f, name);
            utils::bRead(f, val);
        }

        template<typename T_Key, typename T_Val>
        void FiledBlockList<T_Key, T_Val>::FiledBlockBodyNode::write(std::fstream &f, const int serial) {
            assert(f.good());
            f.seekp(getBodyOffset(serial), std::ios::beg);
            utils::bWrite(f, name);
            utils::bWrite(f, val);
            assert(f.good());
        }

        template<typename T_Key, typename T_Val>
        void FiledBlockList<T_Key, T_Val>::FiledBlockBodyNode::write(std::fstream &f, const int serial, const int offset) {
            // DONE Major changes in this block of code!
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

        // Converts serial number of a body node to its offset relative to the beginning of the file.
        template<typename T_Key, typename T_Val>
        int FiledBlockList<T_Key, T_Val>::getBodyOffset(const int num) {
            return FiledBlockBodyNode::body_size * num;
        }

        // Loads the global data from the disk, if it exists.
        template<typename T_Key, typename T_Val>
        void FiledBlockList<T_Key, T_Val>::loadGlobalData() {
            // Check whether the stream is empty. If so, retain the default settings.
            f_global_id = _filed_block_list_counter++;
            if (utils::fEmpty(f_global)) {
                return;
            }
            assert(f_global.good());
            // Revised: This is used to read sequentially the global data from the file.
            utils::bRead(f_global, body_count);
            utils::bRead(f_global, item_count);
            assert(f_global.good());
        }

        // Saves the global data into the disk.
        template<typename T_Key, typename T_Val>
        void FiledBlockList<T_Key, T_Val>::saveGlobalData() {
            // utils::trunc(f_global, f_global_name, file_open_mode);
            f_global.seekp(f_global_id * global_size, std::ios::beg);
            // Revised: This is used to write sequentially the global data from the file.
            // Revised: This is used to ensure that the sequence of destruction does not affect the system.
            // The pointer will be stepped forward.
            utils::bWrite(f_global, body_count);
            utils::bWrite(f_global, item_count);
        }

        // Loads the head from the disk.
        template<typename T_Key, typename T_Val>
        void FiledBlockList<T_Key, T_Val>::loadHead() {
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
        template<typename T_Key, typename T_Val>
        void FiledBlockList<T_Key, T_Val>::saveHead() {
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
        template<typename T_Key, typename T_Val>
        typename FiledBlockList<T_Key, T_Val>::head_list::iterator FiledBlockList<T_Key, T_Val>::seekHead(const T_Key key, const T_Val val) {
            for (auto iter = head.begin(); iter != head.end(); ++iter) {
                auto iter_cp = iter;
                const bool within_left  = (iter == head.begin())    || (iter->key_min < key || (iter->key_min == key && iter->key_min_val <= val));
                const bool within_right = (++iter_cp == head.end()) || (iter->key_max > key || (iter->key_max == key && iter->key_max_val >  val));
                if (within_left && within_right) {
                    return iter;
                }
            }
            // This means that the corresponding pair cannot be inserted into the list.
            return head.end();
        }

        // Find the head with the earliest appearance of key, or where key should be inserted if it does not exist.
        template<typename T_Key, typename T_Val>
        typename FiledBlockList<T_Key, T_Val>::head_list::iterator FiledBlockList<T_Key, T_Val>::seekHead(const T_Key key) {
            for (auto iter = head.begin(); iter != head.end(); ++iter) {
                auto iter_cp = iter;
                const bool within_left  = (iter == head.begin())    || (iter->key_min <= key);
                const bool within_right = (++iter_cp == head.end()) || (iter->key_max >= key);
                if (within_left && within_right) {
                    return iter;
                }
            }
            // This means that the corresponding pair cannot be inserted into the list.
            return head.end();
        }

        // Split a cell that has reached cell_break_threshold.
        template<typename T_Key, typename T_Val>
        void FiledBlockList<T_Key, T_Val>::splitCell(const typename head_list::iterator iter, FiledBlockBodyNode &original_body_node) {
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
        template<typename T_Key, typename T_Val>
        void FiledBlockList<T_Key, T_Val>::insertIntoBody(FiledBlockBodyNode &body, const T_Key key, const T_Val val, const int len) {
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
    }
}

