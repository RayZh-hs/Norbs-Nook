#include "account.hpp"

namespace norb {
    int AccountManager::AccountSize() const {
        return account_list->size();
    }

    int AccountManager::StackSize() const {
        return login_stack.size();
    }

    bool AccountManager::ExistUser(const std::string &userid) const {
        return account_list->count(hash(userid)) != 0;
    }

    bool AccountManager::CorrectUserAndPassword(const std::string &userid, const std::string &password) const {
        auto found = account_list->find(hash(userid));
        if (found.empty()) {
            // User not found!
            return false;
        }
        // If used correctly, there should at most be one occurrence.
        assert(found.size() == 1);
        const auto &ref = found[0];
        return ref.hashed_password == hash(password);
    }

    bool AccountManager::SwitchUser(const std::string &userid, const std::string &password) {
        const int cur_privilege = GetCurPrivilege();
        auto found = account_list->find(hash(userid));
        if (found.empty()) {
            // User not found.
            return false;
        }
        // If used correctly, there should at most be one occurrence.
        assert(found.size() == 1);
        const auto &ref = found[0];
        if (cur_privilege <= ref.privilege) {
            // We need to match the password.
            if (ref.hashed_password != hash(password)) {
                return false;
            }
        }
        // Add the new user to the top of the stack.
        login_stack.push_back(ref);
        return true;
    }

    bool AccountManager::Logout() {
        if (GetCurPrivilege() < 1 || login_stack.empty()) {
            return false;
        }
        login_stack.pop_back();
        return true;
    }

    bool AccountManager::Register(const std::string &userid, const std::string &password, const std::string &username) {
        if (account_list->count(hash(userid))) {
            // User already exists.
            return false;
        }
        Account new_account(userid, password, username, 1);
        account_list->insert(hash(userid), new_account);
        return true;
    }

    bool AccountManager::ChangePassword(const std::string &userid, const std::string &new_password,
                                        const std::string &current_password) {
        if (GetCurPrivilege() < 1) {
            return false;
        }
        const auto hashed_userid = hash(userid);
        const int cur_privilege = GetCurPrivilege();
        auto found = account_list->find(hashed_userid);
        if (found.empty()) {
            // User not found.
            return false;
        }
        // If used correctly, there should at most be one occurrence.
        assert(found.size() == 1);
        const auto &old_account = found[0];
        const auto cur_hashed_password = hash(current_password);
        const auto new_hashed_password = hash(new_password);
        if (cur_privilege < 7 && old_account.hashed_password != cur_hashed_password) {
            return false;
        }
        // Update the password.
        Account new_account(old_account);
        new_account.hashed_password = new_hashed_password;
        // First fix all passwords in the login_stack.
        for (auto &i: login_stack) {
            if (hash(i.userid) == hashed_userid) {
                i.hashed_password = new_hashed_password;
            }
        }
        // Then change the password in the account_list
        account_list->del(hashed_userid, old_account);
        account_list->insert(hashed_userid, new_account);
        return true;
    }

    bool AccountManager::UserAdd(const std::string &userid, const std::string &password, const int privilege,
                                 const std::string &username) {
        if (GetCurPrivilege() <= privilege || GetCurPrivilege() < 3) {
            return false;
        }
        lld hashed_userid = hash(userid);
        if (account_list->count(hashed_userid)) {
            return false;
        }
        account_list->insert(
            hashed_userid, Account(userid, password, username, privilege)
        );
        return true;
    }

    bool AccountManager::Delete(const std::string &userid) noexcept {
        // TODO False alarm? Or true bug.
        if (GetCurPrivilege() < 7) {
            return false;
        }
        const lld hashed_userid = hash(userid);
        auto found = account_list->find(hashed_userid);
        if (found.empty()) {
            return false;
        }
        assert(found.size() == 1);
        const Account account = found[0];
        for (auto &i: login_stack) {
            if (i.userid == string<account_userid_len>(userid)) {
                return false;
            }
        }
        account_list->del(hashed_userid, account);
        return true;
    }

    Account AccountManager::GetActiveUser() const {
        return login_stack.empty() ? Account() : login_stack.back();
    }

    int AccountManager::GetCurPrivilege() const {
        return login_stack.empty() ? 0 : login_stack.back().privilege;
    }
}
