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

#include "RelationshipRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

/* relationship level constants */
const uint16_t RelationshipRecord::cLover        = 0x0000;
const uint16_t RelationshipRecord::cAlly         = 0x0001;
const uint16_t RelationshipRecord::cConfidant    = 0x0002;
const uint16_t RelationshipRecord::cFriend       = 0x0003;
const uint16_t RelationshipRecord::cAcquaintance = 0x0004;
const uint16_t RelationshipRecord::cRival        = 0x0005;
const uint16_t RelationshipRecord::cFoe          = 0x0006;
const uint16_t RelationshipRecord::cEnemy        = 0x0007;
const uint16_t RelationshipRecord::cArchnemesis  = 0x0008;

/* flag constant */
const uint16_t RelationshipRecord::cFlagSecret = 0x8000;

RelationshipRecord::RelationshipRecord()
: BasicRecord(), editorID(""),
  parentNPCFormID(0),
  childNPCFormID(0),
  relationshipLevel(0),
  flags(0),
  associationTypeFormID(0)
{

}

RelationshipRecord::~RelationshipRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool RelationshipRecord::equals(const RelationshipRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (parentNPCFormID==other.parentNPCFormID) and (childNPCFormID==other.childNPCFormID)
      and (relationshipLevel==other.relationshipLevel) and (flags==other.flags)
      and (associationTypeFormID==other.associationTypeFormID));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t RelationshipRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of string +1 byte for NUL-termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +16 /* fixed size */);
}

bool RelationshipRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cRELA, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

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
  output.write((const char*) &parentNPCFormID, 4);
  output.write((const char*) &childNPCFormID, 4);
  output.write((const char*) &relationshipLevel, 2);
  output.write((const char*) &flags, 2);
  output.write((const char*) &associationTypeFormID, 4);

  return output.good();
}
#endif

bool RelationshipRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
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
  in_File.read((char*) &parentNPCFormID, 4);
  in_File.read((char*) &childNPCFormID, 4);
  in_File.read((char*) &relationshipLevel, 2);
  in_File.read((char*) &flags, 2);
  in_File.read((char*) &associationTypeFormID, 4);
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
