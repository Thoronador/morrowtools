/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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

#include "ClassRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

ClassRecord::ClassRecord()
: BasicRecord()
{
  editorID = "";
  nameStringID = 0;
  unknownDESC = 0;
  memset(unknownDATA, 0, 36);
}

ClassRecord::~ClassRecord()
{
  //empty
}

bool ClassRecord::equals(const ClassRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
    and (nameStringID==other.nameStringID) and (unknownDESC==other.unknownDESC)
    and (memcmp(unknownDATA, other.unknownDATA, 36)==0));
}

bool ClassRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cCLAS, 4);
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed length */
        +4 /* DESC */ +2 /* 2 bytes for length */ +4 /* fixed length */
        +4 /* DATA */ +2 /* 2 bytes for length */ +36 /* fixed length */;
  if (!saveSizeAndUnknownValues(output, writeSize)) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write FULL
  output.write((const char*) &cFULL, 4);
  //FULL's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write FULL's stuff
  output.write((const char*) &nameStringID, 4);

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

bool ClassRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: subrecord FULL of CLAS has invalid length ("<<subLength
              <<" bytes). Should be four bytes!\n";
    return false;
  }
  //read FULL's stuff
  in_File.read((char*) &nameStringID, 4);
  bytesRead += 4;
  if (!in_File.good())
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

int32_t ClassRecord::getRecordType() const
{
  return cCLAS;
}

} //namespace
