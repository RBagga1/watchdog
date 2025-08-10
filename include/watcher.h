#ifndef WATCHER_H
#define WATCHER_H

#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <thread>
#include "logger.h"
#include "config.h"
#include "system.h"

class Watcher
{
public:
  Watcher(WatcherConfig config,
          std::filesystem::path pathToWatch,
          LogLevel minimumLogLevel = LogLevel::INFO);
  ~Watcher();

  void startWatching();
  void stopWatching();
  // Helper methods

private:
  // Member variables
  const WatcherConfig config_;
  const std::filesystem::path pathToWatch_;
  Logger logger;
  std::unordered_map<std::string, std::filesystem::file_time_type> fileNameToLastEditTimeMap;
  bool initialScanDone_{false};

  // Watching thread and related variables
  std::thread watchThread_;
  bool doneWatching_{false};
  void executeCommand_();
  std::mutex mutex_;

  // Helper methods
  void watch_();
  void scanOnce_();
};

#endif // WATCHER_H