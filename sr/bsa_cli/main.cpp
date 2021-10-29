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

#include <iostream>
#include <memory>
#include <string>
#include "commands/Operations.hpp"
#include "commands/CommandFactory.hpp"
#include "commands/Info.hpp"
#include "commands/List.hpp"
#include "../base/ReturnCodes.hpp"

void showVersion()
{
  std::cout << "BSA Command Line Tool for Skyrim, version 0.4.1, 2021-10-30\n";
}

void showHelp()
{
  std::cout << "bsa-cli [OPTIONS] COMMAND BSA_FILE\n"
            << "\n"
            << "options:\n"
            << "  --help     - displays this help message and quits\n"
            << "  -?         - same as --help\n"
            << "  --version  - displays the version of the program and quits\n"
            << "  -v         - same as --version\n"
            << "  COMMAND    - sets the command that shall be performed on the BSA\n"
            << "               file. Allowed commands are:\n"
            << "                   commands     - lists available commands\n"
            << "                   extract-file - extracts a single file from the archive\n"
            << "                   help         - shows help for a command\n"
            << "                   info         - shows BSA header information\n"
            << "                   list         - lists all folders and files in the archive\n"
            << "\n"
            << "               More commands may be added in the future.\n"
            << "               The command must be given in most cases.\n"
            << "  BSA_FILE   - set path to the BSA file to operate on to BSA_FILE.\n"
            << "               The BSA_FILE must be given in most cases.\n"
            << "\n"
            << "Use `bsa-cli help <command>` for more information on a specific command.\n";
}

int main(int argc, char **argv)
{
  using namespace SRTP::bsa_cli;
  std::optional<SRTP::Operation> operation;

  if ((argc > 1) && (argv != nullptr))
  {
    int i = 1;
    while (i < argc)
    {
      if (argv[i] != nullptr)
      {
        const std::string param = std::string(argv[i]);
        // help parameter
        if ((param == "--help") || (param == "-?") || (param == "/?"))
        {
          showHelp();
          return 0;
        }
        // version information requested?
        else if ((param == "--version") || (param == "-v"))
        {
          showVersion();
          return 0;
        }
        else if (!operation.has_value())
        {
          operation = SRTP::parseOperation(param);
          if (!operation)
          {
            std::cerr << "Error: '" << param << "' is not an allowed command.\n"
                      << "Allowed commands are 'commands', 'extract-file', "
                      << "'help', list', or 'info'.\n";
            return SRTP::rcInvalidParameter;
          }
          break;
        }
        else
        {
          // unknown or wrong parameter
          std::cerr << "Invalid parameter given: \"" << param << "\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return SRTP::rcInvalidParameter;
        }
      } // parameter exists
      else
      {
        std::cerr << "Parameter at index " << i << " is NULL.\n";
        return SRTP::rcInvalidParameter;
      }
      ++i; // on to next parameter
    }
  } // if arguments present
  else
  {
    std::cerr << "You have to specify certain parameters for this program to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return SRTP::rcInvalidParameter;
  }

  if (!operation)
  {
    std::cerr << "Error: A command has to be specified!\n"
              << "Use --help to get a list of valid parameters and commands.\n";
    return SRTP::rcInvalidParameter;
  }
  auto command = CommandFactory::createCommand(operation.value());
  const int parse = command->parseArguments(argc, argv);
  if (parse != 0)
    return parse;
  return command->run();
}
