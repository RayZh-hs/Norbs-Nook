#include <iostream>

#include "norb_naivelist.hpp"

int main() {
    norb::algo::FiledNaiveList<int> list("test-nl.testdata");
    // list.set(0, 1);
    // list.set(2, 4);
    // list.set(1, 3);
    for (int i = 0; i < list.size(); i++) {
        std::cout << list.get(i) << ' ';
    }
    std::cout << '\n';
    return 0;
}