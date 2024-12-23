#pragma once

#include <string>
#include <unordered_map>

#include "norb_utils.hpp"

namespace norb {
    class Logger {
    public:
        enum class level {
            DEBUG = 10, INFO = 20, WARNING = 30, ERROR = 40, NONE = 50,
        };

        Logger(level level_, const std::string &f_log_name_, std::ostream &os_);

        ~Logger();

        void SetScreenDebugLevel(level new_level);

        void Log(level level_, const std::string &msg);

        void PrintAll(level level_);
        void PrintAll();

        void ClearAll();

    private:

        static constexpr auto file_open_mode = std::ios::in | std::ios::out;

        static const std::unordered_map<level, std::string> level_name;

        level screen_log_level = level::DEBUG;
        std::string f_log_name;
        std::fstream f_log;
        std::ostream &os;
    };
}
