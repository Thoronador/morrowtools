/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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
{
  folderName = "";
  files.clear();
}

BSAFolderBlock::~BSAFolderBlock()
{
  //empty
}

bool BSAFolderBlock::loadFromStream(std::ifstream& in_File, const uint32_t fileCount)
{
  if (!in_File.good())
  {
    std::cout << "BSAFolderBlock::loadFromStream: Error: bad stream given!\n";
    return false;
  }
  uint8_t byteLen = 0;
  //read length
  in_File.read((char*) &byteLen, 1);
  if (!in_File.good())
  {
    std::cout << "BSAFolderBlock::loadFromStream: Error while reading length byte!\n";
    return false;
  }
  char * buffer = new char[byteLen+1];
  memset(buffer, 0, byteLen+1);
  //read folder's name
  in_File.read(buffer, byteLen);
  if (!in_File.good())
  {
    std::cout << "BSAFolderBlock::loadFromStream: Error while reading folder's name!\n";
    delete[] buffer;
    return false;
  }
  folderName = std::string(buffer);
  delete[] buffer;
  buffer = NULL;

  //read file entries
  files.clear();
  BSAFileRecord tempFileRecord;
  uint32_t i;
  for (i=0; i<fileCount; ++i)
  {
    if (!tempFileRecord.loadFromStream(in_File))
    {
      std::cout << "BSAFolderBlock::loadFromStream: Error while reading file records!\n";
      return false;
    }
    files.push_back(tempFileRecord);
  }//for

  return in_File.good();
}

} //namespace
