/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "FootstepSetRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

FootstepSetRecord::FootstepSetRecord()
: BasicRecord(), editorID(""),
  footstepFormIDs(std::vector<uint32_t>())
{
  xCounts[0] = xCounts[1] = xCounts[2] = xCounts[3] = xCounts[4] = 0;
}

FootstepSetRecord::~FootstepSetRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool FootstepSetRecord::equals(const FootstepSetRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (xCounts[0]==other.xCounts[0]) and (xCounts[1]==other.xCounts[1])
      and (xCounts[2]==other.xCounts[2]) and (xCounts[3]==other.xCounts[3])
      and (xCounts[4]==other.xCounts[4]) and (footstepFormIDs==other.footstepFormIDs));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t FootstepSetRecord::getWriteSize() const
{
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* XCNT */ +2 /* 2 bytes for length */ +20 /* fixed size */
        +4 /* ANAM */ +2 /* 2 bytes for length */
        +4*footstepFormIDs.size() /* 4 bytes per form ID */);
}

bool FootstepSetRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cFSTS, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write XCNT
  output.write((const char*) &cXCNT, 4);
  //XCNT's length
  subLength = 20; //fixed
  output.write((const char*) &subLength, 2);
  //write XCNT's stuff
  unsigned int i;
  for (i=0; i<5; ++i)
  {
    output.write((const char*) &(xCounts[0]), 4);
  }//for

  //check consistency of data
  uint32_t sum = 0;
  for (i=0; i<5; ++i)
  {
    sum += xCounts[i];
  }//for
  const uint32_t id_count = footstepFormIDs.size();
  if (sum!=id_count)
  {
    std::cout << "Error: Data in record FSTS is inconsistent. Sum of counts ("
              << sum<<") does not match the number of form IDs ("<<id_count<<")!\n";
    return false;
  }

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = id_count*4;
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  for (i=0; i<id_count; ++i)
  {
    output.write((const char*) &(footstepFormIDs[i]), 4);
  }//for

  return output.good();
}
#endif

bool FootstepSetRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  //EDID's length
  in_File.read((char*) &subLength, 2);
  if (subLength>511)
  {
    std::cout <<"Error: sub record EDID of FSTS is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of FSTS!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read XCNT
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cXCNT)
  {
    UnexpectedRecord(cXCNT, subRecName);
    return false;
  }
  //XCNT's length
  in_File.read((char*) &subLength, 2);
  if (subLength!=20)
  {
    std::cout <<"Error: sub record XCNT of FSTS has invalid length ("
              <<subLength<<" bytes). Should be 20 bytes!\n";
    return false;
  }
  //read XCNT's stuff
  unsigned int i;
  for (i=0; i<5; ++i)
  {
    in_File.read((char*) &(xCounts[i]), 4);
  }
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord XCNT of FSTS!\n";
    return false;
  }

  //add counts
  uint32_t sum = 0;
  for (i=0; i<5; ++i)
  {
    sum += xCounts[i];
  }//for

  //read DATA
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, subRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &subLength, 2);
  if (subLength!=4*sum)
  {
    std::cout <<"Error: sub record DATA of FSTS has invalid length ("
              <<subLength<<" bytes). Should be "<<4*sum<<" bytes!\n";
    return false;
  }
  //read DATA's stuff
  footstepFormIDs.clear();
  uint32_t temp;
  for (i=0; i<sum; ++i)
  {
    in_File.read((char*) &temp, 4);
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord DATA of FSTS!\n";
      return false;
    }
    footstepFormIDs.push_back(temp);
  }//for

  return in_File.good();
}

uint32_t FootstepSetRecord::getRecordType() const
{
  return cFSTS;
}

} //namespace
