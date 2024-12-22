#include <iostream>

#include "logging.hpp"

using level = norb::Logger::level;

int main() {
    norb::Logger logger(level::INFO, "log.testdata", std::cerr);

    std::cout << "Basic Test" << '\n';
    logger.Log(level::INFO, "This should be printed.");
    logger.Log(level::ERROR, "This should be printed too.");
    logger.Log(level::DEBUG, "This should NOT be printed.");

    std::cout << "File Test" << '\n';
    logger.PrintAll(level::DEBUG);
    logger.SetScreenDebugLevel(level::DEBUG);
    logger.Log(level::INFO, "This is appended to the list.");
    logger.PrintAll();

    std::cout << "Trunc Test" << '\n';
    logger.ClearAll();
    logger.PrintAll();
    logger.Log(level::INFO, "This should be the only text available.");
    logger.PrintAll();

    return 0;
}