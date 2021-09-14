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

#include "ShoutRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

bool ShoutRecord::WordEntry::operator==(const ShoutRecord::WordEntry& other) const
{
  return ((wordFormID == other.wordFormID) && (spellFormID == other.spellFormID)
      && (recharge == other.recharge));
}

ShoutRecord::ShoutRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  menuDisplayObjectFormID(0),
  description(LocalizedString()),
  words(std::vector<WordEntry>())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool ShoutRecord::equals(const ShoutRecord& other) const
{
  return (equalsBasic(other) && (editorID == other.editorID)
      && (name == other.name)
      && (menuDisplayObjectFormID == other.menuDisplayObjectFormID)
      && (description == other.description) && (words == other.words));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ShoutRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
      + editorID.length() + 1 /* length of string +1 byte for NUL-termination */
      + description.getWriteSize() /* DESC */
      + words.size()
      * (4 /* SNAM */ + 2 /* 2 bytes for length */ + 12 /* fixed size */);
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  if (menuDisplayObjectFormID != 0)
  {
    writeSize = writeSize + 4 /* MDOB */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  return writeSize;
}

bool ShoutRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cSHOU), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write editor ID (EDID)
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

  if (menuDisplayObjectFormID!=0)
  {
    // write Menu Display Object's form ID (MDOB)
    output.write(reinterpret_cast<const char*>(&cMDOB), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&menuDisplayObjectFormID), 4);
  }

  // write description (DESC)
  if (!description.saveToStream(output, cDESC))
    return false;

  for (const auto& word: words)
  {
    // write SNAM
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    // SNAM's length
    subLength = 12; // fixed size of 12 bytes
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write word's data
    output.write(reinterpret_cast<const char*>(&word.wordFormID), 4);
    output.write(reinterpret_cast<const char*>(&word.spellFormID), 4);
    output.write(reinterpret_cast<const char*>(&word.recharge), 4);
  }

  return output.good();
}
#endif

bool ShoutRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  // read editor ID (EDID)
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
    return false;

  name.reset();
  menuDisplayObjectFormID = 0;
  description.reset();
  words.clear();
  WordEntry temp;

  while (bytesRead < readSize)
  {
    // read next subrecord header
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: SHOU seems to have more than one FULL subrecord.\n";
             return false;
           }
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMDOB:
           if (menuDisplayObjectFormID != 0)
           {
             std::cerr << "Error: SHOU seems to have more than one MDOB subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cMDOB, menuDisplayObjectFormID, false))
             return false;
           bytesRead += 6;
           // check content
           if (menuDisplayObjectFormID == 0)
           {
             std::cerr << "Error: subrecord MDOB of SHOU is zero!\n";
             return false;
           }
           break;
      case cDESC:
           if (description.isPresent())
           {
             std::cerr << "Error: SHOU seems to have more than one DESC subrecord.\n";
             return false;
           }
           if (!description.loadFromStream(in_File, cDESC, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cSNAM:
           // SNAM's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: sub record SNAM of SHOU has invalid length ("
                       << subLength << " bytes). Should be 12 bytes.\n";
             return false;
           }
           // read SNAM's stuff
           temp.wordFormID = 0;
           temp.spellFormID = 0;
           temp.recharge = 0.0f;
           in_File.read(reinterpret_cast<char*>(&temp.wordFormID), 4);
           in_File.read(reinterpret_cast<char*>(&temp.spellFormID), 4);
           in_File.read(reinterpret_cast<char*>(&temp.recharge), 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord SNAM of SHOU!\n";
             return false;
           }
           words.push_back(temp);
           break;
      default:
           std::cerr << "Error: unexpected record type \""
                     << IntTo4Char(subRecName)
                     << "\" found, but only FULL, MDOB, DESC or SNAM are allowed!\n";
           return false;
           break;
    }
  } // while

  // check for presence of required elements
  if (!description.isPresent() || (words.size() != 3))
  {
    std::cerr << "Error: at least one subrecord of SHOU is not present!\n"
              << "has DESC: " << description.isPresent() << ", SNAM.size = "
              << words.size() << "\n";
    return false;
  }

  return in_File.good();
}

uint32_t ShoutRecord::getRecordType() const
{
  return cSHOU;
}

} // namespace
