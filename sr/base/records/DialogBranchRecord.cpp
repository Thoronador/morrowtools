/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#include "DialogBranchRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

DialogBranchRecord::DialogBranchRecord()
: BasicRecord()
{
  editorID = "";
  unknownQNAM = 0;
  unknownTNAM = 0;
  unknownDNAM = 0;
  unknownSNAM = 0;
}

DialogBranchRecord::~DialogBranchRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool DialogBranchRecord::equals(const DialogBranchRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownQNAM==other.unknownQNAM) and (unknownTNAM==other.unknownTNAM)
      and (unknownDNAM==other.unknownDNAM) and (unknownSNAM==other.unknownSNAM));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t DialogBranchRecord::getWriteSize() const
{
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* QNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */
        +4 /* TNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */
        +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */);
}

bool DialogBranchRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cDLBR, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write QNAM
  output.write((const char*) &cQNAM, 4);
  //QNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write QNAM's stuff
  output.write((const char*) &unknownQNAM, 4);

  //write TNAM
  output.write((const char*) &cTNAM, 4);
  //TNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write TNAM's stuff
  output.write((const char*) &unknownTNAM, 4);

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write DNAM's stuff
  output.write((const char*) &unknownDNAM, 4);

  //write SNAM
  output.write((const char*) &cSNAM, 4);
  //SNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write SNAM's stuff
  output.write((const char*) &unknownSNAM, 4);

  return output.good();
}
#endif

bool DialogBranchRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of DLBR is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of DLBR!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read QNAM
  if (!loadUint32SubRecordFromStream(in_File, cQNAM, unknownQNAM, true))
  {
    std::cout << "Error while reading subrecord QNAM of DLRB!\n";
    return false;
  }

  //read TNAM
  if (!loadUint32SubRecordFromStream(in_File, cTNAM, unknownTNAM, true))
  {
    std::cout << "Error while reading subrecord TNAM of DLRB!\n";
    return false;
  }

  //read DNAM
  if (!loadUint32SubRecordFromStream(in_File, cDNAM, unknownDNAM, true))
  {
    std::cout << "Error while reading subrecord DNAM of DLRB!\n";
    return false;
  }

  //read SNAM
  if (!loadUint32SubRecordFromStream(in_File, cSNAM, unknownSNAM, true))
  {
    std::cout << "Error while reading subrecord SNAM of DLRB!\n";
    return false;
  }

  return in_File.good();
}

uint32_t DialogBranchRecord::getRecordType() const
{
  return cDLBR;
}

} //namespace
