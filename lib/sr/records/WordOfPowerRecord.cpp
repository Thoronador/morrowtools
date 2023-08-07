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

#include "WordOfPowerRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

WordOfPowerRecord::WordOfPowerRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  translated(LocalizedString())
{
}

uint32_t WordOfPowerRecord::getRecordType() const
{
  return cWOOP;
}

#ifndef SR_NO_RECORD_EQUALITY
bool WordOfPowerRecord::equals(const WordOfPowerRecord& other) const
{
  return (equalsBasic(other) && (editorID == other.editorID)
      && (name == other.name) && (translated == other.translated));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t WordOfPowerRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + translated.getWriteSize() /* TNAM */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  return writeSize;
}

bool WordOfPowerRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cWOOP), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write EDID
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  // EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write editor ID
  output.write(editorID.c_str(), subLength);

  if (name.isPresent())
  {
    // write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  // write TNAM
  if (!translated.saveToStream(output, cTNAM))
    return false;

  return output.good();
}
#endif

bool WordOfPowerRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
  {
    return false;
  }
  uint32_t subRecName = 0;
  uint32_t bytesRead = 0;

  // read EDID
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
  {
    return false;
  }

  // read optional FULL
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName == cFULL)
  {
    // FULL subrecord is present
    // read FULL
    if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
    {
      std::cerr << "Error while reading subrecord FULL of WOOP!\n";
      return false;
    }

    // read TNAM
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
  }
  else
  {
    // full subrecord is not present
    name.reset();
  }

  // read TNAM
  //   --> header was read before
  if (subRecName != cTNAM)
  {
    UnexpectedRecord(cTNAM, subRecName);
    return false;
  }
  // read TNAM
  if (!translated.loadFromStream(in_File, cTNAM, false, bytesRead, localized, table, buffer))
  {
    std::cerr << "Error while reading subrecord TNAM of WOOP!\n";
    return false;
  }

  return true;
}

} // namespace
