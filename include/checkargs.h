#ifndef CHECKARGS_H
#define CHECKARGS_H

#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include "logger.h"

enum class ArgParseResult
{
  SUCCESS,
  HELP_REQUESTED,
  ERROR
};

ArgParseResult validateArgs(
    std::vector<std::string> &args,
    std::string &pathToWatch,
    LogLevel &logLevel);

void printUsage();

void printErrorAndUsage(const std::string &errorMessage);

#endif // CHECKARGS_H
