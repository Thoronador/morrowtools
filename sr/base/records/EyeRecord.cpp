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

#include "EyeRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

const uint8_t EyeRecord::FlagPlayable = 0x01;
const uint8_t EyeRecord::FlagFemale = 0x02;
const uint8_t EyeRecord::FlagMale = 0x04;

EyeRecord::EyeRecord()
: BasicRecord()
{
  editorID = "";
  nameStringID = 0;
  iconPath = "";
  flags = 0;
}

EyeRecord::~EyeRecord()
{
  //empty
}

int32_t EyeRecord::getRecordType() const
{
  return cEYES;
}

#ifndef SR_NO_RECORD_EQUALITY
bool EyeRecord::equals(const EyeRecord& other) const
{
  return ((editorID==other.editorID) and (nameStringID==other.nameStringID)
      and (iconPath==other.iconPath) and (flags==other.flags)
      and equalsBasic(other));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t EyeRecord::getWriteSize() const
{
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */
        +4 /* ICON */ +2 /* 2 bytes for length */
        +iconPath.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +1 /* fixed length of one byte */);
}

bool EyeRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cEYES, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write FULL
  output.write((char*) &cFULL, 4);
  //FULL's length
  subLength = 4;
  output.write((char*) &subLength, 2);
  //write FULL's data
  output.write((char*) &nameStringID, 4);

  //write ICON
  output.write((char*) &cICON, 4);
  //ICON's length
  subLength = iconPath.length()+1;
  output.write((char*) &subLength, 2);
  //write icon path
  output.write(iconPath.c_str(), subLength);

  //write DATA
  output.write((char*) &cDATA, 4);
  //DATA's length
  subLength = 1;
  output.write((char*) &subLength, 2);
  //write DATA's content
  output.write((char*) &flags, 1);

  return output.good();
}
#endif

bool EyeRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
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
    std::cout <<"Error: sub record EDID of EYES is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of EYES!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read FULL
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cFULL)
  {
    UnexpectedRecord(cFULL, subRecName);
    return false;
  }
  //FULL's length
  in_File.read((char*) &subLength, 2);
  if (subLength!=4)
  {
    std::cout <<"Error: sub record FULL of EYES has invalid length ("<<subLength
              <<" bytes). Should be four bytes.\n";
    return false;
  }
  //read FULL
  in_File.read((char*) &nameStringID, 4);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FULL of EYES!\n";
    return false;
  }

  //read ICON
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cICON)
  {
    UnexpectedRecord(cICON, subRecName);
    return false;
  }
  //ICON's length
  in_File.read((char*) &subLength, 2);
  if (subLength>511)
  {
    std::cout <<"Error: sub record ICON of EYES is longer than 511 characters!\n";
    return false;
  }
  //read ICON path
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord ICON of EYES!\n";
    return false;
  }
  iconPath = std::string(buffer);

  //read DATA
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, subRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &subLength, 2);
  if (subLength!=1)
  {
    std::cout <<"Error: sub record DATA of EYES has invalid length ("<<subLength
              <<" bytes). Should be one byte.\n";
    return false;
  }
  //read DATA
  in_File.read((char*) &flags, 1);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FULL of EYES!\n";
    return false;
  }

  return true;
}

/* flag data - rather confusing and not straightforward
   0x00 (b000) - male + female flag set
   0x01 (b001) - all three flags set
   0x02 (b010) - only female flag set
   0x03 (b011) - playable + female
   0x04 (b100) - only male flag set
   0x05 (b101) - playable + male
   0x07 (b111) - only playable flag set
*/

bool EyeRecord::isPlayable() const
{
  //still quite straightforward
  return ((flags & FlagPlayable) != 0);
}

bool EyeRecord::canBeMale() const
{
  return ((((flags & FlagMale) != 0) and (flags!=0x07)) or (flags<0x01));
}

bool EyeRecord::canBeFemale() const
{
  return (flags <= 0x03);
}

} //namespace
