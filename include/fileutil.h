#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <string>

namespace FileUtil
{
  // Function to help get the basename of a file from its path
  inline std::string getBaseName(const std::string &filePath);
};

#endif // FILEUTIL_H