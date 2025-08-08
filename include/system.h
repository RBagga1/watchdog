#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <cstdio>
#include <array>
#include <memory>
#include <iostream>

#define BUFFER_SIZE 4096

/** RAII wrapper for closing pipes */
struct PipeDeleter
{
  void operator()(FILE *pipe) const
  {
    if (!pipe)
    {
      return;
    }

    int status = pclose(pipe);

    if (status == -1 || !WIFEXITED(status) || WEXITSTATUS(status) != 0)
    {
      throw std::runtime_error("Failed to close pipe or command execution failed.");
    }
  }
};

/**
 * Executes a shell command and returns its output as a string.
 * @param command The command to execute.
 * @return The output of the command.
 * @throws std::runtime_error if the command fails to execute or read.
 */
std::string executeCommand(const std::string &command);

#endif // SYSTEM_H