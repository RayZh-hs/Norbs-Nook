#include <memory>

#include "norb_blocklist.hpp"
#include "norb_string.hpp"

typedef long long lld;
typedef norb::algo::FiledBlockList<lld, norb::string<10>> list_type_;

int main() {
    std::fstream f_global;
    norb::utils::fAssert("global.bin");
    f_global.open("global.bin", std::ios::in | std::ios::out | std::ios::binary);
    f_global.seekg(0);
    {
        std::unique_ptr<list_type_> list;
        list = std::make_unique<list_type_>("lld-head.bin", "lld-body.bin", f_global);
        list->insert(1, "Hello");
    }   // Here list should be released.
    f_global.close();
}