#pragma once

#include <iostream>
#include <logging.hpp>
#include <string>
#include <regex>

namespace norb::norbs_nook_constants {
    namespace account_constants {
        constexpr inline int account_userid_len = 30 + 2;
        constexpr inline int account_password_len = 30 + 2;
        constexpr inline int account_username_len = 30 + 2;

        const inline std::string account_file_head          = "generated/data/account-head.bin";
        const inline std::string account_file_body          = "generated/data/account-body.bin";
    }

    namespace bookkeeper_constants {
        constexpr inline int book_isbn_len                  = 20 + 2;
        constexpr inline int book_name_len                  = 60 + 2;
        constexpr inline int book_author_len                = 60 + 2;
        constexpr inline int book_keyword_len               = 60 + 2;

        constexpr inline char book_keyword_separator        = '|';

        const inline std::string &book_list_head_name       = "generated/data/book-list-head.bin";
        const inline std::string &book_list_body_name       = "generated/data/book-list-body.bin";
        const inline std::string &book_isbn_head_name       = "generated/data/book-isbn-head.bin";
        const inline std::string &book_isbn_body_name       = "generated/data/book-isbn-body.bin";
        const inline std::string &book_name_head_name       = "generated/data/book-name-head.bin";
        const inline std::string &book_name_body_name       = "generated/data/book-name-body.bin";
        const inline std::string &book_author_head_name     = "generated/data/book-author-head.bin";
        const inline std::string &book_author_body_name     = "generated/data/book-author-body.bin";
        const inline std::string &book_keyword_head_name    = "generated/data/book-keyword-head.bin";
        const inline std::string &book_keyword_body_name    = "generated/data/book-keyword-body.bin";
    }

    namespace action_constants {
        constexpr inline int action_description_len         = 100;

        const inline std::string &action_history_name       = "generated/data/action-history.bin";
        const inline std::string &transaction_history_name  = "generated/data/transaction-history.bin";
    }

    namespace global_constants {
        const inline std::string blocklist_global_name      = "generated/data/blocklist-global.bin";
        const inline std::string uid_global_name            = "generated/data/uid-global.bin";
    }

    namespace log_constants {
        constexpr Logger::level log_level                   = Logger::level::DEBUG;
        inline std::ostream& logger_use_ostream             = std::cerr;
        const inline std::string log_name                   = "generated/logs/norbs_nook_logging.log";
    }

    namespace validation_constants {
        const inline std::regex regex_valid_userid             (R"(\w{1,30})");
        const inline std::regex regex_valid_password           (R"(\w{1,30})");
        const inline std::regex regex_valid_username           (R"(\S{1,30})");
        const inline std::regex regex_valid_isbn               (R"(\S{1,20})");
        const inline std::regex regex_valid_book_name          (R"([^\s\"]{1,60})");
        const inline std::regex regex_valid_author             (R"([^\s\"]{1,60})");
        const inline std::regex regex_valid_keyword            (R"([^\s\"]{1,60})");
        const inline std::regex regex_valid_price              (R"(\d{1,10}\.\d{2})");

        const inline std::regex regex_exit                     (R"(^\ *quit\ *$|^\ *exit\ *$)");
        const inline std::regex regex_su_without_password      (R"(^\ *su\ +(\w{1,30})\ *$)");
        const inline std::regex regex_su_with_password         (R"(^\ *su\ +(\w{1,30})\ +(\w{1,30})\ *$)");
        const inline std::regex regex_logout                   (R"(^\ *logout\ *$)");
        const inline std::regex regex_register                 (R"(^\ *register\ +(\w{1,30})\ +(\w{1,30})\ +(\S{1,30})\ *$)");
        const inline std::regex regex_passwd_without_password  (R"(^\ *passwd\ +(\w+)\ +(\w+)\ *$)");
        const inline std::regex regex_passwd_with_password     (R"(^\ *passwd\ +(\w+)\ +(\w+)\ +(\w+)\ *$)");
        const inline std::regex regex_useradd                  (R"(^\ *useradd\ +(\w{1,30})\ +(\w{1,30})\ +([137])\ +(\S{1,30})\ *$)");
        const inline std::regex regex_delete                   (R"(^\ *delete\ +(\w{1,30})\ *$)");
        const inline std::regex regex_show_all                 (R"(^\ *show\ *$)");
        const inline std::regex regex_show_isbn                (R"(^\ *show\ +-ISBN=(\S{1,20})\ *$)");
        const inline std::regex regex_show_name                (R"(^\ *show\ +-name=([^\s\"]{1,60})\ *$)");
        const inline std::regex regex_show_author              (R"(^\ *show\ +-author=([^\s\"]{1,60})\ *$)");
        const inline std::regex regex_show_keyword             (R"(^\ *show\ +-keyword=([^\s\"]{1,60})\ *$)");
        const inline std::regex regex_buy                      (R"(^\ *buy\ +(\S{1,20})\ +(\d{1,10})\ *$)");
        const inline std::regex regex_select                   (R"(^\ *select\ +(\S{1,20})\ *$)");
        const inline std::regex regex_modify                   (R"(^\ *modify\ +(.*?)\ *$)");
        const inline std::regex regex_import                   (R"(^\ *import\ +(\d{1,10})\ +(\d{1,10}\.\d{2})\ *$)");
        const inline std::regex regex_show_finance_all         (R"(^\ *show\ +finance\ *$)");
        const inline std::regex regex_show_finance_selective   (R"(^\ *show\ +finance\ +(\d{1,10})\ *$)");
        const inline std::regex regex_report_finance           (R"(^\ *report\ +finance\ *$)");
        const inline std::regex regex_report_employee          (R"(^\ *report\ +employee\ *$)");
        const inline std::regex regex_log                      (R"(^\ *log\ *$)");
    }
}