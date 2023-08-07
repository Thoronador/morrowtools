/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021, 2022  Dirk Stolle

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

#include "SimplifiedReferenceRecord.hpp"

#include "ReferenceRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

SimplifiedReferenceRecord::SimplifiedReferenceRecord()
: BasicRecord(), baseObjectFormID(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool SimplifiedReferenceRecord::equals(const SimplifiedReferenceRecord& other) const
{
  return equalsBasic(other) && (baseObjectFormID == other.baseObjectFormID);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t SimplifiedReferenceRecord::getWriteSize() const
{
  // This record type is not writable (in most cases)! Thus we return zero.
  return 0;
}

bool SimplifiedReferenceRecord::saveToStream(std::ostream& output) const
{
  // Only the header can be written correctly, and only when the deleted flag
  // is set. For all other cases, the record does not have all the required
  // data.
  if (isDeleted())
  {
    output.write(reinterpret_cast<const char*>(&cREFR), 4);
    return saveSizeAndUnknownValues(output, 0);
  }
  // This is a case where the record cannot be written, because data is missing
  // by design to keep it small. This record type is not writable here!
  std::cerr << "Error: Simplified reference records cannot be saved to stream!\n";
  return false;
}
#endif

bool SimplifiedReferenceRecord::loadFromStream(std::istream& in_File,
                                               [[maybe_unused]] const bool localized,
                                               [[maybe_unused]] const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  uint32_t bytesRead = 0;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;

  baseObjectFormID = 0;
  while (bytesRead < readSize)
  {
    // read next sub record header
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cNAME:
           // read NAME
           if (!loadUint32SubRecordFromStream(in_File, cNAME, baseObjectFormID, false))
             return false;
           bytesRead += 6;
           // skip rest of record
           if (bytesRead < readSize)
           {
             in_File.seekg(readSize - bytesRead, std::ios_base::cur);
             if (!in_File.good())
             {
               std::cerr << "Error while skipping sub records of REFR!\n";
               return false;
             }
           }
           return true;
           break;
      default:
           // read sub record's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           // skip whole sub record
           in_File.seekg(subLength, std::ios_base::cur);
           if (!in_File.good())
           {
             std::cerr << "Error while skipping sub record of REFR!\n";
             return false;
           }
           bytesRead += subLength;
           break;
    }
  }

  // If we get to this point, no NAME has been seen so far.
  std::cerr << "Error: Sub record NAME of REFR is missing!\n";
  return false;
}

uint32_t SimplifiedReferenceRecord::getRecordType() const
{
  return cREFR;
}

} // namespace
