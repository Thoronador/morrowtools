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

#include "AcousticSpaceRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

AcousticSpaceRecord::AcousticSpaceRecord()
: BasicRecord(), editorID(""),
  unknownOBND(std::array<uint8_t, 12>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })),
  loopingSoundFormID(0),
  regionFormID(0),
  environmentTypeFormID(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool AcousticSpaceRecord::equals(const AcousticSpaceRecord& other) const
{
  return ((equalsBasic(other)) && (editorID == other.editorID)
      && (unknownOBND == other.unknownOBND)
      && (loopingSoundFormID == other.loopingSoundFormID)
      && (regionFormID == other.regionFormID)
      && (environmentTypeFormID == other.environmentTypeFormID));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t AcousticSpaceRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size */;
  if (loopingSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* SNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (regionFormID != 0)
  {
    writeSize = writeSize + 4 /* RDAT */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (environmentTypeFormID != 0)
  {
    writeSize = writeSize + 4 /* BNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  return writeSize;
}

bool AcousticSpaceRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cASPC), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write EDID
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  // EDID's length
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write editor ID
  output.write(editorID.c_str(), subLength);

  // write OBND
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  // OBND's length
  subLength = 12;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write OBND
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  if (loopingSoundFormID != 0)
  {
    // write SNAM
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    // SNAM's length
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write form ID of looping sound
    output.write(reinterpret_cast<const char*>(&loopingSoundFormID), 4);
  } // if has SNAM subrecord

  if (regionFormID != 0)
  {
    // write RDAT
    output.write(reinterpret_cast<const char*>(&cRDAT), 4);
    // RDAT's length
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write region form ID
    output.write(reinterpret_cast<const char*>(&regionFormID), 4);
  } // if has RDAT subrecord

  if (environmentTypeFormID != 0)
  {
    // write BNAM
    output.write(reinterpret_cast<const char*>(&cBNAM), 4);
    // BNAM's length
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write environment type
    output.write(reinterpret_cast<const char*>(&environmentTypeFormID), 4);
  } // if has BNAM subrecord

  return output.good();
}
#endif

bool AcousticSpaceRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  // read EDID
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
    return false;

  // read OBND
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
    std::cerr << "Error: sub record OBND of ASPC has invalid length ("
              << subLength << " bytes). Should be 12 bytes.\n";
    return false;
  }
  // read OBND
  in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord OBND of ASPC!\n";
    return false;
  }

  loopingSoundFormID = 0;
  regionFormID = 0;
  environmentTypeFormID = 0;
  while (bytesRead < readSize)
  {
    // read next header
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cSNAM:
           if (loopingSoundFormID != 0)
           {
             std::cerr << "Error: ASPC seems to have more than one SNAM subrecord.\n";
             return false;
           }
           // read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, loopingSoundFormID, false))
             return false;
           bytesRead += 6;
           // check value
           if (loopingSoundFormID == 0)
           {
             std::cerr << "Error: subrecord SNAM of ASPC has value zero!\n";
             return false;
           }
           break;
      case cRDAT:
           if (regionFormID != 0)
           {
             std::cerr << "Error: ASPC seems to have more than one RDAT subrecord.\n";
             return false;
           }
           // read RDAT
           if (!loadUint32SubRecordFromStream(in_File, cRDAT, regionFormID, false))
             return false;
           bytesRead += 6;
           // check value
           if (regionFormID == 0)
           {
             std::cerr << "Error: subrecord RDAT of ASPC has value zero!\n";
             return false;
           }
           break;
      case cBNAM:
           if (environmentTypeFormID != 0)
           {
             std::cerr << "Error: ASPC seems to have more than one BNAM subrecord.\n";
             return false;
           }
           // read BNAM
           if (!loadUint32SubRecordFromStream(in_File, cBNAM, environmentTypeFormID, false))
             return false;
           bytesRead += 6;
           // check value
           if (environmentTypeFormID == 0)
           {
             std::cerr << "Error: subrecord BNAM of ASPC has value zero!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: found unexpected subrecord \"" << IntTo4Char(subRecName)
                     << "\", but only SNAM, RDAT or BNAM are allowed here!\n";
           return false;
    } // swi
  } // while

  return in_File.good();
}

uint32_t AcousticSpaceRecord::getRecordType() const
{
  return cASPC;
}

} // namespace
