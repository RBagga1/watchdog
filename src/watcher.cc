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
