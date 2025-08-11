#ifndef CHECKARGS_H
#define CHECKARGS_H

#include <vector>
#include <string>
#include "logger.h"

enum class ArgParseResult
{
  SUCCESS,
  HELP_REQUESTED,
  ERROR
};

ArgParseResult validateArgs(
    std::vector<std::string> &args,
    std::string *pathToWatch,
    std::string *configFilePath,
    LogLevel *logLevel);

void printUsage();

void printErrorAndUsage(const std::string &errorMessage);

int parseFilepathFlag(std::vector<std::string> &args, int currentIndex, std::string *filepath);

#endif // CHECKARGS_H
