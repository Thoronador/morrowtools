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

#include "ActionRecord.hpp"
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"
#include <cstring>
#include <iostream>

namespace SRTP
{

ActionRecord::ActionRecord()
: BasicRecord(), editorID("")
{

}

ActionRecord::~ActionRecord()
{
  //empty
}

uint32_t ActionRecord::getRecordType() const
{
  return cAACT;
}

#ifndef SR_NO_RECORD_EQUALITY
bool ActionRecord::equals(const ActionRecord& other) const
{
  return (equalsBasic(other) and (editorID==other.editorID));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ActionRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */);
}

bool ActionRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cAACT, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  return output.good();
}
#endif

bool ActionRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  if (isDeleted()) return true;
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
    std::cout <<"Error: sub record EDID of AACT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of AACT!\n";
    return false;
  }
  editorID = std::string(buffer);

  return true;
}

} //namespace
