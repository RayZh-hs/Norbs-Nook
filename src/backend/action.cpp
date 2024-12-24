//
// Created by Rayzh on 2024/12/24.
//

#include "action.hpp"

namespace norb {

    std::ostream &operator<<(std::ostream &os, const Action &action) {
        os << '[' << action.user_id << ']' << '\t' << action.description << '\n';
        return os;
    }

    ActionManager::ActionManager(const std::string &f_action_history_name, const std::string &f_transaction_history_name) {
        action_history = std::make_unique<algo::FiledNaiveList<Action>>(f_action_history_name);
        transaction_history = std::make_unique<algo::FiledNaiveList<double>>(f_transaction_history_name);
    }

    void ActionManager::RegisterAction(const Action &action) {
        action_history->push_back(action);
    }

    void ActionManager::RegisterTransaction(double value) {
        transaction_history->push_back(value);
    }

    std::vector<Action> ActionManager::GetAllActions() const {
        return action_history->as_vector();
    }

    std::vector<double> ActionManager::GetAllTransactions() {
        return transaction_history->as_vector();
    }

    std::pair<double, double> ActionManager::GetTransactionSum() {
        return ActionManager::GetTransactionSum(transaction_history->size());
    }

    std::pair<double, double> ActionManager::GetTransactionSum(int count) {
        int size = transaction_history->size();
        if (count > size) {
            return {npos, npos};
        }
        double in = 0, out = 0;
        for (int i = size - count; i < size; i++) {
            double buff = transaction_history->get(i);
            // Nothing would have a transaction value of 0!
            assert(buff != 0);
            if (buff > 0) {
                in += buff;
            }
            else {
                out += buff;
            }
        }
        return std::make_pair(in, out);
    }


} // norb