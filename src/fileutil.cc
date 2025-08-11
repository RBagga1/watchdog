#include "fileutil.h"
#include <filesystem>

std::string FileUtil::getBaseName(const std::string &filePath)
{
  // Use std::filesystem to get the base name of the file
  std::filesystem::path path(filePath);
  return path.filename().string();
}