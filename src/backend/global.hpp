#pragma once

#include <action.hpp>

#include "account.hpp"
#include "bookkeeper.hpp"
#include "logging.hpp"

namespace norb {
    // Global Variables include:
    // 1. the global pool shared by BlockList (externally managed)
    // 2. current upperbound for unique_id of books
    class GlobalVariableManager {
    public:
        std::fstream f_blocklist_global;
        std::fstream f_uid_global;

        GlobalVariableManager(const std::string &f_blocklist_global_name, const std::string &f_f_uid_global_name);

        GlobalVariableManager();

        ~GlobalVariableManager();

    private:
        static constexpr auto file_open_mode = std::ios::binary | std::ios::in | std::ios::out;
    };

    // Global Assets include:
    // 1. AccountManager: for account management
    // 2. BookManager:    for book and stock management
    // 3. ActionManager:  for recording the history and tracking the financial status
    // 4. Logger:         for creating and keeping track of logs
    class GlobalAssetManager {
        // No named constructors will be provided for this class because it is redundant and hard-to-read.
        // We will rely on the default names from constants.hpp instead.
    public:
        // The first declared in a class is the last to be released!
        std::unique_ptr<GlobalVariableManager> global_variable_manager;

        // These classes should be destructed BEFORE the global variables are cleared.
        std::unique_ptr<AccountManager> account_manager;
        std::unique_ptr<BookManager> book_manager;
        std::unique_ptr<ActionManager> action_manager;
        std::unique_ptr<Logger> logger;

        GlobalAssetManager();

        ~GlobalAssetManager() = default;
    };
}
