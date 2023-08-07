/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2022  Dirk Stolle

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
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

KeywordRecord::KeywordRecord()
: BasicRecord(), editorID(""),
  unknownCNAM(std::optional<uint32_t>())
{
}

uint32_t KeywordRecord::getRecordType() const
{
  return cKYWD;
}

#ifndef SR_NO_RECORD_EQUALITY
bool KeywordRecord::equals(const KeywordRecord& other) const
{
  return (editorID == other.editorID) && (unknownCNAM == other.unknownCNAM)
    && equalsBasic(other);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t KeywordRecord::getWriteSize() const
{
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
      + editorID.length() + 1 /* length of name +1 byte for NUL termination */;
  if (unknownCNAM.has_value())
  {
    writeSize += 4 /* CNAM */ + 2 /* 2 bytes for length */
               + 4 /* fixed length of 4 bytes */;
  }
  return writeSize;
}

bool KeywordRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cKYWD), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);
  if (!output.good())
  {
    std::cerr << "Error while writing sub record EDID of KYWD!\n";
    return false;
  }

  if (unknownCNAM.has_value())
  {
    // write CNAM
    output.write(reinterpret_cast<const char*>(&cCNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownCNAM.value()), 4);
  }

  return output.good();
}
#endif

bool KeywordRecord::loadFromStream(std::istream& input,
                                   [[maybe_unused]] const bool localized,
                                   [[maybe_unused]] const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(input, readSize))
  {
    return false;
  }

  // read editor ID (EDID)
  uint32_t bytesRead = 0;
  char buffer[512];
  if (!loadString512FromStream(input, editorID, buffer, cEDID, true, bytesRead))
    return false;

  // Is there still more to read?
  if (bytesRead < readSize)
  {
    // read CNAM
    uint32_t dummy = 0;
    if (!loadUint32SubRecordFromStream(input, cCNAM, dummy, true))
      return false;
    unknownCNAM = dummy;
  }
  else
  {
    unknownCNAM = {};
  }

  return true;
}

} // namespace
