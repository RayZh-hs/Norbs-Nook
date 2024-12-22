#include <iostream>
#include <fstream>

#include "norb_blocklist.hpp"
#include "norb_utils.hpp"
#include "norb_string.hpp"

int main() {
    std::fstream f_global;
    norb::utils::fAssert("global.testdata");
    f_global.open("global.testdata", std::ios::in | std::ios::out | std::ios::binary);
    f_global.seekg(0);
    norb::algo::FiledBlockList<int, norb::string<10>> block_list_1("head1.testdata", "body1.testdata", f_global);
    norb::algo::FiledBlockList<int, int> block_list_2("head2.testdata", "body2.testdata", f_global);
    // block_list_1.insert(1, norb::string<10>("1"));
    std::cout << block_list_1.size() << '\n';
    // block_list_1.insert(1, norb::string<10>("2"));
    // block_list_1.insert(1, norb::string<10>("3"));
    // block_list_1.insert(5, norb::string<10>("1"));
    // block_list_1.insert(1, norb::string<10>("4"));
    // block_list_1.insert(1, norb::string<10>("5"));
    // block_list_1.insert(1, norb::string<10>("6"));
    // block_list_2.insert(0, 2);
    // block_list_2.insert(0, 4);
    // block_list_2.insert(0, 6);
    // block_list_2.insert(0, 8);
    block_list_1.del(1, "another?");
    std::cout << block_list_1.find(1) << '\n';
    auto p = block_list_2.find(0);
    std::cout << block_list_1.size() << '\n';
    std::cout << p << '\n';
    return 0;
}