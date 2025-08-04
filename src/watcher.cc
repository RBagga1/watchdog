#include "watcher.h"
#include "fileutil.h"

Watcher::Watcher(const std::filesystem::path &pathToWatch)
    : watchPath(pathToWatch),
      logger(LoggerBuilder()
                 .setName("Watcher")
                 .setLogFilePath(LOG_FILE_PATH)
                 .build())
{
  logger.debug("Watcher initialized for path: " + watchPath.string());
}

Watcher::~Watcher()
{
  logger.debug("watcher destructor called for path: " + watchPath.string());
  {
    doneWatching_ = true;
    logger.debug("Watcher stopped for path: " + watchPath.string());
  }
}

void Watcher::handleFileChange_(const std::filesystem::path &filePath)
{
  logger.info("File changed: " + filePath.string());
  // TODO - implement logic to handle file changes, e.g., execute a command
}

void Watcher::scanOnce_()
{
  logger.debug("Scanning once for changes in path: " + watchPath.string());
  std::filesystem::directory_options iteratorOptions = std::filesystem::directory_options::skip_permission_denied;

  for (auto iterator_ptr = std::filesystem::recursive_directory_iterator(watchPath, iteratorOptions);
       iterator_ptr != std::filesystem::end(iterator_ptr);
       ++iterator_ptr)
  {
    const auto &entry = *iterator_ptr;
    const auto &filePath = entry.path();

    // Skip directory if in ignore list
    if (entry.is_directory() && IGNORE_DIRS.count(filePath.filename().string()))
    {
      iterator_ptr.disable_recursion_pending();
      logger.debug("Ignoring directory: " + filePath.string());
      continue;
    }

    // Scan files and check for changes in unignored directories
    if (entry.is_regular_file())
    {
      const auto &filePath = entry.path();
      if (IGNORE_FILE_TYPES.count(filePath.extension().string()))
      {
        logger.debug("Ignoring file: " + filePath.string());
        continue;
      }

      const auto &filePathStr = filePath.string();
      const auto lastWriteTime = std::filesystem::last_write_time(filePath);

      if (fileNameToLastEditTimeMap.find(filePathStr) == fileNameToLastEditTimeMap.end() ||
          fileNameToLastEditTimeMap[filePathStr] < lastWriteTime)
      {
        handleFileChange_(filePath);
        fileNameToLastEditTimeMap[filePathStr] = lastWriteTime;
      }
    }
  }
}

void Watcher::debugMap_()
{
  logger.debug("Current file edit times:");
  for (const auto &pair : fileNameToLastEditTimeMap)
  {
    logger.debug("File: " + pair.first + ", Last Edit Time: " + std::format("{}", std::chrono::file_clock::to_sys(pair.second)));
  }
}