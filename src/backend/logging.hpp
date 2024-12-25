#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

#include "norb_utils.hpp"

namespace norb {
    class Logger {
    public:
        class EOS_t_ {};

        enum class level {
            DEBUG = 10, INFO = 20, WARNING = 30, ERROR = 40, NONE = 50,
        };

        Logger(level level_, const std::string &f_log_name_, std::ostream &os_);

        ~Logger();

        void SetScreenDebugLevel(level new_level);

        template<typename T_>
        void Log(level level_, const T_ &msg);

        void Log(level level_, std::string msg);

        void Log(level level_, const char *msg);

        template<typename T_>
        // std::enable_if_t<!std::is_same_v<std::decay<T_>, const char*>, Logger &>
        friend Logger &operator <<(Logger &logger, const T_ &val) {
            // logger.Log(logger.cur_level, val);
            logger.oss << val;
            assert(logger.oss.good());
            return logger;
        }

        // template <typename T_>
        // friend Logger& operator << (Logger &logger, T_ val);

        // friend Logger& operator << (Logger &logger, std::string val);

        // friend Logger& operator << (Logger &logger, const char* val);

        friend Logger &operator <<(Logger &logger, const level &level_) {
            logger.cur_level = level_;
            return logger;
        }

        friend Logger &operator <<(Logger &logger, const EOS_t_ &) {
            logger.Log(logger.cur_level, logger.oss.str());
            logger.oss.str("");
            logger.oss.clear();
            return logger;
        }

        void PrintAll(level level_);

        void PrintAll();

        std::vector<std::string> GetAll();

        void ClearAll();

        static level WhichLevel(const std::string &msg);

        // End of stream marker
        static EOS_t_ EOS;

    private:
        static constexpr auto file_open_mode = std::ios::in | std::ios::out;

        static const std::unordered_map<level, std::string> level_name;

        level cur_level = level::NONE;
        level screen_log_level = level::DEBUG;
        std::string f_log_name;
        std::fstream f_log;
        std::ostream &os;
        std::ostringstream oss;
    };

    // Definition must be outside the class for a static member.
    inline Logger::EOS_t_ Logger::EOS;
}
