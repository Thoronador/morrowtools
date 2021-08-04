/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Thoronador

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

#include "KeywordRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

KeywordRecord::KeywordRecord()
: BasicRecord(), editorID(""),
  unknownCNAM(0), hasCNAM(false)
{
}

KeywordRecord::~KeywordRecord()
{
}

uint32_t KeywordRecord::getRecordType() const
{
  return cKYWD;
}

#ifndef SR_NO_RECORD_EQUALITY
bool KeywordRecord::equals(const KeywordRecord& other) const
{
  if ((editorID == other.editorID) && (hasCNAM == other.hasCNAM)
    && (equalsBasic(other)))
  {
    if (hasCNAM)
      return (unknownCNAM == other.unknownCNAM);
    return true;
  }
  return false;
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t KeywordRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */;
  if (hasCNAM)
  {
    writeSize = writeSize + 4 /* CNAM */ + 2 /* 2 bytes for length */
               + 4 /* fixed length of 4 bytes */;
  }
  return writeSize;
}

bool KeywordRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cKYWD), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  // write EDID
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  // EDID's length
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write editor ID
  output.write(editorID.c_str(), subLength);
  if (!output.good())
  {
    std::cerr << "Error while writing subrecord EDID of KYWD!\n";
    return false;
  }

  if (hasCNAM)
  {
    // write CNAM
    output.write(reinterpret_cast<const char*>(&cCNAM), 4);
    // CNAM's length
    subLength = 4; /* fixed length of four bytes */
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write CNAM stuff
    output.write(reinterpret_cast<const char*>(&unknownCNAM), 4);
  }

  return output.good();
}
#endif

bool KeywordRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
  {
    return false;
  }
  uint32_t subRecName = 0;
  uint16_t subLength = 0;

  // read EDID
  in_File.read(reinterpret_cast<char*>(&subRecName), 4);
  uint32_t bytesRead = 4;
  if (subRecName != cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  // EDID's length
  in_File.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength > 511)
  {
    std::cerr << "Error: sub record EDID of KYWD is longer than 511 characters!\n";
    return false;
  }
  // read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of KYWD!\n";
    return false;
  }
  editorID = std::string(buffer);

  if (bytesRead < readSize)
  {
    hasCNAM = true;
    // read CNAM
    if (!loadUint32SubRecordFromStream(in_File, cCNAM, unknownCNAM, true))
      return false;
  }
  else
  {
    hasCNAM = false;
    unknownCNAM = 0;
  }

  return true;
}

} // namespace
