#pragma once

#include <string>

namespace norb::norbs_nook_constants {
    namespace account_constants {
        constexpr inline int account_userid_len = 30 + 2;
        constexpr inline int account_password_len = 30 + 2;
        constexpr inline int account_username_len = 30 + 2;

        const inline std::string account_file_head = "account-head.bin";
        const inline std::string account_file_body = "account-body.bin";
    }

    namespace bookkeeper_constants {
        constexpr inline int book_isbn_len = 20 + 2;
        constexpr inline int book_name_len = 60 + 2;
        constexpr inline int book_author_len = 60 + 2;
        constexpr inline int book_keyword_len = 60 + 2;

        constexpr inline char book_keyword_separator = '|';

        const inline std::string &book_list_head_name = "book-list-head.bin";
        const inline std::string &book_list_body_name = "book-list-body.bin";
        const inline std::string &book_isbn_head_name = "book-isbn-head.bin";
        const inline std::string &book_isbn_body_name = "book-isbn-body.bin";
        const inline std::string &book_name_head_name = "book-name-head.bin";
        const inline std::string &book_name_body_name = "book-name-body.bin";
        const inline std::string &book_author_head_name = "book-author-head.bin";
        const inline std::string &book_author_body_name = "book-author-body.bin";
        const inline std::string &book_keyword_head_name = "book-keyword-head.bin";
        const inline std::string &book_keyword_body_name = "book-keyword-body.bin";
    }

    namespace action_constants {
        constexpr inline int action_description_len = 100;

        const inline std::string &action_history_name = "action-history.bin";
        const inline std::string &transaction_history_name = "transaction-history.bin";
    }
}