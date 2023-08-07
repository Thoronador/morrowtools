/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2022  Dirk Stolle

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

#include "EyeRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

const uint8_t EyeRecord::FlagPlayable = 0x01;
const uint8_t EyeRecord::FlagFemale = 0x02;
const uint8_t EyeRecord::FlagMale = 0x04;

EyeRecord::EyeRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  iconPath(""),
  flags(0)
{

}

uint32_t EyeRecord::getRecordType() const
{
  return cEYES;
}

#ifndef SR_NO_RECORD_EQUALITY
bool EyeRecord::equals(const EyeRecord& other) const
{
  return (editorID == other.editorID) && (name == other.name)
      && (iconPath == other.iconPath) && (flags == other.flags)
      && equalsBasic(other);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t EyeRecord::getWriteSize() const
{
  return 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + name.getWriteSize() /* FULL */
        + 4 /* ICON */ + 2 /* 2 bytes for length */
        + iconPath.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* DATA */ + 2 /* 2 bytes for length */ +1 /* fixed length of one byte */;
}

bool EyeRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cEYES, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;

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

  //write ICON
  output.write((const char*) &cICON, 4);
  //ICON's length
  subLength = iconPath.length()+1;
  output.write((const char*) &subLength, 2);
  //write icon path
  output.write(iconPath.c_str(), subLength);

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 1;
  output.write((const char*) &subLength, 2);
  //write DATA's content
  output.write((const char*) &flags, 1);

  return output.good();
}
#endif

bool EyeRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;

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
    std::cerr <<"Error: sub record EDID of EYES is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of EYES!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read FULL
  if (!name.loadFromStream(in_File, cFULL, true, subRecName, localized, table, buffer))
  {
    std::cerr << "Error while reading subrecord FULL of EYES!\n";
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
    std::cerr <<"Error: sub record ICON of EYES is longer than 511 characters!\n";
    return false;
  }
  //read ICON path
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord ICON of EYES!\n";
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
    std::cerr <<"Error: sub record DATA of EYES has invalid length ("<<subLength
              <<" bytes). Should be one byte.\n";
    return false;
  }
  //read DATA
  in_File.read((char*) &flags, 1);
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord FULL of EYES!\n";
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
  return ((((flags & FlagMale) != 0) && (flags != 0x07)) || (flags < 0x01));
}

bool EyeRecord::canBeFemale() const
{
  return (flags <= 0x03);
}

} // namespace
