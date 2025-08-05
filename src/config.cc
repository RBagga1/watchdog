#include "config.h"
#include <fstream>
#include <stdexcept>

WatcherConfig loadConfig(const std::filesystem::path &configFilePath)
{
  // Validate config file
  if (!std::filesystem::exists(configFilePath))
  {
    throw std::runtime_error("Configuration file does not exist: " + configFilePath.string());
  }
  if (configFilePath.extension() != ".json")
  {
    throw std::runtime_error("Configuration file must be a JSON file: " + configFilePath.string());
  }

  // Parse json file
  std::ifstream configFile(configFilePath.string());
  json data = json::parse(configFile);

  // Return constructed WatcherConfig
  if (!data.contains("scanIntervalSeconds") || !data.contains("ignoreDirs") ||
      !data.contains("ignoreFileTypes") || !data.contains("pathToWatch") ||
      !data.contains("commandToExecute") || !data.contains("logFilePath") ||
      !data.contains("loggerName"))
  {
    throw std::runtime_error("Configuration file is missing required fields.");
  }

  return WatcherConfig{
      data["scanIntervalSeconds"].get<time_t>(),
      data["ignoreDirs"].get<std::unordered_set<std::string>>(),
      data["ignoreFileTypes"].get<std::unordered_set<std::string>>(),
      data["pathToWatch"].get<std::filesystem::path>(),
      data["commandToExecute"].get<std::string>(),
      data["logFilePath"].get<std::string>(),
      data["loggerName"].get<std::string>(),
  };
}
