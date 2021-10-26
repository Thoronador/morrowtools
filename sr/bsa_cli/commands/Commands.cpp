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

#include "Commands.hpp"
#include <iostream>
#include "../../base/ReturnCodes.hpp"
#include "CommandFactory.hpp"
#include "Operations.hpp"

namespace SRTP::bsa_cli
{

int Commands::parseArguments(int argc, char** argv)
{
  if (argv == nullptr)
  {
    std::cerr << "Error: Parameter array pointer is NULL.\n";
    return SRTP::rcInvalidParameter;
  }

  if (argc > 2)
  {
    std::cerr << "Error: This command does not allow additional parameters.\n";
    return SRTP::rcInvalidParameter;
  }

  return 0;
}

int Commands::run()
{
  const auto all = allOperations();
  std::cout << "Available commands:\n";
  std::string::size_type maxNameLength = 0;
  for (const auto operation: all)
  {
    const auto name = operationToString(operation);
    const auto size = name.size();
    if (name.size() > maxNameLength)
      maxNameLength = size;
  }
  for (const auto operation: all)
  {
    const auto command = CommandFactory::createCommand(operation);
    const auto name = operationToString(operation);
    std::cout << "\t" << operationToString(operation)
              << std::string(maxNameLength - name.size(), ' ') << "\t"
              << command->helpShort() << "\n";
  }
  return 0;
}

std::string Commands::helpShort() const
{
  return "Lists available commands.";
}

} // namespace
