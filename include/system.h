#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <cstdio>
#include <array>
#include <memory>
#include <iostream>
#include <sys/wait.h>

#define BUFFER_SIZE 4096

/** RAII wrapper for closing pipes */
struct PipeDeleter
{
  int *exitCodePtr = nullptr;

  void operator()(FILE *pipe) const
  {
    if (!pipe)
    {
      return;
    }

    int status = pclose(pipe);

    if (exitCodePtr)
    {
      *exitCodePtr = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }

    if (status == -1 || !WIFEXITED(status) || WEXITSTATUS(status) != 0)
    {
      throw std::runtime_error("Failed to close pipe or command execution failed.");
    }
  }
};

namespace my_system
{
  /**
   * Holds shell command execution result.
   */
  struct CommandResult
  {
    int exitCode;
    std::string stdout;
  };

  /**
   * Executes a shell command and returns its output as a string.
   * @param command The command to execute.
   * @return CommandResult containing the exit code and standard output.
   * @throws std::runtime_error if the command fails to execute or read.
   */
  CommandResult executeCommand(const std::string &command);
};
#endif // SYSTEM_H