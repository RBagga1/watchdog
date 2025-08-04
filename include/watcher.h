#ifndef WATCHER_H
#define WATCHER_H

#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <thread>
#include "logger.h"

const std::string LOGGER_NAME = "Watchdog";
const std::filesystem::path LOG_FILE_PATH = "logs/watcher_log.log";
const std::unordered_set<std::string> IGNORE_DIRS = {
    ".git",
    ".vscode",
    "build",
    "logs",
};
const std::unordered_set<std::string> IGNORE_FILE_TYPES = {
    ".log",
    ".tmp",
    ".o",
    ".a",
    ".so",
    ".d",
    ".gitmodules",
    ".gitignore",
    ".git",
};

class Watcher
{
public:
  Watcher(const std::filesystem::path &pathToWatch);
  ~Watcher();

  void startWatching();
  void stopWatching();
  // Helper methods
  void handleFileChange_(const std::filesystem::path &filePath);
  void debugMap_();

private:
  // Member variables
  std::filesystem::path watchPath;
  Logger logger;
  std::unordered_map<std::string, std::filesystem::file_time_type> fileNameToLastEditTimeMap;
  const std::string EXECUTE_CMD;

  // Watching thread and related variables
  std::thread watchThread_;
  bool doneWatching_{false};

  // Helper methods
  void watch_();
  void scanOnce_();
};

#endif // WATCHER_H