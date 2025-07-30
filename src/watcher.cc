#include "watcher.h"
#include "fileutil.h"

Watcher::Watcher(const std::filesystem::path &pathToWatch) : watchPath(pathToWatch), logger(LOGGER_NAME, LOG_FILE_PATH)
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
