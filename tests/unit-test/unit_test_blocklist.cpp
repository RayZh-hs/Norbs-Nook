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
    norb::algo::FiledBlockList<int, norb::string<25>> block_list_1("head1.testdata", "body1.testdata", f_global);
    norb::algo::FiledBlockList<int, int> block_list_2("head2.testdata", "body2.testdata", f_global);
    block_list_1.insert(1, "one");
    block_list_1.insert(2, "two_another");
    block_list_1.insert(3, "a_another");
    block_list_1.insert(4, "c_another1");
    block_list_1.insert(4, "c_another2");
    block_list_1.insert(4, "c_another3");
    block_list_1.insert(5, "d_another");
    block_list_1.insert(6, "r_another");

    std::cout << block_list_1.valAsVector() << '\n';
    std::cout << block_list_1.findFirst(4) << '\n';

    return 0;
}