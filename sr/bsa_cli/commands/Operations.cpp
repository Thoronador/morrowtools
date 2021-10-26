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

#include "Operations.hpp"

namespace SRTP
{

std::optional<Operation> parseOperation(const std::string_view op)
{
  if (op == "list")
    return Operation::List;
  if (op == "info")
    return Operation::Info;
  if (op == "commands")
    return Operation::Commands;

  return std::nullopt;
}

std::string operationToString(const Operation op)
{
  switch (op)
  {
    case Operation::List:
         return "list";
    case Operation::Info:
         return "info";
    case Operation::Commands:
         return "commands";
    default:
         return "";
  }
}

std::array<Operation, 3> allOperations()
{
  return {
    Operation::Commands,
    Operation::Info,
    Operation::List
  };
}

} // namespace
