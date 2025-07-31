#ifndef WATCHER_H
#define WATCHER_H

#include <filesystem>
#include <unordered_map>
#include <string>
#include <thread>
#include "logger.h"

const std::string LOGGER_NAME = "Watchdog";
const std::filesystem::path LOG_FILE_PATH = "logs/watcher_log.txt";

class Watcher
{
public:
  Watcher(const std::filesystem::path &pathToWatch);
  ~Watcher();

  void startWatching();
  void stopWatching();

private:
  // Member variables
  std::filesystem::path watchPath;
  Logger logger;
  std::unordered_map<std::string, std::filesystem::file_time_type> fileNameToLastEditTimeMap;
  const std::string EXECUTE_CMD;

  // Watching thread and related variables
  std::thread watchThread_;
  bool doneWatching_{false};
  void watch_();
  void scanOnce_();
};

#endif // WATCHER_H