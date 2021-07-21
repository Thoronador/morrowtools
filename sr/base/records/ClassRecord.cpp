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

#include "ClassRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

ClassRecord::ClassRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  unknownDESC(0)
{
  memset(unknownDATA, 0, 36);
}

ClassRecord::~ClassRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ClassRecord::equals(const ClassRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
    and (name==other.name) and (unknownDESC==other.unknownDESC)
    and (memcmp(unknownDATA, other.unknownDATA, 36)==0));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ClassRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  return ( 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed length */
        +4 /* DESC */ +2 /* 2 bytes for length */ +4 /* fixed length */
        +4 /* DATA */ +2 /* 2 bytes for length */ +36 /* fixed length */);
}

bool ClassRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cCLAS, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write FULL
  if (!name.saveToStream(output, cFULL))
    return false;

  //write DESC
  output.write((const char*) &cDESC, 4);
  //DESC's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write DESC's stuff
  output.write((const char*) &unknownDESC, 4);

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 36;
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) unknownDATA, 36);

  return output.good();
}
#endif

bool ClassRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
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
    std::cout <<"Error: sub record EDID of CLASS is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of CLASS!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read FULL
  if (!name.loadFromStream(in_File, cFULL, true, bytesRead, localized, table, buffer))
  {
    std::cout << "Error while reading subrecord FULL of CLAS!\n";
    return false;
  }//if

  //read DESC
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cDESC)
  {
    UnexpectedRecord(cDESC, subRecName);
    return false;
  }
  //DESC's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=4)
  {
    std::cout <<"Error: subrecord DESC of CLAS has invalid length ("<<subLength
              <<" bytes). Should be four bytes!\n";
    return false;
  }
  //read DESC's stuff
  in_File.read((char*) &unknownDESC, 4);
  bytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DESC of CLAS!\n";
    return false;
  }//if

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
  if (subLength!=36)
  {
    std::cout <<"Error: subrecord DATA of CLAS has invalid length ("<<subLength
              <<" bytes). Should be 36 bytes!\n";
    return false;
  }
  //read DATA's stuff
  in_File.read((char*) unknownDATA, 36);
  bytesRead += 36;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of CLAS!\n";
    return false;
  }//if

  return in_File.good();
}

uint32_t ClassRecord::getRecordType() const
{
  return cCLAS;
}

} //namespace
