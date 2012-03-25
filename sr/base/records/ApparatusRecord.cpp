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

#include "ApparatusRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

ApparatusRecord::ApparatusRecord()
: BasicRecord()
{
  editorID = "";
  memset(unknownOBND, 0, 12);
  nameStringID = 0;
  quality = 0;
  unknownDESC = 0;
  memset(unknownDATA, 0, 8);
}

ApparatusRecord::~ApparatusRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ApparatusRecord::equals(const ApparatusRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (nameStringID==other.nameStringID) and (quality==other.quality)
      and (unknownDESC==other.unknownDESC)
      and (memcmp(unknownDATA, other.unknownDATA, 8)==0));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ApparatusRecord::getWriteSize() const
{
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length */
        +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed length */
        +4 /* QUAL */ +2 /* 2 bytes for length */ +4 /* fixed length */
        +4 /* DESC */ +2 /* 2 bytes for length */ +4 /* fixed length */
        +4 /* DATA */ +2 /* 2 bytes for length */ +8 /* fixed length */);
}

bool ApparatusRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cAPPA, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write OBND
  output.write((const char*) unknownOBND, 12);

  //write FULL
  output.write((const char*) &cFULL, 4);
  //FULL's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write FULL's data
  output.write((const char*) &nameStringID, 4);

  //write QUAL
  output.write((const char*) &cQUAL, 4);
  //QUAL's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write QUAL's data
  output.write((const char*) &quality, 4);

  //write DESC
  output.write((const char*) &cDESC, 4);
  //DESC's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write DESC's data
  output.write((const char*) &unknownDESC, 4);

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 8; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) unknownDATA, 8);

  return output.good();
}
#endif

bool ApparatusRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
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
    std::cout <<"Error: sub record EDID of APPA is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of APPA!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read OBND
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  //OBND's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=12)
  {
    std::cout <<"Error: sub record OBND of APPA has invalid length ("
              <<subLength<<" bytes. Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of APPA!\n";
    return false;
  }

  //read FULL
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cFULL)
  {
    UnexpectedRecord(cFULL, subRecName);
    return false;
  }
  //FULL's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=4)
  {
    std::cout <<"Error: sub record FULL of APPA has invalid length ("
              <<subLength<<" bytes. Should be four bytes!\n";
    return false;
  }
  //read FULL's stuff
  in_File.read((char*) &nameStringID, 4);
  bytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FULL of APPA!\n";
    return false;
  }

  //read QUAL
  if (!loadUint32SubRecordFromStream(in_File, cQUAL, quality, true))
  {
    std::cout << "Error while reading subrecord QUAL of APPA!\n";
    return false;
  }

  //read DESC
  if (!loadUint32SubRecordFromStream(in_File, cDESC, unknownDESC, true))
  {
    std::cout << "Error while reading subrecord DESC of APPA!\n";
    return false;
  }

  //read DATA
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, subRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=8)
  {
    std::cout <<"Error: sub record DATA of APPA has invalid length ("
              <<subLength<<" bytes. Should be 8 bytes!\n";
    return false;
  }
  //read DATA's stuff
  in_File.read((char*) unknownDATA, 8);
  bytesRead += 8;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of APPA!\n";
    return false;
  }

  return in_File.good();
}

uint32_t ApparatusRecord::getRecordType() const
{
  return cAPPA;
}

} //namespace
