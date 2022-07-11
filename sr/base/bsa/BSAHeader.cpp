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

#include "BSAHeader.hpp"
#include <iostream>

namespace SRTP
{

const int32_t cBSA_NUL = 0x00415342; //"BSA\0" = 42 53 41 00

BSAHeader::BSAHeader()
: fileID(0),
  version(0),
  offset(0),
  archiveFlags(0),
  directoryCount(0),
  fileCount(0),
  totalDirectoryNameLength(0),
  totalFileNameLength(0),
  fileFlags(0)
{
}

bool BSAHeader::loadFromStream(std::istream& input)
{
  if (!input.good())
  {
    std::cerr << "BSAHeader::loadFromStream: Error: Bad stream given!\n";
    return false;
  }
  // read stuff
  input.read(reinterpret_cast<char*>(&fileID), 4);
  if (!input.good())
  {
    std::cerr << "BSAHeader::loadFromStream: Error: Could not read header!\n";
    return false;
  }
  if (fileID != cBSA_NUL)
  {
    std::cerr << "BSAHeader::loadFromStream: Error: Invalid BSA header!\n";
    return false;
  }
  // read version
  input.read(reinterpret_cast<char*>(&version), 4);
  // read offset
  input.read(reinterpret_cast<char*>(&offset), 4);
  if (!input.good())
  {
    std::cerr << "BSAHeader::loadFromStream: Error: Could not read version and offset!\n";
    return false;
  }
  if (offset != 36)
  {
    std::cerr << "BSAHeader::loadFromStream: Error: Offset is not 36, aborting!\n";
    return false;
  }
  // read rest
  input.read(reinterpret_cast<char*>(&archiveFlags), 4);
  input.read(reinterpret_cast<char*>(&directoryCount), 4);
  input.read(reinterpret_cast<char*>(&fileCount), 4);
  input.read(reinterpret_cast<char*>(&totalDirectoryNameLength), 4);
  input.read(reinterpret_cast<char*>(&totalFileNameLength), 4);
  input.read(reinterpret_cast<char*>(&fileFlags), 4);
  if (!input.good())
  {
    std::cerr << "BSAHeader::loadFromStream: Error while reading from stream!\n";
    return false;
  }

  return true;
}

bool BSAHeader::hasNamesForDirectories() const
{
  return (archiveFlags & 1) != 0;
}

bool BSAHeader::hasNamesForFiles() const
{
  return (archiveFlags & (1 << 1)) != 0;
}

bool BSAHeader::filesCompressedByDefault() const
{
  return (archiveFlags & (1 << 2)) != 0;
}

bool BSAHeader::isXboxArchive() const
{
  return (archiveFlags & (1 << 6)) != 0;
}

bool BSAHeader::hasEmbeddedFileNames() const
{
  return (archiveFlags & (1 << 8)) != 0;
}

bool BSAHeader::usesXMemCodec() const
{
  // Meaning of the 10th flag bit is not clear in version 103 and earlier, so
  // do not use it to indicate XMem compression codec there.
  return ((archiveFlags & (1 << 9)) != 0) && (version >= 104);
}

bool BSAHeader::contains(const ContentType content) const
{
  return (fileFlags & static_cast<uint32_t>(content)) != 0;
}

} // namespace
