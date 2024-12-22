#include <iostream>

#include "account.hpp"

int main() {
    std::fstream f_global;
    norb::utils::fAssert("global.testdata");
    f_global.open("global.testdata", std::ios::in | std::ios::out | std::ios::binary);
    f_global.seekg(0);
    norb::AccountManager account_manager(f_global);
    std::cout << account_manager.SwitchUser("root", "sjtu") << '\t';
    std::cout << account_manager.GetActiveUser().userid << '\n';
    std::cout << account_manager.Register("user-1", "password", "username-1") << '\t';
    std::cout << account_manager.GetActiveUser().userid << '\n';
    std::cout << account_manager.SwitchUser("user-1", "password") << '\t';
    std::cout << account_manager.GetActiveUser().userid << '\n';
    std::cout << account_manager.Logout() << '\t';
    std::cout << account_manager.GetActiveUser().userid << '\n';
    return 0;
}