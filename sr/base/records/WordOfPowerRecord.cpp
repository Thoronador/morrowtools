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

#include "WordOfPowerRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

WordOfPowerRecord::WordOfPowerRecord()
: BasicRecord(), editorID(""),
  hasFULL(false), nameStringID(0),
  translatedStringID(0)
{

}

WordOfPowerRecord::~WordOfPowerRecord()
{
  //empty
}

uint32_t WordOfPowerRecord::getRecordType() const
{
  return cWOOP;
}

#ifndef SR_NO_RECORD_EQUALITY
bool WordOfPowerRecord::equals(const WordOfPowerRecord& other) const
{
  if ((editorID==other.editorID) and (hasFULL==other.hasFULL)
    and (translatedStringID==other.translatedStringID) and (equalsBasic(other)))
  {
    if (hasFULL) return (nameStringID==other.nameStringID);
    return true;
  }
  return false;
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t WordOfPowerRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* TNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */
               +4 /* fixed length of four bytes */;
  }
  return writeSize;
}

bool WordOfPowerRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cWOOP, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write FULL's data
    output.write((const char*) &nameStringID, 4);
  }

  //write TNAM
  output.write((const char*) &cTNAM, 4);
  //TNAM's length
  subLength = 4; /* fixed size */
  output.write((const char*) &subLength, 2);
  //write TNAM's data
  output.write((const char*) &translatedStringID, 4);

  return output.good();
}
#endif

bool WordOfPowerRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  uint32_t bytesRead = 0;
  subRecName = subLength = 0;

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
    std::cout <<"Error: sub record EDID of WOOP is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of WOOP!\n";
    return false;
  }
  editorID = std::string(buffer);


  //read optional FULL
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName==cFULL)
  {
    //FULL subrecord is present
    //FULL's length
    in_File.read((char*) &subLength, 2);
    bytesRead += 2;
    if (subLength!=4)
    {
      std::cout <<"Error: sub record FULL of WOOP has invalid length ("<<subLength
                <<" bytes). Should be four bytes.\n";
      return false;
    }
    //read FULL
    in_File.read((char*) &nameStringID, 4);
    bytesRead += 4;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord FULL of WOOP!\n";
      return false;
    }
    hasFULL = true;

    //read TNAM
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
  }
  else
  {
    //full subrecord is not present
    hasFULL = false;
  }

  //read TNAM
  //  --> was read before
  if (subRecName!=cTNAM)
  {
    UnexpectedRecord(cTNAM, subRecName);
    return false;
  }
  //TNAM's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=4)
  {
    std::cout <<"Error: sub record TNAM of WOOP has invalid length ("<<subLength
              <<" bytes). Should be four bytes.\n";
    return false;
  }
  //read TNAM
  in_File.read((char*) &translatedStringID, 4);
  bytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord TNAM of WOOP!\n";
    return false;
  }

  return true;
}

} //namespace
