#pragma once

#include <ostream>
#include <string>

namespace norb::con {
    const inline std::string clear            = "\033[0m"   ;
    const inline std::string colorBlack      = "\033[0;30m";
    const inline std::string colorRed        = "\033[0;31m";
    const inline std::string colorGreen      = "\033[0;32m";
    const inline std::string colorYellow     = "\033[0;33m";
    const inline std::string colorBlue       = "\033[0;34m";
    const inline std::string colorPurple     = "\033[0;35m";
    const inline std::string colorCyan       = "\033[0;36m";
    const inline std::string colorWhite      = "\033[0;37m";
    const inline std::string boldBlack       = "\033[1;30m";
    const inline std::string boldRed         = "\033[1;31m";
    const inline std::string boldGreen       = "\033[1;32m";
    const inline std::string boldYellow      = "\033[1;33m";
    const inline std::string boldBlue        = "\033[1;34m";
    const inline std::string boldPurple      = "\033[1;35m";
    const inline std::string boldCyan        = "\033[1;36m";
    const inline std::string boldWhite       = "\033[1;37m";
    const inline std::string underlineBlack  = "\033[4;30m";
    const inline std::string underlineRed    = "\033[4;31m";
    const inline std::string underlineGreen  = "\033[4;32m";
    const inline std::string underlineYellow = "\033[4;33m";
    const inline std::string underlineBlue   = "\033[4;34m";
    const inline std::string underlinePurple = "\033[4;35m";
    const inline std::string underlineCyan   = "\033[4;36m";
    const inline std::string underlineWhite  = "\033[4;37m";
}