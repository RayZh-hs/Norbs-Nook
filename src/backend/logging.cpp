#include "logging.hpp"

#include <utility>

#include "norb_utils.hpp"
#include "account.hpp"

namespace norb {
    Logger::Logger(const level level_, const std::string &f_log_name_, std::ostream &os_)
        : screen_log_level(level_), os(os_) {
        f_log_name = f_log_name_;
        utils::fAssert(f_log_name_);
        f_log.open(f_log_name_, file_open_mode);
        assert(f_log.good());
    }

    Logger::~Logger() {
        assert(f_log.good());
        f_log.close();
    }

    const std::unordered_map<Logger::level, std::string> Logger::level_name = {
        {Logger::level::DEBUG, "DEBUG"}, {Logger::level::INFO, "INFO"}, {Logger::level::WARNING, "WARNING"},
        {Logger::level::ERROR, "ERROR"}, {Logger::level::NONE, "NONE"},
    };

    void Logger::SetScreenDebugLevel(const level new_level) {
        screen_log_level = new_level;
    }

    template<typename T_>
    void Logger::Log(const level level_, const T_ &msg) {
        assert(f_log.good());
        f_log.seekp(0, std::ios::end);
        assert(f_log.good());
        f_log << '[' << Logger::level_name.find(level_)->second << ']' << ' ' << msg << std::endl;
        if (screen_log_level <= level_)
            os << '[' << Logger::level_name.find(level_)->second << ']' << ' ' << msg << std::endl;
    }

    void Logger::Log(Logger::level level_, std::string msg) {
        Logger::Log(level_, msg.c_str());
    }

    void Logger::Log(const level level_, const char *msg) {
        assert(f_log.good());
        f_log.seekp(0, std::ios::end);
        assert(f_log.good());
        f_log << '[' << Logger::level_name.find(level_)->second << ']' << ' ' << msg << std::endl;
        if (screen_log_level <= level_)
            os << '[' << Logger::level_name.find(level_)->second << ']' << ' ' << msg << std::endl;
    }

    // template<typename T_>
    // // std::enable_if_t<!std::is_same_v<std::decay<T_>, const char*>, Logger &>
    // Logger &
    // operator<<(Logger &logger, const T_ &val) {
    //     logger.Log(logger.cur_level, val);
    //     return logger;
    // }

    // template<typename T_>
    // Logger &operator <<(Logger &logger, T_ val) {
    //     logger.Log(logger.cur_level, val);
    //     return logger;
    // }

    // Logger &operator<<(Logger &logger, std::string val) {
    //     logger.Log(logger.cur_level, std::move(val));
    //     return logger;
    // }
    //
    // Logger &operator<<(Logger &logger, const char* val) {
    //     logger.Log(logger.cur_level, val);
    //     return logger;
    // }

    // Explicit Instantiation of the classes for the code to work.
    // Shout out to Gemini :) such a cool trick.
    template void Logger::Log<norb::Account>(level l, const norb::Account& val);
    template void Logger::Log<long long>(level l, const long long& val);
    template void Logger::Log<int>(level l, const int& val);
    template void Logger::Log<char>(level l, const char& val);

    void Logger::PrintAll(level level_) {
        assert(f_log.good());
        f_log.seekg(0, std::ios::beg);
        assert(f_log.good());
        std::string line;
        while (std::getline(f_log, line)) {
            if (line.empty())
                continue;
            if (WhichLevel(line) >= level_)
                os << line << std::endl;
        }
        f_log.clear();
        assert(f_log.good());
    }

    void Logger::PrintAll() {
        PrintAll(screen_log_level);
    }

    std::vector<std::string> Logger::GetAll() {
        assert(f_log.good());
        f_log.seekg(0, std::ios::beg);
        assert(f_log.good());
        std::string line;
        std::vector<std::string> ret;
        while (std::getline(f_log, line)) {
            ret.push_back(line);
        }
        f_log.clear();
        assert(f_log.good());
        return ret;
    }

    void Logger::ClearAll() {
        utils::trunc(f_log, f_log_name, file_open_mode);
    }

    Logger::level Logger::WhichLevel(const std::string &msg) {
        assert(msg.size() >= 3);
        switch (msg[1]) {
            case 'E':
                return Logger::level::ERROR;
            case 'W':
                return Logger::level::WARNING;
            case 'I':
                return Logger::level::INFO;
            case 'D':
                return Logger::level::DEBUG;
            case 'N':
                return Logger::level::NONE;
            default:
                throw std::runtime_error("logging.cpp: Undefined Logging Mode!");
        }
    }
}
