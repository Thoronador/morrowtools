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

#include "PerkRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

PerkRecord::PerkRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  description(LocalizedString()),
  subBlocks(std::vector<SubBlock>())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool PerkRecord::equals(const PerkRecord& other) const
{
  return (equalsBasic(other) && (editorID == other.editorID)
      && (name == other.name) && (description == other.description)
      && (subBlocks == other.subBlocks));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t PerkRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + description.getWriteSize() /* DESC */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }
  for (const auto& block: subBlocks)
  {
    writeSize = writeSize + 4 /* header */ + 2 /* 2 bytes for length */
               + block.subData.size() /* length */;
  }
  return writeSize;
}

bool PerkRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cPERK), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write EDID (editorID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  if (name.isPresent())
  {
    // write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  // write DESC
  if (!description.saveToStream(output, cDESC))
    return false;

  for (const auto& block: subBlocks)
  {
    if (!block.subData.saveToStream(output, block.subType))
    {
      std::cerr << "Error while writing (binary) subrecord "
                << IntTo4Char(block.subType) << " of PERK!\n";
      return false;
    }
  }

  return output.good();
}
#endif

bool PerkRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  uint32_t subRecName = 0;
  uint32_t bytesRead = 0;

  // read EDID
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
    return false;

  // now read the rest
  name.reset();
  description.reset();
  subBlocks.clear();
  SubBlock temp;
  while (bytesRead < readSize)
  {
    // read next subrecord's name
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: Record PERK seems to have more than one FULL subrecord.\n";
             return false;
           }
           // read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cDESC:
           if (description.isPresent())
           {
             std::cerr << "Error: Record PERK seems to have more than one DESC subrecord.\n";
             return false;
           }
           // read DESC
           if (!description.loadFromStream(in_File, cDESC, false, bytesRead, localized, table, buffer))
             return false;
           break;
      default:
           temp.subType = subRecName;
           if (!temp.subData.loadFromStream(in_File, subRecName, false))
           {
             std::cerr << "Error while reading subrecord "
                       << IntTo4Char(subRecName) << " of PERK!\n";
             return false;
           }
           bytesRead += (2 + temp.subData.size());
           subBlocks.push_back(temp);
           break;
    }
  }

  if (!description.isPresent())
  {
    std::cerr << "Error: Subrecord DESC of PERK is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t PerkRecord::getRecordType() const
{
  return cPERK;
}

} // namespace
