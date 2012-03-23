/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#include "BasicRecord.h"
#include <cstring>
#include <iostream>
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

BasicRecord::BasicRecord()
{
  headerFlags = headerFormID = headerRevision = headerVersion = headerUnknown5 = 0;
}

BasicRecord::~BasicRecord()
{
  //empty
  //Purpose is to have a virtual destructor to get rid of compiler warnings.
}

bool BasicRecord::isGenericRecord() const
{
  return false;
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
  return ((headerFlags==other.headerFlags)
    and (headerFormID==other.headerFormID)
    and (headerRevision==other.headerRevision)
    and (headerVersion==other.headerVersion)
    and (headerUnknown5==other.headerUnknown5));
}
#endif

bool BasicRecord::loadSizeAndUnknownValues(std::ifstream& in_File, uint32_t& sizeStorage)
{
  in_File.read((char*) &sizeStorage, 4);
  //partially unknown values
  in_File.read((char*) &headerFlags, 4);
  in_File.read((char*) &headerFormID, 4);
  in_File.read((char*) &headerRevision, 4);
  in_File.read((char*) &headerVersion, 2);
  in_File.read((char*) &headerUnknown5, 2);
  if (!in_File.good())
  {
    std::cout << "BasicRecord::loadSizeAndUnknownValues: Error while reading "
              << "record size and unknown header data.\n";
    return false;
  }
  return true;
}

#ifndef SR_UNSAVEABLE_RECORDS
bool BasicRecord::saveSizeAndUnknownValues(std::ofstream& output, const uint32_t theSize) const
{
  //record size
  output.write((const char*) &theSize, 4);
  //partially unknown values
  output.write((const char*) &headerFlags, 4);
  output.write((const char*) &headerFormID, 4);
  output.write((const char*) &headerRevision, 4);
  output.write((const char*) &headerVersion, 2);
  output.write((const char*) &headerUnknown5, 2);
  if (!output.good())
  {
    std::cout << "BasicRecord::saveSizeAndUnknownValues: Error while writing "
              << "record size and unknown header data.\n";
    return false;
  }
  return true;
}
#endif

bool BasicRecord::loadUint32SubRecordFromStream(std::istream& in_File, const int32_t subHeader, uint32_t& target, const bool withHeader) const
{
  if (withHeader)
  {
    int32_t subRecName = 0;
    //read header
    in_File.read((char*) &subRecName, 4);
    if (subRecName!=subHeader)
    {
      UnexpectedRecord(subHeader, subRecName);
      return false;
    }
  }//if header
  //subrecord's length
  uint16_t subLength = 0;
  in_File.read((char*) &subLength, 2);
  if (subLength!=4)
  {
    std::cout <<"Error: sub record "<<IntTo4Char(subHeader)<<" of "
              <<IntTo4Char(getRecordType())<<" has invalid length ("<<subLength
              <<" bytes). Should be four bytes.\n";
    return false;
  }
  //read value
  in_File.read((char*) &target, 4);
  if (!in_File.good())
  {
    std::cout << "BasicRecord::loatUint32: Error while reading subrecord "
              <<IntTo4Char(subHeader)<<" of "<<IntTo4Char(getRecordType())<<"!\n";
    return false;
  }
  return true;
}

bool BasicRecord::loadString512FromStream(std::istream& in_File, std::string& target, char * buffer, const int32_t subHeader, const bool withHeader, uint32_t& bytesRead) const
{
  if (withHeader)
  {
    int32_t subRecName = 0;
    //read header
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    if (subRecName!=subHeader)
    {
      UnexpectedRecord(subHeader, subRecName);
      return false;
    }
  }//if with header
  //subrecord's length
  uint16_t subLength = 0;
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength>511)
  {
    std::cout <<"Error: sub record "<<IntTo4Char(subHeader)<<" of "<<IntTo4Char(getRecordType())<<" is longer than 511 characters!\n";
    return false;
  }
  //read string
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord "<<IntTo4Char(subHeader)<<" of "<<IntTo4Char(getRecordType())<<"!\n";
    return false;
  }
  target = std::string(buffer);
  return true;
}

bool BasicRecord::loadBufferFromStream(std::istream& in_File, const uint16_t len, uint8_t * target, const int32_t subHeader, const bool withHeader, uint32_t& bytesRead) const
{
  if (withHeader)
  {
    int32_t subRecName = 0;
    //read header
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    if (subRecName!=subHeader)
    {
      UnexpectedRecord(subHeader, subRecName);
      return false;
    }
  }//if with header
  //subrecord's length
  uint16_t subLength = 0;
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=len)
  {
    std::cout <<"Error: sub record "<<IntTo4Char(subHeader)<<" of "<<IntTo4Char(getRecordType())
              <<" has invalid length ("<<subLength<<" bytes). Should be "<<len<<" bytes!\n";
    return false;
  }
  in_File.read((char*) target, len);
  bytesRead += len;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord "<<IntTo4Char(subHeader)<<" of "<<IntTo4Char(getRecordType())<<"!\n";
    return false;
  }
  return true;
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t BasicRecord::getTotalWrittenSize() const
{
  /* 24 bytes are the "header" of each record, containing type, size and so on */
  return (24 + getWriteSize());
}
#endif

bool BasicRecord::isCompressed() const
{
  return ((headerFlags & cCompressionFlag)!=0);
}

bool BasicRecord::isDeleted() const
{
  return ((headerFlags & cDeletedFlag)!=0);
}

bool BasicRecord::isIgnored() const
{
  return ((headerFlags & cIgnoredFlag)!=0);
}

} //namespace
