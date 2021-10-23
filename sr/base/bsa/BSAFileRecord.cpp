/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2021  Thoronador

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

#include "BSAFileRecord.hpp"
#include <iostream>

namespace SRTP
{

BSAFileRecord::BSAFileRecord()
: nameHash(0),
  fileSize(0),
  offset(0),
  fileName("")
{
}

bool BSAFileRecord::loadFromStream(std::istream& input)
{
  if (!input.good())
  {
    std::cerr << "BSAFileRecord::loadFromStream: Error: Bad stream given!\n";
    return false;
  }
  // read file record's stuff
  input.read(reinterpret_cast<char*>(&nameHash), sizeof(BSAHash));
  if (!input.good())
  {
    std::cerr << "BSAFileRecord::loadFromStream: Error: Could not read hash!\n";
    return false;
  }
  input.read(reinterpret_cast<char*>(&fileSize), 4);
  input.read(reinterpret_cast<char*>(&offset), 4);
  if (!input.good())
  {
    std::cerr << "BSAFileRecord::loadFromStream: Error while reading data!\n";
    return false;
  }
  // File name is at another location in the file, so it cannot be set here.
  fileName.clear();
  // All is well.
  return true;
}

bool BSAFileRecord::isCompressionToggled() const
{
  return (fileSize & (1 << 30)) != 0;
}

uint32_t BSAFileRecord::getRealFileSize() const
{
  return fileSize & ~(1 << 30);
}

} // namespace
