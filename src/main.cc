#include <iostream>
#include <filesystem>
#include <string>
#include "logger.h"
#include "watcher.h"
#include "config.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    LogLevel logLevel = LogLevel::INFO;
    std::vector<std::string> args(argv + 1, argv + argc);
    for (const auto &arg : args)
    {
        if (arg == "--debug" || arg == "-d")
        {
            logLevel = LogLevel::DEBUG;
        }
    }

    try
    {
        Watcher watcher(loadConfig("config.json"), logLevel);

        watcher.startWatching();

        std::cout << "Press Enter to stop watching..." << std::endl;
        std::cin.get();

        watcher.stopWatching();
        std::cout << "Watcher stopped." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Watchdog exiting successfully." << std::endl;
    return 0;
}
