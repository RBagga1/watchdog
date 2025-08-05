#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <filesystem>
#include <unordered_set>
#include <chrono>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// This structure is used to define the parameters for the file watcher
// including the scan interval, directories to ignore, file types to ignore,
// the path to watch, the command to execute, and logging details.
typedef struct
{
  const time_t scanIntervalSeconds;                      // Interval between scans in seconds
  const std::unordered_set<std::string> ignoreDirs;      // Directories to ignore
  const std::unordered_set<std::string> ignoreFileTypes; // File types to ignore
  const std::filesystem::path pathToWatch;               // Path to watch
  const std::string commandToExecute;                    // Command to execute
  const std::string logFilePath;                         // Log file path
  const std::string loggerName;                          // Logger name
} WatcherConfig;

/**
 * Load the configuration from a JSON file.
 * It reads the file, parses the JSON content, and returns a WatcherConfig object.
 * If the file cannot be read or parsed, it throws an exception.
 */
WatcherConfig loadConfig(const std::filesystem::path &configFilePath);

#endif // CONFIG_H