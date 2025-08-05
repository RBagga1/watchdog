#ifndef WATCHER_H
#define WATCHER_H

#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <thread>
#include "logger.h"
#include "config.h"

class Watcher
{
public:
  Watcher(WatcherConfig config,
          LogLevel minimumLogLevel = LogLevel::INFO);
  ~Watcher();

  void startWatching();
  void stopWatching();
  // Helper methods

private:
  // Member variables
  const WatcherConfig config_;
  Logger logger;
  std::unordered_map<std::string, std::filesystem::file_time_type> fileNameToLastEditTimeMap;

  // Watching thread and related variables
  std::thread watchThread_;
  bool doneWatching_{false};
  void executeCommand_();

  // Helper methods
  void watch_();
  void scanOnce_();
};

#endif // WATCHER_H