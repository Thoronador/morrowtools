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

#include "TES4HeaderRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

Tes4HeaderRecord::MasterFile::MasterFile()
: fileName(""),
  data(0)
{
}

bool Tes4HeaderRecord::MasterFile::operator==(const Tes4HeaderRecord::MasterFile& other) const
{
  return (data == other.data) && (fileName == other.fileName);
}

Tes4HeaderRecord::Tes4HeaderRecord()
: BasicRecord(),
  version(0.0f),
  numRecordsAndGroups(0),
  nextObjectID(0),
  authorName(""),
  summary(""),
  dependencies(std::vector<MasterFile>()),
  unknownONAM(std::vector<uint32_t>()),
  unknownIntValue(0),
  unknownINCC(std::optional<uint32_t>())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool Tes4HeaderRecord::equals(const Tes4HeaderRecord& other) const
{
  return equalsBasic(other) && (version == other.version)
    && (numRecordsAndGroups == other.numRecordsAndGroups)
    && (nextObjectID == other.nextObjectID)
    && (authorName == other.authorName) && (summary == other.summary)
    && (dependencies == other.dependencies) && (unknownONAM == other.unknownONAM)
    && (unknownIntValue == other.unknownIntValue)
    && (unknownINCC == other.unknownINCC);
}
#endif

uint32_t Tes4HeaderRecord::getRecordType() const
{
  return cTES4;
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t Tes4HeaderRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* HEDR */ + 2 /* 2 bytes for length */ + 12 /* fixed length of 12 bytes */
        + 4 /* CNAM */ + 2 /* 2 bytes for length */
        + authorName.size() + 1 /* length of name + 1 byte for NUL termination */
        + 4 /* INTV */ + 2 /* 2 bytes for length */ + 4 /* fixed length of 4 bytes */;
  if (!summary.empty())
  {
    writeSize = writeSize + 4 /* SNAM */ + 2 /* 2 bytes for length */
        + summary.size() + 1; /* length of summary +1 byte for NUL termination */
  }
  for (const auto& elem: dependencies)
  {
    writeSize = writeSize + 4 /* MAST */ + 2 /* 2 bytes for length */
        + elem.fileName.size() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* DATA */ + 2 /* 2 bytes for length */ + 8 /* fixed length of 8 bytes */;
  }
  if (!unknownONAM.empty())
  {
    writeSize = writeSize + 4 /* ONAM */ + 2 /* 2 bytes for length */
                +4 * unknownONAM.size(); /* four bytes per value */
  }
  if (unknownINCC.has_value())
  {
    writeSize = writeSize + 4 /* INCC */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  return writeSize;
}

bool Tes4HeaderRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cTES4), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write HEDR
  output.write(reinterpret_cast<const char*>(&cHEDR), 4);
  // HEDR's length
  uint16_t subLength = 12;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write HEDR's content
  // ---- write version
  output.write(reinterpret_cast<const char*>(&version), 4);
  // ---- write group-record-count + next free form ID
  output.write(reinterpret_cast<const char*>(&numRecordsAndGroups), 4);
  output.write(reinterpret_cast<const char*>(&nextObjectID), 4);
  if (!output.good())
  {
    std::cerr << "Error while writing sub record HEDR of TES4!\n";
    return false;
  }

  // write CNAM
  output.write(reinterpret_cast<const char*>(&cCNAM), 4);
  // CNAM's length
  subLength = authorName.size() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write author's name
  output.write(authorName.c_str(), subLength);

  if (!summary.empty())
  {
    // write SNAM
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    // SNAM's length
    subLength = summary.size() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write summary
    output.write(summary.c_str(), subLength);
  }

  for (const auto& elem : dependencies)
  {
    // write MAST
    output.write(reinterpret_cast<const char*>(&cMAST), 4);
    // MAST's length
    subLength = elem.fileName.size() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write file name
    output.write(elem.fileName.c_str(), subLength);

    // write DATA
    output.write(reinterpret_cast<const char*>(&cDATA), 4);
    // DATA's length
    subLength = 8; // fixed length for int64_t
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write data value
    output.write(reinterpret_cast<const char*>(&(elem.data)), 8);
  }

  if (!unknownONAM.empty())
  {
    // write ONAM
    output.write(reinterpret_cast<const char*>(&cONAM), 4);
    // ONAM's length
    subLength = unknownONAM.size() * 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);

    // write ONAM's data
    for (const uint32_t elem : unknownONAM)
    {
      output.write(reinterpret_cast<const char*>(&elem), 4);
    }
  }

  // write INTV
  output.write(reinterpret_cast<const char*>(&cINTV), 4);
  // INTV's length
  subLength = 4; // fixed size for uint32_t
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write integer value
  output.write(reinterpret_cast<const char*>(&unknownIntValue), 4);

  if (unknownINCC.has_value())
  {
    // write INCC
    output.write(reinterpret_cast<const char*>(&cINCC), 4);
    // INCC's length
    subLength = 4; // fixed size for uint32_t
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write integer value
    output.write(reinterpret_cast<const char*>(&unknownINCC.value()), 4);
  }

  return output.good();
}
#endif

bool Tes4HeaderRecord::loadFromStream(std::istream& in_File,
                                      [[maybe_unused]] const bool localized,
                                      [[maybe_unused]] const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  uint32_t SubRecName = 0;
  uint16_t SubLength = 0;

  // read HEDR
  in_File.read(reinterpret_cast<char*>(&SubRecName), 4);
  uint32_t BytesRead = 4;
  if (SubRecName != cHEDR)
  {
    UnexpectedRecord(cHEDR, SubRecName);
    return false;
  }
  // HEDR's length
  in_File.read(reinterpret_cast<char*>(&SubLength), 2);
  BytesRead += 2;
  if (SubLength != 12)
  {
    std::cerr << "Error: Sub record HEDR of TES4 has invalid length ("
              << SubLength << " bytes). Should be 12 bytes.\n";
    return false;
  }
  // read HEDR's stuff
  // ---- read version number
  in_File.read(reinterpret_cast<char*>(&version), 4);
  // ---- read unknown values
  in_File.read(reinterpret_cast<char*>(&numRecordsAndGroups), 4);
  in_File.read(reinterpret_cast<char*>(&nextObjectID), 4);
  BytesRead += 12;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord HEDR of TES4!\n";
    return false;
  }

  // read CNAM (author's name)
  char buffer[512];
  if (!loadString512FromStream(in_File, authorName, buffer, cCNAM, true, BytesRead))
    return false;

  summary.clear();
  dependencies.clear();
  MasterFile depEntry;
  unknownONAM.clear();
  uint32_t tempUint32;
  unsigned int count;
  bool hasDoneINTV = false;
  unknownINCC = std::nullopt;
  while (BytesRead < readSize)
  {
    // read next subrecord
    in_File.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cSNAM:
           if (!summary.empty())
           {
             std::cerr << "Error: Record TES4 seems to have more than one SNAM sub record!\n";
             return false;
           }
           if (!loadString512FromStream(in_File, summary, buffer, cSNAM, false, BytesRead))
             return false;
           // check: empty strings not allowed
           if (summary.empty())
           {
             std::cerr << "Error: Sub record SNAM of TES4 is empty!\n";
             return false;
           }
           break;
      case cMAST:
           // master file incoming
           if (!loadString512FromStream(in_File, depEntry.fileName, buffer, cMAST, false, BytesRead))
             return false;

           // data coming next...
           // read DATA
           in_File.read(reinterpret_cast<char*>(&SubRecName), 4);
           BytesRead += 4;
           if (SubRecName != cDATA)
           {
             UnexpectedRecord(cDATA, SubRecName);
             return false;
           }
           // DATA's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 2);
           BytesRead += 2;
           if (SubLength != 8)
           {
             std::cerr << "Error: Sub record DATA of TES4 has invalid length ("
                       << SubLength << " bytes). Should be 8 bytes.\n";
             return false;
           }
           // read DATA's stuff
           in_File.read(reinterpret_cast<char*>(&(depEntry.data)), 8);
           BytesRead += 8;
           if (!in_File.good())
           {
             std::cerr << "Error while reading sub record DATA of TES4!\n";
             return false;
           }
           dependencies.emplace_back(depEntry);
           break;
      case cONAM:
           if (!unknownONAM.empty())
           {
             std::cerr << "Error: Record TES4 seems to have more than one ONAM sub record!\n";
             return false;
           }
           // ONAM's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 2);
           BytesRead += 2;
           if ((SubLength <= 0) || ((SubLength % 4) != 0))
           {
             std::cerr << "Error: Sub record ONAM of TES4 has invalid length ("
                       << SubLength << " bytes). Should be an integral multiple"
                       << " of four bytes and larger than zero.\n";
             return false;
           }
           count = SubLength / 4;
           for (unsigned int i = 0; i < count; ++i)
           {
             in_File.read(reinterpret_cast<char*>(&tempUint32), 4);
             BytesRead += 4;
             if (!in_File.good())
             {
               std::cerr << "Error while reading sub record ONAM of TES4!\n";
               return false;
             }
             unknownONAM.push_back(tempUint32);
           }
           break;
      case cINTV:
           if (hasDoneINTV)
           {
             std::cerr << "Error: Record TES4 seems to have more than one INTV sub record!\n";
             return false;
           }
           // read INTV
           if (!loadUint32SubRecordFromStream(in_File, cINTV, unknownIntValue, false))
             return false;
           BytesRead += 6;
           hasDoneINTV = true;
           break;
      case cINCC:
           if (unknownINCC.has_value())
           {
             std::cerr << "Error: Record TES4 seems to have more than one INCC sub record!\n";
             return false;
           }
           // read INCC
           if (!loadUint32SubRecordFromStream(in_File, cINCC, tempUint32, false))
             return false;
           unknownINCC = tempUint32;
           BytesRead += 6;
           break;
      default:
           std::cerr << "Error: Found unexpected sub record \"" << IntTo4Char(SubRecName)
                     << "\", but only SNAM, MAST, DATA, ONAM, INTV or INCC are allowed here!\n";
           return false;
           break;
    }
  } // while

  // check for required field
  if (!hasDoneINTV)
  {
    std::cerr << "Error: Record TES4 does not have a INTV field!\n";
    return false;
  }

  return in_File.good();
}

bool Tes4HeaderRecord::isMasterFile() const
{
  return (headerFlags & cMasterFlag) != 0;
}

bool Tes4HeaderRecord::isLocalized() const
{
  return (headerFlags & cLocalizedFlag) != 0;
}

} // namespace
