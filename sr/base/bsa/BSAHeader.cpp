/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "BSAHeader.h"
#include <iostream>

namespace SRTP
{

const int32_t cBSA_NUL = 0x00415342; //"BSA\0" = 42 53 41 00

BSAHeader::BSAHeader()
{
  fileID = 0;
  version = 0;
  offset = 0;
  archiveFlags = 0;
  folderCount = 0;
  fileCount = 0;
  totalFolderNameLength = 0;
  totalFileNameLength = 0;
  fileFlags = 0;
}

BSAHeader::~BSAHeader()
{
  //empty
}

bool BSAHeader::loadFromStream(std::ifstream& in_File)
{
  if (!in_File.good())
  {
    std::cout << "BSAHeader::loadFromStream: Error: bad stream given!\n";
    return false;
  }
  //read stuff
  in_File.read((char*) &fileID, 4);
  if (!in_File.good())
  {
    std::cout << "BSAHeader::loadFromStream: Error: could not read header!\n";
    return false;
  }
  if (fileID!=cBSA_NUL)
  {
    std::cout << "BSAHeader::loadFromStream: Error: invalid BSA header!\n";
    return false;
  }
  //read version
  in_File.read((char*) &version, 4);
  //read offset
  in_File.read((char*) &offset, 4);
  if (!in_File.good())
  {
    std::cout << "BSAHeader::loadFromStream: Error: could not read version and offset!\n";
    return false;
  }
  if (version!=104)
  {
    std::cout << "BSAHeader::loadFromStream: Warning: invalid version detected, but let's give it a try anyway!\n";
  }
  if (offset!=36)
  {
    std::cout << "BSAHeader::loadFromStream: Error: offset is not 36, aborting!\n";
    return false;
  }
  //read rest
  in_File.read((char*) &archiveFlags, 4);
  in_File.read((char*) &folderCount, 4);
  in_File.read((char*) &fileCount, 4);
  in_File.read((char*) &totalFolderNameLength, 4);
  in_File.read((char*) &totalFileNameLength, 4);
  in_File.read((char*) &fileFlags, 4);
  if (!in_File.good())
  {
    std::cout << "BSAHeader::loadFromStream: Error while reading from stream!\n";
    return false;
  }

  return true;
}

bool BSAHeader::hasNamesForFolders() const
{
  return ((archiveFlags & 1)!=0);
}

bool BSAHeader::hasNamesForFiles() const
{
  return ((archiveFlags & (1<<1))!=0);
}

bool BSAHeader::filesCompressedByDefault() const
{
  return ((archiveFlags & (1<<2))!=0);
}


} //namespace
