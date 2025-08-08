#include "system.h"

std::string executeCommand(const std::string &command)
{
  std::array<char, BUFFER_SIZE> buffer;
  std::string result;

  // Open a pipe to the command
  std::unique_ptr<FILE, PipeDeleter> pipe(popen(command.c_str(), "r"));

  if (!pipe)
  {
    throw std::runtime_error("Failed to open pipe");
  }

  // Read the output from the command
  while (size_t bytesRead = fread(buffer.data(), sizeof(char), buffer.size(), pipe.get()))
  {
    result.append(buffer.data(), bytesRead);
  }

  return result;
}
