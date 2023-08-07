/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2022  Dirk Stolle

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

#include "FormListRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

FormListRecord::FormListRecord()
: BasicRecord(),
  editorID(""),
  listFormIDs(std::vector<uint32_t>())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool FormListRecord::equals(const FormListRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (listFormIDs == other.listFormIDs);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t FormListRecord::getWriteSize() const
{
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
      + editorID.length() + 1 /* length of string +1 byte for NUL-termination */
      + listFormIDs.size() *
            (4 /* LNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */);
  return writeSize;
}

bool FormListRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cFLST), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  for (const uint32_t id: listFormIDs)
  {
    // write form ID (LNAM)
    output.write(reinterpret_cast<const char*>(&cLNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&id), 4);
  }

  return output.good();
}
#endif

bool FormListRecord::loadFromStream(std::istream& input, [[maybe_unused]] const bool localized, [[maybe_unused]] const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(input, readSize))
    return false;
  uint32_t bytesRead = 0;

  //read EDID
  char buffer[512];
  if (!loadString512FromStream(input, editorID, buffer, cEDID, true, bytesRead))
  {
    std::cerr << "Error while reading sub record EDID of FLST!\n";
    return false;
  }

  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  listFormIDs.clear();
  uint32_t temp_fID;
  while (bytesRead < readSize)
  {
    // read LNAM
    input.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    if (subRecName != cLNAM)
    {
      UnexpectedRecord(cLNAM, subRecName);
      return false;
    }
    // LNAM's length
    input.read(reinterpret_cast<char*>(&subLength), 2);
    bytesRead += 2;
    if (subLength != 4)
    {
      std::cerr << "Error: Sub record LNAM of FLST has invalid length("
                << subLength << " bytes). Should be four bytes!\n";
      return false;
    }
    // read LNAM's stuff
    input.read(reinterpret_cast<char*>(&temp_fID), 4);
    bytesRead += 4;
    if (!input.good())
    {
      std::cerr << "Error while reading sub record LNAM of FLST!\n";
      return false;
    }
    listFormIDs.push_back(temp_fID);
  }

  return input.good();
}

uint32_t FormListRecord::getRecordType() const
{
  return cFLST;
}

} // namespace
