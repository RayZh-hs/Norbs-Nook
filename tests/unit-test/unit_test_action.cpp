#include <iostream>

#include "action.hpp"

int main() {
    norb::ActionManager action_manager;

    // Test Register
    action_manager.RegisterAction({
        "norb", "made a piston door"
    });
    action_manager.RegisterAction({
        "norb", "completed Parkour Paradise"
    });
    action_manager.RegisterTransaction(15);
    action_manager.RegisterTransaction(5);
    action_manager.RegisterTransaction(-3);
    action_manager.RegisterTransaction(-1);
    action_manager.RegisterTransaction(6);

    // Test Get
    std::cout << ' ' << action_manager.GetAllActions() << '\n';
    std::cout << action_manager.GetAllTransactions() << '\n';
    std::cout << action_manager.GetTransactionSum(3) << '\n';
    std::cout << action_manager.GetTransactionSum(7) << '\n';
}