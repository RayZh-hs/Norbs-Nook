#include <iostream>
#include <cassert>

#include "norb_string.hpp"

int main() {
    const std::string a1 = "Hello World";
    const char a2[] = "Hello World";
    const norb::string<50> a3 = "Hello World";

    assert(norb::hash(a1) == norb::hash(a2));
    assert(norb::hash(a2) == norb::hash<50>(a3));

    std::cout << a3.split(' ') << '\n';
    std::cout << a3.split('o') << '\n';
    std::cout << a3.split('=') << '\n';

    const norb::string<50> b = "a|b|cc|d";

    std::cout << b.split('|') << '\n';
    std::cout << b.split_and_hash('|') << '\n';

    std::cout << "Congratulations! You passed all tests!" << '\n';

    return 0;
}