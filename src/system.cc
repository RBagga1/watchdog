#include "system.h"

my_system::CommandResult my_system::executeCommand(const std::string &command)
{
  std::array<char, BUFFER_SIZE> buffer;
  std::string stdout;
  my_system::CommandResult result;

  // Open a pipe to the command
  std::unique_ptr<FILE, PipeDeleter> pipe(
      popen(command.c_str(), "r"),
      PipeDeleter{&result.exitCode});

  if (!pipe)
  {
    throw std::runtime_error("Failed to open pipe");
  }

  // Read the output from the command
  while (size_t bytesRead = fread(buffer.data(), sizeof(char), buffer.size(), pipe.get()))
  {
    stdout.append(buffer.data(), bytesRead);
  }

  result.stdout = std::move(stdout);
  return result;
}

void my_system::showNotification(const std::string &title,
                                 const std::string &message,
                                 int exitCode,
                                 const std::string &status)
{
  // Construct the command to execute the Python script
  std::stringstream command{};
  command << "python3 src/notify.py "
          << "-t \"" << title << "\" "
          << "-m \"" << message << "\" "
          << "-e " << exitCode << " "
          << "-s " << status;

  // Execute the command
  int result = std::system(command.str().c_str());
  if (result != 0)
  {
    std::cerr << "Failed to execute notification command: " << command.str() << std::endl;
  }
}
