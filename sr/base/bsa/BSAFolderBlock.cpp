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

#include "BSAFolderBlock.hpp"
#include <iostream>
#include <cstring>

namespace SRTP
{

BSAFolderBlock::BSAFolderBlock()
: folderName(std::string()),
  files(std::vector<BSAFileRecord>())
{
}

bool BSAFolderBlock::loadFromStream(std::istream& input, const uint32_t fileCount)
{
  if (!input.good())
  {
    std::cerr << "BSAFolderBlock::loadFromStream: Error: Bad stream given!\n";
    return false;
  }
  uint8_t byteLen = 0;
  // read length of folder name
  input.read(reinterpret_cast<char*>(&byteLen), 1);
  if (!input.good())
  {
    std::cerr << "BSAFolderBlock::loadFromStream: Error while reading length byte!\n";
    return false;
  }
  char * buffer = new char[byteLen + 1];
  memset(buffer, 0, byteLen + 1);
  // read folder's name
  input.read(buffer, byteLen);
  if (!input.good())
  {
    std::cerr << "BSAFolderBlock::loadFromStream: Error while reading folder's name!\n";
    delete[] buffer;
    return false;
  }
  folderName = std::string(buffer);
  delete[] buffer;
  buffer = nullptr;

  // read file entries
  files.clear();
  BSAFileRecord tempFileRecord;
  for (uint32_t i = 0; i < fileCount; ++i)
  {
    if (!tempFileRecord.loadFromStream(input))
    {
      std::cerr << "BSAFolderBlock::loadFromStream: Error while reading file records!\n";
      return false;
    }
    files.emplace_back(tempFileRecord);
  }

  return true;
}

} // namespace
