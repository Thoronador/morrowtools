/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Thoronador

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
#include "../../../mw/base/HelperIO.hpp"
#include "../SR_Constants.hpp"

namespace SRTP
{

BasicRecord::BasicRecord()
: headerFlags(0), headerFormID(0), headerRevision(0),
  headerVersion(0), headerUnknown5(0)
{
}

BasicRecord::~BasicRecord()
{
  //empty
  //Purpose is to have a virtual destructor to get rid of compiler warnings.
}

void BasicRecord::copyBasicMembers(const BasicRecord& other)
{
  headerFlags = other.headerFlags;
  headerFormID = other.headerFormID;
  headerRevision = other.headerRevision;
  headerVersion = other.headerVersion;
  headerUnknown5 = other.headerUnknown5;
}

#ifndef SR_NO_RECORD_EQUALITY
bool BasicRecord::equalsBasic(const BasicRecord& other) const
{
  return (headerFlags == other.headerFlags)
    && (headerFormID == other.headerFormID)
    && (headerRevision == other.headerRevision)
    && (headerVersion == other.headerVersion)
    && (headerUnknown5 == other.headerUnknown5);
}
#endif

bool BasicRecord::loadSizeAndUnknownValues(std::istream& input, uint32_t& sizeStorage)
{
  input.read(reinterpret_cast<char*>(&sizeStorage), 4);
  // partially unknown values
  input.read(reinterpret_cast<char*>(&headerFlags), 4);
  input.read(reinterpret_cast<char*>(&headerFormID), 4);
  input.read(reinterpret_cast<char*>(&headerRevision), 4);
  input.read(reinterpret_cast<char*>(&headerVersion), 2);
  input.read(reinterpret_cast<char*>(&headerUnknown5), 2);
  if (!input.good())
  {
    std::cerr << "BasicRecord::loadSizeAndUnknownValues: Error while reading "
              << "record size and unknown header data.\n";
    return false;
  }
  return true;
}

#ifndef SR_UNSAVEABLE_RECORDS
bool BasicRecord::saveSizeAndUnknownValues(std::ostream& output, const uint32_t theSize) const
{
  // record size
  output.write(reinterpret_cast<const char*>(&theSize), 4);
  // partially unknown values
  output.write(reinterpret_cast<const char*>(&headerFlags), 4);
  output.write(reinterpret_cast<const char*>(&headerFormID), 4);
  output.write(reinterpret_cast<const char*>(&headerRevision), 4);
  output.write(reinterpret_cast<const char*>(&headerVersion), 2);
  output.write(reinterpret_cast<const char*>(&headerUnknown5), 2);
  if (!output.good())
  {
    std::cerr << "BasicRecord::saveSizeAndUnknownValues: Error while writing "
              << "record size and unknown header data.\n";
    return false;
  }
  return true;
}
#endif

bool BasicRecord::loadUint32SubRecordFromStream(std::istream& input, const uint32_t subHeader, uint32_t& target, const bool withHeader) const
{
  if (withHeader)
  {
    uint32_t subRecName = 0;
    // read header
    input.read(reinterpret_cast<char*>(&subRecName), 4);
    if (subRecName != subHeader)
    {
      UnexpectedRecord(subHeader, subRecName);
      return false;
    }
  }
  // subrecord's length
  uint16_t subLength = 0;
  input.read(reinterpret_cast<char*>(&subLength), 2);
  if (subLength != 4)
  {
    std::cerr << "Error: Sub record " << IntTo4Char(subHeader) << " of "
              << IntTo4Char(getRecordType()) << " has invalid length ("
              << subLength << " bytes). Should be four bytes.\n";
    return false;
  }
  // read value
  input.read(reinterpret_cast<char*>(&target), 4);
  if (!input.good())
  {
    std::cerr << "BasicRecord::loadUint32: Error while reading subrecord "
              << IntTo4Char(subHeader) << " of " << IntTo4Char(getRecordType())
              << "!\n";
    return false;
  }
  return true;
}

bool BasicRecord::loadString512FromStream(std::istream& input, std::string& target, char * buffer, const uint32_t subHeader, const bool withHeader, uint32_t& bytesRead) const
{
  if (withHeader)
  {
    uint32_t subRecName = 0;
    // read header
    input.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    if (subRecName != subHeader)
    {
      UnexpectedRecord(subHeader, subRecName);
      return false;
    }
  }
  // subrecord's length
  uint16_t subLength = 0;
  input.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength > 511)
  {
    std::cerr << "Error: Sub record " << IntTo4Char(subHeader) << " of "
              << IntTo4Char(getRecordType())
              << " is longer than 511 characters!\n";
    return false;
  }
  // read string
  memset(buffer, 0, 512);
  input.read(buffer, subLength);
  bytesRead += subLength;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord " << IntTo4Char(subHeader)
              << " of " << IntTo4Char(getRecordType()) << "!\n";
    return false;
  }
  target = std::string(buffer);
  return true;
}

bool BasicRecord::loadKeywords(std::istream& input, std::vector<uint32_t>& keywords, uint32_t& bytesRead) const
{
  if (!keywords.empty())
  {
    std::cerr << "Error: Record " << IntTo4Char(getRecordType())
              << " seems to have more than one KSIZ subrecord!\n";
    return false;
  }
  // KSIZ's length
  uint16_t subLength = 0;
  input.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength != 4)
  {
    std::cerr << "Error: Subrecord KSIZ of " << IntTo4Char(getRecordType())
              << " has invalid length (" << subLength
              << " bytes). Should be four bytes!\n";
    return false;
  }
  // read KSIZ's stuff
  uint32_t k_Size = 0;
  input.read(reinterpret_cast<char*>(&k_Size), 4);
  bytesRead += 4;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord KSIZ of "
              << IntTo4Char(getRecordType()) << "!\n";
    return false;
  }
  if (k_Size == 0)
  {
    std::cerr << "Error: " << IntTo4Char(getRecordType())
              << "'s KSIZ value is zero, but that's not allowed!\n";
    return false;
  }

  // read KWDA
  uint32_t u32 = 0;
  input.read(reinterpret_cast<char*>(&u32), 4);
  bytesRead += 4;
  if (u32 != cKWDA)
  {
    UnexpectedRecord(cKWDA, u32);
    return false;
  }
  // KWDA's length
  input.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength != 4 * k_Size)
  {
    std::cerr << "Error: Sub record KWDA of " << IntTo4Char(getRecordType())
              << " has invalid length (" << subLength << " bytes). Should be "
              << 4 * k_Size << " bytes!\n";
    return false;
  }
  // read KWDA's stuff
  for (uint32_t i = 0; i < k_Size; ++i)
  {
    input.read(reinterpret_cast<char*>(&u32), 4);
    keywords.push_back(u32);
  }
  bytesRead += 4 * k_Size;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord KWDA of "
              << IntTo4Char(getRecordType()) << "!\n";
    return false;
  }
  return true;
}

bool BasicRecord::loadBufferFromStream(std::istream& in_File, const uint16_t len, uint8_t * target, const uint32_t subHeader, const bool withHeader, uint32_t& bytesRead) const
{
  if (withHeader)
  {
    uint32_t subRecName = 0;
    // read header
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    if (subRecName != subHeader)
    {
      UnexpectedRecord(subHeader, subRecName);
      return false;
    }
  }
  // subrecord's length
  uint16_t subLength = 0;
  in_File.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength != len)
  {
    std::cerr << "Error: Sub record " << IntTo4Char(subHeader) << " of "
              << IntTo4Char(getRecordType()) << " has invalid length ("
              << subLength << " bytes). Should be " << len << " bytes!\n";
    return false;
  }
  in_File.read(reinterpret_cast<char*>(target), len);
  bytesRead += len;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord " << IntTo4Char(subHeader)
              << " of " << IntTo4Char(getRecordType()) << "!\n";
    return false;
  }
  return true;
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t BasicRecord::getTotalWrittenSize() const
{
  /* 24 bytes are the "header" of each record, containing type, size and so on */
  return 24 + getWriteSize();
}
#endif

bool BasicRecord::isCompressed() const
{
  return (headerFlags & cCompressionFlag) != 0;
}

bool BasicRecord::isDeleted() const
{
  return (headerFlags & cDeletedFlag) != 0;
}

bool BasicRecord::isIgnored() const
{
  return (headerFlags & cIgnoredFlag) != 0;
}

} // namespace
