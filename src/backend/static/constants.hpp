#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

namespace norb {
    namespace norbs_nook_constants {
        namespace account_constants {
            constexpr int account_userid_len = 30 + 2;
            constexpr int account_password_len = 30 + 2;
            constexpr int account_username_len = 30 + 2;

            const std::string account_file_head = "account-head.bin";
            const std::string account_file_body = "account-body.bin";
        }
    }
}

#endif //CONSTANTS_HPP
