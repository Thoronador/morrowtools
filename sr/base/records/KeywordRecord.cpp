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

#include "KeywordRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

KeywordRecord::KeywordRecord()
: BasicRecord()
{
  editorID = "";
  unknownCNAM = 0;
  hasCNAM = true;
}

KeywordRecord::~KeywordRecord()
{
  //empty
}

int32_t KeywordRecord::getRecordType() const
{
  return cKYWD;
}

#ifndef SR_NO_RECORD_EQUALITY
bool KeywordRecord::equals(const KeywordRecord& other) const
{
  if ((editorID==other.editorID) and (hasCNAM==other.hasCNAM)
    and (equalsBasic(other)))
  {
    if (hasCNAM) return (unknownCNAM==other.unknownCNAM);
    return true;
  }
  return false;
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t KeywordRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  if (hasCNAM)
  {
    writeSize = writeSize +4 /* CNAM */ +2 /* 2 bytes for length */
               +4 /* fixed length of 4 bytes */;
  }
  return writeSize;
}

bool KeywordRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cKYWD, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  //write EDID
  output.write((char*) &cEDID, 4);
  //HEDR's length
  uint16_t subLength = editorID.length()+1;
  output.write((char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);
  if (!output.good())
  {
    std::cout << "Error while writing subrecord EDID of KYWD!\n";
    return false;
  }

  if (hasCNAM)
  {
    //write CNAM
    output.write((char*) &cCNAM, 4);
    //CNAM's length
    subLength = 4; /* fixed length of four bytes */
    output.write((char*) &subLength, 2);
    //write CNAM stuff
    output.write((char*) &unknownCNAM, 4);
  }

  return output.good();
}
#endif

bool KeywordRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  bytesRead = 4;
  if (subRecName!=cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  //EDID's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength>511)
  {
    std::cout <<"Error: sub record EDID of KYWD is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of KYWD!\n";
    return false;
  }
  editorID = std::string(buffer);

  if (bytesRead<readSize)
  {
    hasCNAM = true;
    //read CNAM
    if (!loadUint32SubRecordFromStream(in_File, cCNAM, unknownCNAM)) return false;
  }
  else
  {
    hasCNAM = false;
    unknownCNAM = 0;
  }

  return true;
}

} //namespace
