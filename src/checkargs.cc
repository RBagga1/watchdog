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
      if (i + 1 < args.size())
      {
        // Check if the next argument is another flag
        if (args[i + 1].starts_with("-"))
        {
          printErrorAndUsage("No path provided after --path or -p option.");
          return ArgParseResult::ERROR;
        }

        // Valid string provided
        *pathToWatch = args[++i];
        if (!std::filesystem::exists(*pathToWatch))
        {
          printErrorAndUsage("Provided path does not exist: " + *pathToWatch);
          return ArgParseResult::ERROR;
        }
      }
    }
    else if (args[i] == "--config" || args[i] == "-c")
    {
      if (i + 1 < args.size())
      {
        // Check if the next argument is another flag
        if (args[i + 1].starts_with("-"))
        {
          printErrorAndUsage("No config file provided after --config or -c option.");
          return ArgParseResult::ERROR;
        }

        // Valid string provided
        *configFilePath = args[++i];
        if (!std::filesystem::exists(*configFilePath))
        {
          printErrorAndUsage("Provided config file does not exist: " + *configFilePath);
          return ArgParseResult::ERROR;
        }
      }
    }
  }
  // If no path for path to watch was provided, return an error
  if (pathToWatch->empty())
  {
    printErrorAndUsage("No path provided after --path or -p option.");
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
