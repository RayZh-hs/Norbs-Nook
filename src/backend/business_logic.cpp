#include "business_logic.hpp"

namespace norb {
    BusinessLogicInterface::~BusinessLogicInterface() = default;

    BusinessLogicImplement::BusinessLogicImplement(): BusinessLogicInterface() {
        manager = std::make_unique<GlobalAssetManager>();
    }

    BusinessLogicImplement::~BusinessLogicImplement() = default;

    void BusinessLogicImplement::Exit() {
        manager->logger->Log(level::INFO, "Called Exit.");
        throw QuitUtilityException();
    }

    void BusinessLogicImplement::SwitchUser(const std::string &user_id) {
        manager->logger->Log(level::DEBUG, std::string("Called SwitchUser: ") + user_id);
        const auto user = manager->account_manager->GetActiveUser();
        const bool success = manager->account_manager->SwitchUser(user_id);
        if (success) {
            manager->logger->Log(level::INFO, "SwitchUser successful!");
            (*manager->logger) << level::DEBUG << "Current user: " << manager->account_manager->GetActiveUser() << Logger::EOS;
            if (user.privilege == 3) {
                manager->action_manager->RegisterAction({
                    user.username,
                    "switched to user: " + std::string(manager->account_manager->GetActiveUser().username)
                });
            }
            manager->book_manager->StackCreate();
        } else {
            manager->logger->Log(level::WARNING, "Failed to switch user!");
            throw UtilityException("USER SWITCH FAILURE");
        }
    }

    void BusinessLogicImplement::SwitchUser(const std::string &user_id, const std::string &password) {
        // manager->logger->Log(level::INFO, std::string("Called SwitchUser: ") + user_id + " with password " + password);
        *(manager->logger) << level::DEBUG << "Called SwitchUser: " << user_id << " with password #" << hash(password) << Logger::EOS;
        const auto user = manager->account_manager->GetActiveUser();
        const bool success = manager->account_manager->SwitchUser(user_id);
        if (success) {
            manager->logger->Log(level::INFO, "SwitchUser successful!");
            (*manager->logger) << level::DEBUG << "Current user: " << manager->account_manager->GetActiveUser() << Logger::EOS;
            if (user.privilege == 3) {
                manager->action_manager->RegisterAction({
                    user.username,
                    "switched to user: " + std::string(manager->account_manager->GetActiveUser().username)
                });
            }
            manager->book_manager->StackCreate();
        } else {
            manager->logger->Log(level::WARNING, "Failed to switch user!");
            throw UtilityException("USER SWITCH FAILURE");
        }
    }

    void BusinessLogicImplement::Logout() {
        manager->logger->Log(level::DEBUG, std::string("Called Logout"));
        const auto user = manager->account_manager->GetActiveUser();
        const bool success = manager->account_manager->Logout();
        if (success) {
            manager->logger->Log(level::INFO, "Logout Successful.");
            (*manager->logger) << level::DEBUG << "Current user: " << manager->account_manager->GetActiveUser() << Logger::EOS;
            if (user.privilege == 3) {
                manager->action_manager->RegisterAction({
                    user.username, "logged out"
                });
            }
            manager->book_manager->StackRemove();
        } else {
            manager->logger->Log(level::WARNING, "Failed to log out. Stack is empty!");
            throw UtilityException("LOGOUT FAILURE");
        }
    }

    void BusinessLogicImplement::Register(const std::string &user_id, const std::string &password,
                                          const std::string &username) {
        manager->logger->Log(level::DEBUG, std::string("Called Register"));
        if (!manager->account_manager->Register(user_id, password, username)) {
            manager->logger->Log(level::WARNING, "Failed to register. UserID collided!");
            throw UtilityException("REGISTER FAILURE");
        }
    }

    void BusinessLogicImplement::Password(const std::string &user_id, const std::string &password) {
        manager->logger->Log(level::DEBUG, std::string("Called Password without current password."));
        if (!manager->account_manager->ChangePassword(user_id, password)) {
            manager->logger->Log(level::WARNING, "Failed to change password.");
            throw UtilityException("PASSWORD FAILURE");
        } else {
            auto user = manager->account_manager->GetActiveUser();
            if (user.privilege == 3) {
                manager->action_manager->RegisterAction({
                    user.username, "changed the password of user " + user_id
                });
            }
        }
    }

    void BusinessLogicImplement::Password(const std::string &user_id, const std::string &current_password,
                                          const std::string &password) {
        manager->logger->Log(level::DEBUG, std::string("Called Password with current password."));
        if (!manager->account_manager->ChangePassword(user_id, password, current_password)) {
            manager->logger->Log(level::WARNING, "Failed to change password.");
            throw UtilityException("PASSWORD FAILURE");
        }
    }

    void BusinessLogicImplement::UserAdd(const std::string &user_id, const std::string &password,
                                         const std::string &privilege, const std::string &username) {
        manager->logger->Log(level::DEBUG, std::string("Called UserAdd"));
        int num_p;
        switch (privilege[0]) {
            // case '0':
            //     num_p = 0; break;
            case '1':
                num_p = 1;
                break;
            case '3':
                num_p = 3;
                break;
            case '7':
                num_p = 7;
                break;
            default:
                throw UtilityException("PRIVILEGE UNDEFINED ERROR");
        }
        if (!manager->account_manager->UserAdd(user_id, password, num_p, username)) {
            manager->logger->Log(level::WARNING, "Failed to add user.");
            throw UtilityException("PASSWORD FAILURE");
        } else {
            auto user = manager->account_manager->GetActiveUser();
            if (user.privilege == 3) {
                manager->action_manager->RegisterAction({
                    user.username, "added new user " + user_id
                });
            }
        }
    }

    void BusinessLogicImplement::Delete(const std::string &user_id) {
        manager->logger->Log(level::DEBUG, std::string("Called Delete"));
        if (!manager->account_manager->Delete(user_id)) {
            manager->logger->Log(level::WARNING, "Failed to delete user.");
            throw UtilityException("DELETE FAILURE");
        } else {
            auto user = manager->account_manager->GetActiveUser();
            if (user.privilege == 3) {
                manager->action_manager->RegisterAction({
                    user.username, "deleted user " + user_id
                });
            }
        }
    }

    std::vector<Book> BusinessLogicImplement::FindAll() {
        manager->logger->Log(level::DEBUG, std::string("Called FindAll"));
        return manager->book_manager->FindAll();
    }

    std::vector<Book> BusinessLogicImplement::FindByAuthor(const std::string &author) {
        manager->logger->Log(level::DEBUG, std::string("Called FindByAuthor: " + author));
        return manager->book_manager->FindByAuthor(author);
    }

    std::vector<Book> BusinessLogicImplement::FindByIsbn(const std::string &isbn) {
        manager->logger->Log(level::DEBUG, std::string("Called FindByISBN: " + isbn));
        return manager->book_manager->FindByIsbn(isbn);
    }

    std::vector<Book> BusinessLogicImplement::FindByKeyword(const std::string &keyword) {
        manager->logger->Log(level::DEBUG, std::string("Called FindByKeyword: " + keyword));
        return manager->book_manager->FindByKeyword(keyword);
    }

    std::vector<Book> BusinessLogicImplement::FindByName(const std::string &name) {
        manager->logger->Log(level::DEBUG, std::string("Called FindByName: " + name));
        return manager->book_manager->FindByName(name);
    }

    double BusinessLogicImplement::Buy(const std::string &isbn, int quantity) {
        manager->logger->Log(level::DEBUG, std::string("Called Buy"));
        auto success = manager->book_manager->Buy(manager->book_manager->GetId(isbn), quantity);
        if (success == (BookManager::npos)) {
            throw UtilityException("BOOK PURCHASE ERROR");
        } else {
            (*manager->logger) << level::INFO << "Successfully purchased #" << isbn << " x" << quantity << Logger::EOS;
            manager->action_manager->RegisterTransaction(success);
        }
        return success;
    }

    void BusinessLogicImplement::Select(const std::string &isbn) {
        manager->logger->Log(level::DEBUG, std::string("Called Select"));
        manager->book_manager->Select(isbn);
        manager->logger->Log(level::INFO, "Selection successful");
    }

    void BusinessLogicImplement::Modify(const Book &new_info) {
        manager->logger->Log(level::DEBUG, std::string("Called Modify"));
        if (!manager->book_manager->Modify(new_info)) {
            manager->logger->Log(level::WARNING, "Modification failed!");
            throw UtilityException("MODIFY ERROR");
        } else {
            manager->logger->Log(level::INFO, "Modification successful!");
        }
    }

    void BusinessLogicImplement::Import(int quantity, double total_cost) {
        manager->logger->Log(level::DEBUG, std::string("Called Import"));
        if (manager->book_manager->Import(quantity, total_cost)) {
            manager->logger->Log(level::INFO, "Import successful!");
            manager->action_manager->RegisterTransaction(-total_cost);
        } else {
            manager->logger->Log(level::WARNING, "Import failed!");
            throw UtilityException("IMPORT ERROR");
        }
    }

    std::pair<double, double> BusinessLogicImplement::GetFinance() {
        manager->logger->Log(level::DEBUG, std::string("Called GetFinance"));
        auto ret = manager->action_manager->GetTransactionSum();
        if (ret.first == ActionManager::npos) {
            manager->logger->Log(level::WARNING, std::string("Get finance failed"));
            throw UtilityException("GET FINANCE FAILURE");
        } else {
            manager->logger->Log(level::DEBUG, std::string("GetFinance succeeded"));
            return ret;
        }
    }

    std::pair<double, double> BusinessLogicImplement::GetFinance(int count) {
        manager->logger->Log(level::DEBUG, std::string("Called GetFinance"));
        auto ret = manager->action_manager->GetTransactionSum(count);
        if (ret.first == ActionManager::npos) {
            manager->logger->Log(level::WARNING, std::string("Get finance failed"));
            throw UtilityException("GET FINANCE FAILURE");
        } else {
            manager->logger->Log(level::DEBUG, std::string("GetFinance succeeded"));
            return ret;
        }
    }

    std::vector<std::string> BusinessLogicImplement::GetLog() {
        manager->logger->Log(level::DEBUG, std::string("Called GetLog"));
        return manager->logger->GetAll();
    }

    std::vector<double> BusinessLogicImplement::ReportFinance() {
        manager->logger->Log(level::DEBUG, std::string("Called ReportFinance"));
        return manager->action_manager->GetAllTransactions();
    }

    std::vector<Action> BusinessLogicImplement::ReportEmployee() {
        manager->logger->Log(level::DEBUG, std::string("Called ReportEmployee"));
        return manager->action_manager->GetAllActions();
    }
}
