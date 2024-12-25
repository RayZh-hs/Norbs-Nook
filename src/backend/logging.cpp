#include "logging.hpp"

#include "norb_utils.hpp"

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

    template<typename T_>
    Logger &operator<<(Logger &logger, const T_ &val) {
        Log(logger.cur_level, val);
        return logger;
    }

    Logger &operator <<(Logger &logger, const Logger::level &level_) {
        logger.cur_level = level_;
        return logger;
    }

    void Logger::PrintAll(level level_) {
        assert(f_log.good());
        f_log.seekg(0, std::ios::beg);
        assert(f_log.good());
        std::string line;
        while (std::getline(f_log, line)) {
            os << line << std::endl;
        }
        f_log.clear();
        assert(f_log.good());
    }

    void Logger::PrintAll() {
        PrintAll(screen_log_level);
    }

    void Logger::ClearAll() {
        utils::trunc(f_log, f_log_name, file_open_mode);
    }
}
