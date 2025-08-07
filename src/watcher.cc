#include "watcher.h"
#include "fileutil.h"
#include <chrono>
#include <format>

Watcher::Watcher(WatcherConfig config,
                 std::filesystem::path pathToWatch,
                 LogLevel minimumLogLevel)
    : config_(config),
      pathToWatch_(pathToWatch),
      logger(LoggerBuilder()
                 .setName(config.loggerName)
                 .setLogFilePath(config.logFilePath)
                 .setMinimumLogLevel(minimumLogLevel)
                 .build())
{
  logger.debug("Watcher initialized for path: " + pathToWatch_.string());
}

Watcher::~Watcher()
{
  logger.debug("watcher destructor called for path: " + pathToWatch_.string());
  {
    doneWatching_ = true;
    logger.debug("Watcher stopped for path: " + pathToWatch_.string());
  }
}

void Watcher::executeCommand_()
{
  logger.info("Changes found during scan, executing command at time: " +
              std::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::system_clock::now()));

  // Execute the command provided
  if (!config_.commandToExecute.empty())
  {
    logger.info("Executing command: " + config_.commandToExecute);
    int result = std::system(config_.commandToExecute.c_str());
    if (result != 0)
    {
      logger.error("Command execution failed with code: " + std::to_string(result));
      return;
    }

    logger.info("Command executed successfully.");
  }
}

void Watcher::watch_()
{
  logger.info("Performing initial scan for changes in path: " + pathToWatch_.string());
  scanOnce_(); // Initial scan to populate the map

  while (!doneWatching_)
  {
    std::this_thread::sleep_for(std::chrono::seconds(config_.scanIntervalSeconds));
    scanOnce_();
  }
}

void Watcher::startWatching()
{
  if (watchThread_.joinable())
  {
    logger.warning("Watcher thread is already running.");
    return;
  }

  watchThread_ = std::thread(&Watcher::watch_, this);
  logger.info("Watcher started for path: " + pathToWatch_.string());
}

void Watcher::stopWatching()
{
  doneWatching_ = true;

  if (!watchThread_.joinable())
  {
    logger.warning("Watcher thread is not running.");
    return;
  }

  watchThread_.join();
  logger.info("Watcher stopped for path: " + pathToWatch_.string());
}

void Watcher::scanOnce_()
{
  logger.info(
      "Scanning once for changes in path: " + pathToWatch_.string() +
      " at time: " + std::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::system_clock::now()));
  bool changesFound = false;

  std::filesystem::directory_options iteratorOptions = std::filesystem::directory_options::skip_permission_denied;
  for (auto iterator_ptr = std::filesystem::recursive_directory_iterator(pathToWatch_, iteratorOptions);
       iterator_ptr != std::filesystem::recursive_directory_iterator();
       ++iterator_ptr)
  {
    const auto &entry = *iterator_ptr;
    const auto &filePath = entry.path();

    // Skip directory if in ignore list
    if (entry.is_directory() && config_.ignoreDirs.count(filePath.filename().string()))
    {
      iterator_ptr.disable_recursion_pending();
      logger.debug("Ignoring directory: " + filePath.string());
      continue;
    }

    // Scan files and check for changes in unignored directories
    if (entry.is_regular_file())
    {
      const auto &filePath = entry.path();
      if (config_.ignoreFileTypes.count(filePath.extension().string()))
      {
        logger.debug("Ignoring file: " + filePath.string());
        continue;
      }

      const auto &filePathStr = filePath.string();
      const auto lastWriteTime = std::filesystem::last_write_time(filePath);

      if (fileNameToLastEditTimeMap.find(filePathStr) == fileNameToLastEditTimeMap.end() ||
          fileNameToLastEditTimeMap[filePathStr] < lastWriteTime)
      {
        changesFound = true;
        logger.info("Change detected in file: " + filePathStr);
      }
      fileNameToLastEditTimeMap[filePathStr] = lastWriteTime;
    }
  }

  if (changesFound)
  {
    logger.info("Changes detected during scan, executing command.");
    executeCommand_();
  }
}
