#pragma once

#include <memory>
#include <vector>

#include "constants.hpp"
#include "norb_string.hpp"
#include "norb_blocklist.hpp"

// The using namespace here only imports such constants that the file needs, and thus is justified.
using namespace norb::norbs_nook_constants::account_constants;

namespace norb {
    class Account {
    public:
        Account() = default;

        Account(const string<account_userid_len> &userid, const std::string &password, const std::string &username,
                const int privilege)
            : userid(userid), hashed_password(hash(password)), username(username), privilege(privilege) {
        }

        ~Account() = default;

        bool operator == (Account other) const {
            return userid == other.userid;
        }

        bool operator < (Account other) const {
            return userid < other.userid;
        }

        bool operator > (Account other) const {
            return userid > other.userid;
        }

        bool operator <= (Account other) const {
            return userid <= other.userid;
        }

        bool operator >= (Account other) const {
            return userid >= other.userid;
        }

        friend std::ostream& operator << (std::ostream& os, const Account& account) {
            os << "{" << account.privilege << "} [" << account.userid << ']' << ' ' << account.username << " #" << account.hashed_password;
            return os;
        }

        string<account_userid_len> userid{};
        // string<account_password_len> password{};
        // Storing hashed passwords are safer than storing them in plain text.
        lld hashed_password{};
        string<account_username_len> username{};
        int privilege = 0;
    };

    class AccountManager {
    public:
        AccountManager(const std::string &f_head_name, const std::string &f_body_name,
                       std::reference_wrapper<std::fstream> f_global) {
            account_list = std::make_unique<account_list_t_>(f_head_name, f_body_name, f_global);
            if (account_list->empty()) {
                // Create the root account.
                // TODO What's the username of root ?
                Account root("root", "sjtu", "root", 7);
                account_list->insert(hash("root"), root);
            }
        }

        // As a singleton, this is the preferred source of construction.
        explicit AccountManager(std::reference_wrapper<std::fstream> f_global)
            : AccountManager(account_file_head, account_file_body, f_global) {
        }

        // Returns the number of all accounts registered.
        int AccountSize() const;

        // Returns the size of the login stack.
        int StackSize() const;

        // Returns whether a user exists in the database, given his/her userid.
        bool ExistUser(const std::string &userid) const;

        // Return whether a user exists and the password is correct.
        bool CorrectUserAndPassword(const std::string &userid, const std::string &password) const;

        // Switch to another user. Returns whether the operation is successful.
        bool SwitchUser(const std::string &userid, const std::string &password = "");

        // Log out the current user. Returns whether the operation is successful.
        bool Logout();

        // Register a new user, with the privilege of 1. Returns whether the operation is successful.
        bool Register(const std::string &userid, const std::string &password, const std::string &username);

        // Change the password for a user. Returns whether the action is valid.
        // Note that the order of the strings are shifted in this method.
        bool ChangePassword(const std::string &userid, const std::string &new_password, const std::string &current_password = "");

        // Add a user to the system. Returns whether the operation is successful.
        bool UserAdd(const std::string &userid, const std::string &password, int privilege, const std::string &username);

        // Delete a user. Returns whether the operation is successful.
        bool Delete(const std::string &userid) noexcept;

        // Get the current active user.
        Account GetActiveUser() const;

        [[nodiscard]] int GetCurPrivilege() const;

        void DebugPrintAccountInfo () const {
            auto accounts = account_list->asDict();
            for (auto i : accounts) {
                std::cout << i.first << "\t:\t" << i.second << '\n';
            }
        }

    private:
        // account_list is a map from the hash of userid to the full information.
        typedef algo::FiledBlockList<lld, Account> account_list_t_;
        std::unique_ptr<account_list_t_> account_list;
        std::vector<Account> login_stack;
    };
}