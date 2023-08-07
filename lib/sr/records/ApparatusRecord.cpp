/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Dirk Stolle

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

#include "ApparatusRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

ApparatusRecord::ApparatusRecord()
: BasicRecord(), editorID(""),
  unknownOBND(std::array<uint8_t, 12>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}),
  name(LocalizedString()),
  quality(0),
  description(LocalizedString()),
  value(0),
  weight(0.0f)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool ApparatusRecord::equals(const ApparatusRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownOBND == other.unknownOBND)
      && (name == other.name) && (quality == other.quality)
      && (description == other.description)
      && (value == other.value) && (weight == other.weight);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ApparatusRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  return 4 /* EDID */ +2 /* 2 bytes for length */
      + editorID.length() + 1 /* length of name +1 byte for NUL termination */
      + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed length */
      + name.getWriteSize() /* FULL */
      + 4 /* QUAL */ + 2 /* 2 bytes for length */ + 4 /* fixed length */
      + description.getWriteSize() /* DESC */
      + 4 /* DATA */ + 2 /* 2 bytes for length */ + 8 /* fixed length */;
}

bool ApparatusRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cAPPA), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  // write object bounds (OBND)
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  subLength = 12; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  // write FULL
  if (!name.saveToStream(output, cFULL))
  {
    std::cerr << "Error while writing subrecord FULL of APPA!\n";
    return false;
  }

  // write quality (QUAL)
  output.write(reinterpret_cast<const char*>(&cQUAL), 4);
  subLength = 4; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&quality), 4);

  // write description (DESC)
  if (!description.saveToStream(output, cDESC))
  {
    std::cerr << "Error while writing subrecord DESC of APPA!\n";
    return false;
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = 8; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write DATA's content
  output.write(reinterpret_cast<const char*>(&value), 4);
  output.write(reinterpret_cast<const char*>(&weight), 4);

  return output.good();
}
#endif

bool ApparatusRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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

  // read object bounds (OBND)
  in_File.read(reinterpret_cast<char*>(&subRecName), 4);
  bytesRead += 4;
  if (subRecName != cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  // OBND's length
  in_File.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength != 12)
  {
    std::cerr << "Error: Sub record OBND of APPA has invalid length ("
              << subLength << " bytes). Should be 12 bytes!\n";
    return false;
  }
  // read OBND's stuff
  in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord OBND of APPA!\n";
    return false;
  }

  // read FULL
  if (!name.loadFromStream(in_File, cFULL, true, bytesRead, localized, table, buffer))
  {
    std::cerr << "Error while reading subrecord FULL of APPA!\n";
    return false;
  }

  // read QUAL
  if (!loadUint32SubRecordFromStream(in_File, cQUAL, quality, true))
  {
    std::cerr << "Error while reading subrecord QUAL of APPA!\n";
    return false;
  }
  bytesRead += 10;

  // read DESC
  if (!description.loadFromStream(in_File, cDESC, true, bytesRead, localized, table, buffer))
  {
    std::cerr << "Error while reading subrecord DESC of APPA!\n";
    return false;
  }

  // read DATA
  in_File.read(reinterpret_cast<char*>(&subRecName), 4);
  bytesRead += 4;
  if (subRecName != cDATA)
  {
    UnexpectedRecord(cDATA, subRecName);
    return false;
  }
  // DATA's length
  in_File.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength != 8)
  {
    std::cerr << "Error: Sub record DATA of APPA has invalid length ("
              << subLength << " bytes). Should be 8 bytes!\n";
    return false;
  }
  // read DATA's stuff
  in_File.read(reinterpret_cast<char*>(&value), 4);
  in_File.read(reinterpret_cast<char*>(&weight), 4);
  bytesRead += 8;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord DATA of APPA!\n";
    return false;
  }

  return in_File.good();
}

uint32_t ApparatusRecord::getRecordType() const
{
  return cAPPA;
}

} // namespace
