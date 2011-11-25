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

#include "BasicRecord.h"
#include <iostream>
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

BasicRecord::BasicRecord()
{
  headerFlags = headerFormID = headerUnknown3 = headerUnknown4 = 0;
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
  headerUnknown3 = other.headerUnknown3;
  headerUnknown4 = other.headerUnknown4;
}

bool BasicRecord::equalsBasic(const BasicRecord& other) const
{
  return ((headerFlags==other.headerFlags)
    and (headerFormID==other.headerFormID)
    and (headerUnknown3==other.headerUnknown3)
    and (headerUnknown4==other.headerUnknown4));
}

bool BasicRecord::loadSizeAndUnknownValues(std::ifstream& in_File, uint32_t& sizeStorage)
{
  in_File.read((char*) &sizeStorage, 4);
  //unknown values
  in_File.read((char*) &headerFlags, 4);
  in_File.read((char*) &headerFormID, 4);
  in_File.read((char*) &headerUnknown3, 4);
  in_File.read((char*) &headerUnknown4, 4);
  if (!in_File.good())
  {
    std::cout << "BasicRecord::loadSizeAndUnknownValues: Error while reading "
              << "record size and unknown header data.\n";
    return false;
  }
  return true;
}

bool BasicRecord::saveSizeAndUnknownValues(std::ofstream& output, const uint32_t theSize) const
{
  //record size
  output.write((char*) &theSize, 4);
  //unknown values
  output.write((char*) &headerFlags, 4);
  output.write((char*) &headerFormID, 4);
  output.write((char*) &headerUnknown3, 4);
  output.write((char*) &headerUnknown4, 4);
  if (!output.good())
  {
    std::cout << "BasicRecord::saveSizeAndUnknownValues: Error while reading "
              << "record size and unknown header data.\n";
    return false;
  }
  return true;
}

bool BasicRecord::loadUint32SubRecordFromStream(std::ifstream& in_File, const int32_t subHeader, uint32_t& target) const
{
  int32_t subRecName = 0;
  //read header
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=subHeader)
  {
    UnexpectedRecord(subHeader, subRecName);
    return false;
  }
  //subrecord's length
  uint16_t subLength = 0;
  in_File.read((char*) &subLength, 2);
  if (subLength!=4)
  {
    std::cout <<"Error: sub record "<<IntTo4Char(subHeader)<<" has invalid length ("<<subLength
              <<" bytes). Should be 4 bytes.\n";
    return false;
  }
  //read value
  in_File.read((char*) &target, 4);
  if (!in_File.good())
  {
    std::cout << "BasicRecord::loatUint32: Error while reading subrecord "<<IntTo4Char(subHeader)<<"!\n";
    return false;
  }
  return true;
}

} //namespace