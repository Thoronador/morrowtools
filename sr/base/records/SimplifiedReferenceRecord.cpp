/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#include "SimplifiedReferenceRecord.h"

#include "ReferenceRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

SimplifiedReferenceRecord::SimplifiedReferenceRecord()
: BasicRecord(), baseObjectFormID(0)
{

}

SimplifiedReferenceRecord::~SimplifiedReferenceRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool SimplifiedReferenceRecord::equals(const SimplifiedReferenceRecord& other) const
{
  return ((equalsBasic(other)) and (baseObjectFormID==other.baseObjectFormID));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t SimplifiedReferenceRecord::getWriteSize() const
{
  //This record type is not writable! Thus we return zero.
  return 0;
}

bool SimplifiedReferenceRecord::saveToStream(std::ofstream& output) const
{
  #warning This record type is not writable!
  std::cout << "Error: Simplified reference records cannot be saved to stream!\n";
  return false;
}
#endif

bool SimplifiedReferenceRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  bool hasReadNAME = false; baseObjectFormID = 0;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cNAME:
           if (hasReadNAME)
           {
             std::cout << "Error: REFR seems to have more than one NAME subrecord!\n";
             return false;
           }
           //read NAME
           if (!loadUint32SubRecordFromStream(in_File, cNAME, baseObjectFormID, false)) return false;
           bytesRead += 6;
           hasReadNAME = true;
           //skip rest of record
           if (bytesRead<readSize)
           {
             in_File.seekg(readSize-bytesRead, std::ios_base::cur);
             if (!in_File.good())
             {
               std::cout << "Error while skipping subrecords of REFR!\n";
               return false;
             }
             bytesRead = readSize;
           }//if
           break;
      default:
           //read subrecord's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           //skip whole subrecord
           in_File.seekg(subLength, std::ios_base::cur);
           if (!in_File.good())
           {
             std::cout << "Error while skipping subrecord of REFR!\n";
             return false;
           }
           bytesRead += subLength;
           break;
    }//swi
  }//while

  //presence checks
  if (!hasReadNAME)
  {
    std::cout << "Error: subrecord NAME of REFR is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t SimplifiedReferenceRecord::getRecordType() const
{
  return cREFR;
}

} //namespace
