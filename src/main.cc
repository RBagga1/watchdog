#include <iostream>
#include <filesystem>
#include <string>
#include "logger.h"
#include "watcher.h"

namespace fs = std::filesystem;

int main()
{
    const std::filesystem::path pathToWatch = fs::current_path();
    const std::string cmd = "cmake --build build";
    try
    {
        Watcher watcher(pathToWatch, cmd);

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

    std::cout << "Testing the build watchdog!" << std::endl;
    std::cout << "Exiting program." << std::endl;
    return 0;
}
