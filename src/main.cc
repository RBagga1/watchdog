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
    std::filesystem::path pathToWatch;
    std::vector<std::string> args(argv + 1, argv + argc);
    for (int i = 0; i < args.size(); i++)
    {
        if (args[i] == "--debug")
        {
            logLevel = LogLevel::DEBUG;
        }
        else if (args[i] == "--path" || args[i] == "-p")
        {
            if (i + 1 < args.size())
            {
                if (args[i + 1].starts_with("-"))
                {
                    std::cerr << "Error: No path provided after --path or -p option." << std::endl;
                    return 1;
                }

                pathToWatch = fs::path(args[++i]);
                if (!fs::exists(pathToWatch))
                {
                    std::cerr << "Error: Path to watch does not exist: " << pathToWatch << std::endl;
                    return 1;
                }
            }
            else
            {
                std::cerr << "Error: No path provided after --path or -p option." << std::endl;
                return 1;
            }
        }
    }

    if (pathToWatch.empty())
    {
        std::cerr << "Error: No path to watch specified. Use --path or -p option." << std::endl;
        return 1;
    }

    try
    {
        Watcher watcher(
            loadConfig("config.json"),
            pathToWatch,
            logLevel);

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
