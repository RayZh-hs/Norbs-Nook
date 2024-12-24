//
// Created by Rayzh on 2024/12/23.
//

#include "global.hpp"

#include <iostream>

#include "norb_utils.hpp"

using namespace norb::norbs_nook_constants::global_constants;
using namespace norb::norbs_nook_constants::log_constants;

namespace norb {
    GlobalVariableManager::GlobalVariableManager(const std::string &f_blocklist_global_name,
                                                 const std::string &f_f_uid_global_name) {
        utils::fAssert(f_blocklist_global_name);
        utils::fAssert(f_f_uid_global_name);
        f_blocklist_global.open(f_blocklist_global_name, file_open_mode);
        f_uid_global.open(f_f_uid_global_name, file_open_mode);
        assert(f_blocklist_global.good());
        assert(f_uid_global.good());
        // We need to ensure that f_uid_global has a number inside.
        if (utils::fEmpty(f_uid_global)) {
            f_uid_global.seekp(0);
            int _ = 0;
            utils::bWrite(f_uid_global, _);
            assert(f_uid_global.good());
        }
        // Reset the file pointers.
        f_blocklist_global.seekg(0);
        f_uid_global.seekg(0);
        assert(f_blocklist_global.good());
        assert(f_uid_global.good());
    }

    GlobalVariableManager::GlobalVariableManager()
        : GlobalVariableManager(blocklist_global_name, uid_global_name) {}

    GlobalVariableManager::~GlobalVariableManager() {
        assert(f_blocklist_global.good());
        assert(f_uid_global.good());
        f_blocklist_global.close();
        f_uid_global.close();
    }

    GlobalAssetManager::GlobalAssetManager() {
        global_variable_manager = std::make_unique<GlobalVariableManager>();
        auto &blocklist_global = global_variable_manager->f_blocklist_global;
        auto &uid_global = global_variable_manager->f_uid_global;
        account_manager = std::make_unique<AccountManager>(blocklist_global);
        book_manager = std::make_unique<BookManager>(uid_global, blocklist_global);
        action_manager = std::make_unique<ActionManager>();
        logger = std::make_unique<Logger>(log_level, log_name, std::cout);
    }

}
