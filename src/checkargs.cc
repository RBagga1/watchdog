#include "checkargs.h"

ArgParseResult validateArgs(
    std::vector<std::string> &args,
    std::string *pathToWatch,
    std::string *configFilePath,
    LogLevel *logLevel)
{
  for (int i = 0; i < args.size(); i++)
  {
    if (args[i] == "--debug" || args[i] == "-d")
    {
      *logLevel = LogLevel::DEBUG;
    }
    else if (args[i] == "--help" || args[i] == "-h")
    {
      printUsage();
      return ArgParseResult::HELP_REQUESTED;
    }
    else if (args[i] == "--path" || args[i] == "-p")
    {
      if (parseFilepathFlag(args, i, pathToWatch) == -1)
      {
        return ArgParseResult::ERROR;
      }
    }
    else if (args[i] == "--config" || args[i] == "-c")
    {
      if (parseFilepathFlag(args, i, configFilePath) == -1)
      {
        return ArgParseResult::ERROR;
      }
    }
  }
  // If no path for path to watch was provided, return an error
  if (pathToWatch->empty())
  {
    printErrorAndUsage("Required argument --path or -p is missing.");
    return ArgParseResult::ERROR;
  }

  return ArgParseResult::SUCCESS;
}

void printErrorAndUsage(const std::string &errorMessage)
{
  std::cerr << "Error: " << errorMessage << std::endl;
  printUsage();
}

void printUsage()
{
  std::cout << "Usage: watchdog -p <path> [options]\n"
               "Monitors a directory for file system changes.\n\n"
               "Required:\n"
               "  -p, --path <path>    Specify the path to watch\n"
               "Options:\n"
               "  -c, --config <file>  Specify the config file to use\n"
               "  -d, --debug          Enable debug logging\n"
               "  -h, --help           Show this help message\n";
}

int parseFilepathFlag(std::vector<std::string> &args, int currentIndex, std::string *filepath)
{
  if ((currentIndex + 1) >= args.size())
  {
    printErrorAndUsage("No filepath provided for " + args[currentIndex]);
    return -1;
  }

  // Check if the next arg is another flag
  if (args[currentIndex + 1].starts_with("-"))
  {
    printErrorAndUsage("No path provided after " + args[currentIndex] + " option.");
    return -1;
  }

  // Valid string provided
  *filepath = args[++currentIndex];
  if (!std::filesystem::exists(*filepath))
  {
    printErrorAndUsage("Provided path does not exist: " + *filepath);
    return -1;
  }

  return 0;
}
