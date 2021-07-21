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

#include "VoiceTypeRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{


const uint8_t VoiceTypeRecord::cAllowDefaultDialogue = 0x01;
const uint8_t VoiceTypeRecord::cFemale = 0x02;

VoiceTypeRecord::VoiceTypeRecord()
: BasicRecord(), editorID(""),
  flags(0)
{

}

VoiceTypeRecord::~VoiceTypeRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool VoiceTypeRecord::equals(const VoiceTypeRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (flags==other.flags));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t VoiceTypeRecord::getWriteSize() const
{
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +1 /* fixed length */);
}

bool VoiceTypeRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cVTYP, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 1;
  output.write((const char*) &subLength, 2);
  //write DNAM's stuff
  output.write((const char*) &flags, 1);

  return output.good();
}
#endif

bool VoiceTypeRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  //uint32_t bytesRead;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  //bytesRead = 4;
  if (subRecName!=cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  //EDID's length
  in_File.read((char*) &subLength, 2);
  //bytesRead += 2;
  if (subLength>511)
  {
    std::cout <<"Error: sub record EDID of VTYP is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  //bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of VTYP!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read DNAM
  in_File.read((char*) &subRecName, 4);
  //bytesRead += 4;
  if (subRecName!=cDNAM)
  {
    UnexpectedRecord(cDNAM, subRecName);
    return false;
  }
  //DNAM's length
  in_File.read((char*) &subLength, 2);
  //bytesRead += 2;
  if (subLength!=1)
  {
    std::cout <<"Error: subrecord DNAM of VTYP has invalid length ("<<subLength
              <<" bytes). Should be one byte!\n";
    return false;
  }
  //read DNAM's stuff
  in_File.read((char*) &flags, 1);
  //bytesRead += 1;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DNAM of VTYP!\n";
    return false;
  }

  return in_File.good();
}

uint32_t VoiceTypeRecord::getRecordType() const
{
  return cVTYP;
}

bool VoiceTypeRecord::isFemale() const
{
  return ((flags & cFemale)!=0);
}

bool VoiceTypeRecord::allowsDefaultDialogue() const
{
  return ((flags & cAllowDefaultDialogue)!=0);
}

} //namespace
