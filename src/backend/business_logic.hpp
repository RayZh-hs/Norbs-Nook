#pragma once

#include <string>
#include <exception>

#include "global.hpp"

namespace norb {
    class UtilityException : std::exception {
        // using std::exception::exception;
    public:
        explicit UtilityException(const char *msg)
            : msg(msg) {
        }

        const char *what() const noexcept override {
            return msg.c_str();
        }

    private:
        std::string msg;
    };

    class QuitUtilityException final : UtilityException {
    public:
        explicit QuitUtilityException()
            : UtilityException("QUIT") {
        }
    };

    // BusinessLogicInterface is the base class of the interface that encapsulate all logic in the project.
    class BusinessLogicInterface {
    public:
        virtual ~BusinessLogicInterface() = 0;

        virtual void Exit() = 0;

        virtual void SwitchUser(const std::string &user_id) = 0;

        virtual void SwitchUser(const std::string &user_id, const std::string &password) = 0;

        virtual void Logout() = 0;

        virtual void Register(const std::string &user_id, const std::string &password, const std::string &username) = 0;

        virtual void Password(const std::string &user_id, const std::string &password) = 0;

        virtual void Password(const std::string &user_id, const std::string &current_password, const std::string &password) = 0;

        virtual void UserAdd(const std::string &user_id, const std::string &password, const std::string &privilege,
                             const std::string &username) = 0;

        virtual void Delete(const std::string &user_id) = 0;

        virtual std::vector<Book> FindAll() = 0;

        virtual std::vector<Book> FindByIsbn(const std::string &isbn) = 0;

        virtual std::vector<Book> FindByName(const std::string &name) = 0;

        virtual std::vector<Book> FindByAuthor(const std::string &author) = 0;

        virtual std::vector<Book> FindByKeyword(const std::string &keyword) = 0;

        virtual double Buy(const std::string &isbn, int quantity) = 0;

        virtual void Select(const std::string &isbn) = 0;

        virtual void Modify(const Book &new_info) = 0;

        virtual void Import(int quantity, double total_cost) = 0;

        virtual std::pair<double, double> GetFinance() = 0;

        virtual std::pair<double, double> GetFinance(int count) = 0;

        virtual std::vector<std::string> GetLog() = 0;

        virtual std::vector<double> ReportFinance() = 0;

        virtual std::vector<Action> ReportEmployee() = 0;

        virtual Book GetSelectedBook() = 0;

        virtual bool IsbnIsOccupied(const std::string &) = 0;
    };

    // This class implements the interface.
    // It assumes that everything passed in is in valid condition.
    // That is, all strings must have lengths that are valid, etc.
    // This verification is the job of the UserInterface classes.
    class BusinessLogicImplement : public BusinessLogicInterface {
    public:
        using level = Logger::level;

        BusinessLogicImplement();

        ~BusinessLogicImplement() override;

        void Exit() override;

        void SwitchUser(const std::string &user_id) override;

        void SwitchUser(const std::string &user_id, const std::string &password) override;

        void Logout() override;

        void Register(const std::string &user_id, const std::string &password, const std::string &username) override;

        void Password(const std::string &user_id, const std::string &password) override;

        void Password(const std::string &user_id, const std::string &current_password, const std::string &password) override;

        void UserAdd(const std::string &user_id, const std::string &password, const std::string &privilege, const std::string &username) override;

        void Delete(const std::string &user_id) override;

        std::vector<Book> FindAll() override;

        std::vector<Book> FindByAuthor(const std::string &author) override;

        std::vector<Book> FindByIsbn(const std::string &isbn) override;

        std::vector<Book> FindByKeyword(const std::string &keyword) override;

        std::vector<Book> FindByName(const std::string &name) override;

        double Buy(const std::string &isbn, int quantity) override;

        void Select(const std::string &isbn) override;

        void Modify(const Book &new_info) override;

        void Import(int quantity, double total_cost) override;

        std::pair<double, double> GetFinance() override;

        std::pair<double, double> GetFinance(int count) override;

        std::vector<std::string> GetLog() override;

        std::vector<double> ReportFinance() override;

        std::vector<Action> ReportEmployee() override;

        Book GetSelectedBook() override;

        bool IsbnIsOccupied(const std::string &) override;

    private:
        std::unique_ptr<GlobalAssetManager> manager;

        void RequirePrivilege(const int privilege) const {
            if (manager->account_manager->GetCurPrivilege() < privilege)
                throw UtilityException("UNDERPRIVILEGED ERROR");
        }
    };
}
