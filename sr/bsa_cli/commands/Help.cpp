/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "Help.hpp"
#include <iostream>
#include "../../base/ReturnCodes.hpp"
#include "CommandFactory.hpp"
#include "Operations.hpp"

namespace SRTP::bsa_cli
{

Help::Help()
: executableName(std::string()),
  cmd(std::nullopt)
{
}

int Help::parseArguments(int argc, char** argv)
{
  if (argv == nullptr)
  {
    std::cerr << "Error: Parameter array pointer is NULL.\n";
    return SRTP::rcInvalidParameter;
  }
  if (argv[0] == nullptr || argv[0][0] == '\0')
  {
    std::cerr << "Error: Executable name is empty.\n";
    return SRTP::rcInvalidParameter;
  }
  executableName = std::string(argv[0]);

  for(int i = 2; i < argc; ++i)
  {
    if (argv[i] != nullptr)
    {
      if (cmd.has_value())
      {
        std::cerr << "Error: The help command only expects one parameter, not more.\n";
        return SRTP::rcInvalidParameter;
      }
      const std::string param = std::string(argv[i]);
      cmd = parseOperation(param);
      // If cmd has no value, then parsing failed.
      if (!cmd.has_value())
      {
        std::cerr << "Error: " << param << " is no known command.\n"
                  << "Run '" << executableName << " commands' to get a list of available commands.\n";
        return SRTP::rcInvalidParameter;
      }
    }
    else
    {
      std::cerr << "Error: Parameter at index " << i << " is NULL.\n";
      return SRTP::rcInvalidParameter;
    }
  }
  if (!cmd.has_value())
  {
    std::cerr << "Error: A command has to be specified to show the help, e. g. '"
              << executableName << " help list' to show the help for the list"
              << " command.\n";
    return SRTP::rcInvalidParameter;
  }

  return 0;
}

int Help::run()
{
  auto command = CommandFactory::createCommand(cmd.value());
  std::cout << command->helpFull("bsa-cli");
  return 0;
}

std::string Help::helpShort() const
{
  return "Shows help for a command.";
}

std::string Help::helpFull(const std::string_view binaryName) const
{
  return std::string(binaryName).append(" help\n")
      .append("Shows help for a command.\n\n")
      .append("Usage:\n    ")
      .append(binaryName).append(" help [command]\n\n")
      .append("Options:\n    command    - The command whose help message shall be displayed.\n");
}

} // namespace
