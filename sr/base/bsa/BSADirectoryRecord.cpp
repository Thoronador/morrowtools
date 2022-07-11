/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2021, 2022  Dirk Stolle

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

#include "BSADirectoryRecord.hpp"
#include <iostream>

namespace SRTP
{

BSADirectoryRecord::BSADirectoryRecord()
: nameHash(0),
  count(0),
  offset(0)
{
}

bool BSADirectoryRecord::loadFromStream(std::istream& input, const uint32_t bsaVersion)
{
  if (!input.good())
  {
    std::cerr << "BSADirectoryRecord::loadFromStream: Error: Bad stream given!\n";
    return false;
  }
  // read directory record's stuff
  input.read(reinterpret_cast<char*>(&nameHash), sizeof(BSAHash));
  if (!input.good())
  {
    std::cerr << "BSADirectoryRecord::loadFromStream: Error: Could not read hash!\n";
    return false;
  }
  input.read(reinterpret_cast<char*>(&count), 4);
  if (bsaVersion >= 105)
  {
    // Skip the four bytes of padding that are new in version 105.
    input.seekg(4, std::ios::cur);
  }
  input.read(reinterpret_cast<char*>(&offset), 4);
  if (bsaVersion >= 105)
  {
    // Skip the four bytes of padding that are new in version 105.
    input.seekg(4, std::ios::cur);
  }
  if (!input.good())
  {
    std::cerr << "BSADirectoryRecord::loadFromStream: Error while reading data!\n";
    return false;
  }
  // All is well.
  return true;
}

} // namespace
