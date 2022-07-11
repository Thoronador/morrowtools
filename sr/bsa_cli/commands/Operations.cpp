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
  if (op == "extract-directory")
    return Operation::ExtractDirectory;
  if (op == "help")
    return Operation::Help;
  if (op == "list")
    return Operation::List;
  if (op == "directories")
    return Operation::Directories;
  if (op == "info")
    return Operation::Info;
  if (op == "commands")
    return Operation::Commands;
  if (op == "file-metadata")
    return Operation::FileMetadata;
  if (op == "directory-metadata")
    return Operation::DirectoryMetadata;

  return std::nullopt;
}

std::string operationToString(const Operation op)
{
  switch (op)
  {
    case Operation::Directories:
         return "directories";
    case Operation::ExtractAll:
         return "extract-all";
    case Operation::ExtractFile:
         return "extract-file";
    case Operation::ExtractDirectory:
         return "extract-directory";
    case Operation::Help:
         return "help";
    case Operation::List:
         return "list";
    case Operation::Info:
         return "info";
    case Operation::Commands:
         return "commands";
    case Operation::FileMetadata:
         return "file-metadata";
    case Operation::DirectoryMetadata:
         return "directory-metadata";
    default:
         return "";
  }
}

} // namespace
