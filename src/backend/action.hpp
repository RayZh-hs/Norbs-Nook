#pragma once

#include <constants.hpp>
#include <memory>
#include <norb_naivelist.hpp>

#include "norb_string.hpp"
#include "constants.hpp"

using namespace norb::norbs_nook_constants::account_constants;
using namespace norb::norbs_nook_constants::action_constants;

namespace norb {
    struct Action {
        string<account_userid_len> user_id{};
        string<action_description_len> description{};

        Action(const std::string &user, const std::string &description)
            : user_id(user), description(description) {}

        Action() = default;

        friend std::ostream &operator << (std::ostream &os, const Action &action);
    };

    class ActionManager {
    public:
        ActionManager(const std::string &f_action_history_name, const std::string &f_transaction_history_name);
        ActionManager()
            : ActionManager(action_history_name, transaction_history_name) {}

        void RegisterAction(const Action &action);

        void RegisterTransaction(double value);

        std::vector<Action> GetAllActions() const;

        std::vector<double> GetAllTransactions();

        // Returns npos, npos if
        std::pair<double, double> GetTransactionSum();
        std::pair<double, double> GetTransactionSum(int count);

        constexpr static double npos = -1.00;

    private:
        std::unique_ptr<algo::FiledNaiveList<Action>> action_history;
        std::unique_ptr<algo::FiledNaiveList<double>> transaction_history;
    };
} // norb
