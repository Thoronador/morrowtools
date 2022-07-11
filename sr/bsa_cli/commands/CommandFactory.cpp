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

#include "CommandFactory.hpp"
#include "Commands.hpp"
#include "DirectoryMetadata.hpp"
#include "ExtractAll.hpp"
#include "ExtractFile.hpp"
#include "ExtractFolder.hpp"
#include "Folders.hpp"
#include "Help.hpp"
#include "Info.hpp"
#include "List.hpp"
#include "Metadata.hpp"

namespace SRTP::bsa_cli::CommandFactory
{

std::unique_ptr<Command> createCommand(const Operation op)
{
  switch (op)
  {
    case Operation::ExtractAll:
         return std::unique_ptr<Command>(new ExtractAll());
    case Operation::ExtractFile:
         return std::unique_ptr<Command>(new ExtractFile());
    case Operation::ExtractFolder:
         return std::unique_ptr<Command>(new ExtractFolder());
    case Operation::List:
         return std::unique_ptr<Command>(new List());
    case Operation::Folders:
         return std::unique_ptr<Command>(new Folders());
    case Operation::Info:
         return std::unique_ptr<Command>(new Info());
    case Operation::Commands:
         return std::unique_ptr<Command>(new Commands());
    case Operation::Metadata:
         return std::unique_ptr<Command>(new Metadata());
    case Operation::DirectoryMetadata:
         return std::unique_ptr<Command>(new DirectoryMetadata());
    case Operation::Help:
         return std::unique_ptr<Command>(new Help());
    default:
         return nullptr;
  }
}

} // namespace
