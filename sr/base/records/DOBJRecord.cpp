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

#include "DOBJRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

/* EntryDNAM's functions */
bool DOBJRecord::EntryDNAM::operator==(const DOBJRecord::EntryDNAM& other) const
{
  return ((label==other.label) and (id==other.id));
}

/* DOBJRecord's functions */

DOBJRecord::DOBJRecord()
: BasicRecord(),
  unknownDNAMs(std::vector<EntryDNAM>())
{

}

DOBJRecord::~DOBJRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool DOBJRecord::equals(const DOBJRecord& other) const
{
  return (equalsBasic(other) and (unknownDNAMs==other.unknownDNAMs));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t DOBJRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  return (4 /* DNAM */ +2 /* 2 bytes for length */
         +unknownDNAMs.size()*8 /* eight bytes per DNAM entry */);
}

bool DOBJRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cDOBJ, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  const uint16_t vecEntries = unknownDNAMs.size();
  const uint16_t subLength = vecEntries*8;
  output.write((const char*) &subLength, 2);
  //write DNAM entries
  unsigned int i;
  for (i=0; i<vecEntries; ++i)
  {
    output.write((const char*) &unknownDNAMs[i].label, 4);
    output.write((const char*) &unknownDNAMs[i].id, 4);
  }//for

  return output.good();
}
#endif

bool DOBJRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  //uint32_t bytesRead;

  unknownDNAMs.clear();
  //read DNAM
  in_File.read((char*) &subRecName, 4);
  //bytesRead = 4;
  if (subRecName!=cDNAM)
  {
    UnexpectedRecord(cDNAM, subRecName);
    return false;
  }
  //DNAM's length
  in_File.read((char*) &subLength, 2);
  //bytesRead += 2;
  if ((subLength==0) or ((subLength%8)!=0))
  {
    std::cout << "Error: subrecord DNAM of DOBJ has invalid length ("<<subLength
              << " bytes). Should be an integral multiple of 8 bytes!\n";
    return false;
  }
  //read DNAM's stuff
  const unsigned int vecSize = subLength / 8;
  EntryDNAM temp;
  unsigned int i;
  for (i=0; i<vecSize; ++i)
  {
    in_File.read((char*) &temp.label, 4);
    in_File.read((char*) &temp.id, 4);
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord DNAM of DOBJ!\n";
      return false;
    }
    unknownDNAMs.push_back(temp);
  }//for

  return in_File.good();
}

uint32_t DOBJRecord::getRecordType() const
{
  return cDOBJ;
}

} //namespace
