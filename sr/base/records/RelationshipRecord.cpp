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

#include "RelationshipRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

RelationshipRecord::RelationshipRecord()
: BasicRecord()
{
  editorID = "";
  unknownDATA[0] = unknownDATA[1] = unknownDATA[2] = unknownDATA[3] = 0;
}

RelationshipRecord::~RelationshipRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool RelationshipRecord::equals(const RelationshipRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownDATA[0]==other.unknownDATA[0]) and (unknownDATA[1]==other.unknownDATA[1])
      and (unknownDATA[2]==other.unknownDATA[2]) and (unknownDATA[3]==other.unknownDATA[3]));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t RelationshipRecord::getWriteSize() const
{
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of string +1 byte for NUL-termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +16 /* fixed size */);
}

bool RelationshipRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cRELA, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 16; //fixed size
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) &unknownDATA[0], 4);
  output.write((const char*) &unknownDATA[1], 4);
  output.write((const char*) &unknownDATA[2], 4);
  output.write((const char*) &unknownDATA[3], 4);

  return output.good();
}
#endif

bool RelationshipRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of RELA is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of RELA!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read DATA
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, subRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &subLength, 2);
  if (subLength!=16)
  {
    std::cout <<"Error: sub record DATA of RELA has invalid length("<<subLength
              <<" bytes). Should be 16 bytes!\n";
    return false;
  }
  //read DATA's stuff
  in_File.read((char*) &unknownDATA[0], 4);
  in_File.read((char*) &unknownDATA[1], 4);
  in_File.read((char*) &unknownDATA[2], 4);
  in_File.read((char*) &unknownDATA[3], 4);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of RELA!\n";
    return false;
  }

  return true;
}

uint32_t RelationshipRecord::getRecordType() const
{
  return cRELA;
}

} //namespace
