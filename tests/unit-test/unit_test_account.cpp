#include <iostream>

#include "account.hpp"

int main() {
    std::fstream f_global;
    norb::utils::fAssert("global.testdata");
    f_global.open("global.testdata", std::ios::in | std::ios::out | std::ios::binary);
    f_global.seekg(0);
    norb::AccountManager account_manager(f_global);
    std::cout << std::boolalpha;
    std::cout << account_manager.SwitchUser("root", "sjtu") << '\t';
    std::cout << account_manager.GetActiveUser().userid << '\n';
    std::cout << account_manager.Register("user-1", "password", "username-1") << '\t';
    std::cout << account_manager.GetActiveUser().userid << '\n';
    std::cout << account_manager.SwitchUser("user-1", "password") << '\t';
    std::cout << account_manager.GetActiveUser().userid << '\n';
    std::cout << account_manager.SwitchUser("root", "sjtu++") << '\t';
    std::cout << account_manager.SwitchUser("root", "sjtu") << '\t';
    std::cout << account_manager.GetActiveUser().userid << '\n';
    std::cout << account_manager.UserAdd("user-2", "user-2-password", 3, "username-2") << '\t';
    std::cout << account_manager.UserAdd("user-2", "user-2-false", 3, "username-2-false") << '\n';
    std::cout << "Accounts now : " << account_manager.AccountSize() << '\n';
    std::cout << "Login Stack now : " << account_manager.StackSize() << '\n';
    while (account_manager.StackSize() != 0) {
        std::cout << account_manager.GetActiveUser().userid << '\t';
        std::cout << account_manager.Logout() << '\n';
    }
    std::cout << account_manager.Delete("user-3") << '\n';
    std::cout << account_manager.Delete("user-2") << '\n';
    std::cout << "Accounts now : " << account_manager.AccountSize() << '\n';
    std::cout << "Login Stack now : " << account_manager.StackSize() << '\n';

    std::cout << "\nExistUser\n";
    std::cout << account_manager.ExistUser("user-2") << '\t';
    std::cout << account_manager.ExistUser("user-1") << '\t';

    std::cout << "\nCorrectUserAndPassword\n";
    std::cout << account_manager.CorrectUserAndPassword("root", "sjtu")   << '\t';
    std::cout << account_manager.CorrectUserAndPassword("user-1", "sjtu") << '\t';

    std::cout << "\nChangePassword\n";
    std::cout << account_manager.ChangePassword("root", "roots") << '\t';
    std::cout << account_manager.ChangePassword("root", "roots", "sjtu") << '\n';
    std::cout << account_manager.SwitchUser("root", "roots") << '\n';

    std::cout << account_manager.GetActiveUser().username << '\n';
    return 0;
}