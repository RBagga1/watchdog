#include <iostream>
#include <filesystem>
#include <string>
#include "Logger.h"
#include "watcher.h"

namespace fs = std::filesystem;

int main()
{
    const std::filesystem::path pathToWatch = fs::current_path();
    Watcher watcher(pathToWatch);
    return 0;
}
