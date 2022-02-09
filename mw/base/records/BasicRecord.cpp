/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Dirk Stolle

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

#include "BasicRecord.hpp"
#include <cstring>
#include <iostream>
#include "../HelperIO.hpp"

namespace MWTP
{

const int32_t FlagPersists = 1024;
const int32_t FlagBlocked = 8192;

BasicRecord::BasicRecord()
: HeaderOne(0),
  HeaderFlags(0)
{
}

BasicRecord::~BasicRecord()
{
}

int32_t BasicRecord::getHeaderOne() const
{
  return HeaderOne;
}

int32_t BasicRecord::getHeaderFlags() const
{
  return HeaderFlags;
}

void BasicRecord::setHeaderOne(const int32_t newValue)
{
  HeaderOne = newValue;
}

void BasicRecord::setHeaderFlags(const int32_t newValue)
{
  HeaderFlags = newValue;
}

bool BasicRecord::isBlocked() const
{
  return (HeaderFlags & FlagBlocked) != 0;
}

bool BasicRecord::isPersistent() const
{
  return (HeaderFlags & FlagPersists) != 0;
}

bool BasicRecord::loadString256(std::istream& input, std::string& target, char * buffer, const uint32_t subHeader, uint32_t& bytesRead) const
{
  // subrecord's length
  uint32_t subLength = 0;
  input.read(reinterpret_cast<char*>(&subLength), 4);
  bytesRead += 4;
  if (subLength > 255)
  {
    std::cerr << "Error: Sub record " << IntTo4Char(subHeader) // << " of "
              // << IntTo4Char(getRecordType())
              << " is longer than 255 characters!\n";
    return false;
  }
  // read string
  memset(buffer, 0, 256);
  input.read(buffer, subLength);
  bytesRead += subLength;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record " << IntTo4Char(subHeader)
              << "!\n";
    return false;
  }
  target = std::string(buffer);
  return true;
}

bool BasicRecord::loadString256WithHeader(std::istream& input, std::string& target, char * buffer, const uint32_t subHeader, uint32_t& bytesRead) const
{
  uint32_t subRecName = 0;
  // read header
  input.read(reinterpret_cast<char*>(&subRecName), 4);
  if (subRecName != subHeader)
  {
    UnexpectedRecord(subHeader, subRecName);
    return false;
  }
  bytesRead += 4;
  // read string
  return loadString256(input, target, buffer, subHeader, bytesRead);
}

} // namespace
