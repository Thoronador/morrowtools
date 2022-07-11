/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021, 2022  Dirk Stolle

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
  if ((op == "extract-all") || (op == "extract"))
    return Operation::ExtractAll;
  if (op == "extract-file")
    return Operation::ExtractFile;
  if (op == "extract-folder")
    return Operation::ExtractFolder;
  if (op == "help")
    return Operation::Help;
  if (op == "list")
    return Operation::List;
  if (op == "folders")
    return Operation::Folders;
  if (op == "info")
    return Operation::Info;
  if (op == "commands")
    return Operation::Commands;
  if (op == "metadata")
    return Operation::Metadata;
  if (op == "directory-metadata")
    return Operation::DirectoryMetadata;

  return std::nullopt;
}

std::string operationToString(const Operation op)
{
  switch (op)
  {
    case Operation::ExtractAll:
         return "extract-all";
    case Operation::ExtractFile:
         return "extract-file";
    case Operation::ExtractFolder:
         return "extract-folder";
    case Operation::Folders:
         return "folders";
    case Operation::Help:
         return "help";
    case Operation::List:
         return "list";
    case Operation::Info:
         return "info";
    case Operation::Commands:
         return "commands";
    case Operation::Metadata:
         return "metadata";
    case Operation::DirectoryMetadata:
         return "directory-metadata";
    default:
         return "";
  }
}

} // namespace
