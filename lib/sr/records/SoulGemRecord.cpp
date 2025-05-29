/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021, 2025  Dirk Stolle

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

#include "SoulGemRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

/* constants for capacity / filled stuff */
const uint8_t SoulGemRecord::cCapacityEmpty = 0;
const uint8_t SoulGemRecord::cCapacityPetty = 1;
const uint8_t SoulGemRecord::cCapacityLesser = 2;
const uint8_t SoulGemRecord::cCapacityCommon = 3;
const uint8_t SoulGemRecord::cCapacityGreater = 4;
const uint8_t SoulGemRecord::cCapacityAzura = 5;

SoulGemRecord::SoulGemRecord()
: BasicRecord(),
  editorID(""),
  unknownOBND(std::array<uint8_t, 12>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })),
  name(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  keywords(std::vector<uint32_t>()),
  // sub record DATA
  value(0),
  weight(0.0f),
  // end of DATA
  soulInside(0), // sub record SOUL
  capacity(0),   // sub record SLCP
  linkedToFormID(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool SoulGemRecord::equals(const SoulGemRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownOBND == other.unknownOBND)
      && (name == other.name)
      && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
      && (keywords == other.keywords) && (value == other.value)
      && (weight == other.weight) && (soulInside == other.soulInside)
      && (capacity == other.capacity)
      && (linkedToFormID == other.linkedToFormID);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t SoulGemRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
      + editorID.length() + 1 /* length of name +1 byte for NUL termination */
      + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size */
      + 4 /* DATA */ + 2 /* 2 bytes for length */ + 8 /* fixed size */
      + 4 /* SOUL */ + 2 /* 2 bytes for length */ + 1 /* fixed size */
      + 4 /* SLCP */ + 2 /* 2 bytes for length */ + 1 /* fixed size */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize + 4 /* MODL */ + 2 /* 2 bytes for length */
      + modelPath.length() + 1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */ + unknownMODT.size() /* size */;
  }
  if (!keywords.empty())
  {
    writeSize = writeSize + 4 /* KSIZ */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
               + 4 /* KWDA */ + 2 /* 2 bytes for length */ + 4 * keywords.size() /* 4 bytes per element */;
  }
  if (linkedToFormID != 0)
  {
    writeSize = writeSize + 4 /* NAM0 */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  return writeSize;
}

bool SoulGemRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cSLGM), 4);
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
  subLength = 12; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  if (name.isPresent())
  {
    // write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  if (!modelPath.empty())
  {
    // write model path (MODL)
    output.write(reinterpret_cast<const char*>(&cMODL), 4);
    subLength = modelPath.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(modelPath.c_str(), subLength);
  }

  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing sub record MODT of SLGM!\n";
      return false;
    }
  }

  if (!keywords.empty())
  {
    // write KSIZ
    output.write(reinterpret_cast<const char*>(&cKSIZ), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write keyword count
    const uint32_t k_Size = keywords.size();
    output.write(reinterpret_cast<const char*>(&k_Size), 4);

    // write keyword array (KWDA)
    output.write(reinterpret_cast<const char*>(&cKWDA), 4);
    subLength = 4 * k_Size; // four bytes per element
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write keywords
    for (const uint32_t keyword: keywords)
    {
      output.write(reinterpret_cast<const char*>(&keyword), 4);
    }
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = 8; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write DATA's stuff
  output.write(reinterpret_cast<const char*>(&value), 4);
  output.write(reinterpret_cast<const char*>(&weight), 4);

  // write power of the trapped soul (SOUL)
  output.write(reinterpret_cast<const char*>(&cSOUL), 4);
  subLength = 1; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&soulInside), 1);

  // write soul gem capacity (SLCP)
  output.write(reinterpret_cast<const char*>(&cSLCP), 4);
  subLength = 1; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&capacity), 1);

  if (linkedToFormID != 0)
  {
    // write "Linked to" form ID (NAM0)
    output.write(reinterpret_cast<const char*>(&cNAM0), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&linkedToFormID), 4);
  }

  return output.good();
}
#endif

bool SoulGemRecord::loadFromStream(std::istream& input, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(input, readSize))
    return false;
  if (isDeleted())
    return true;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  // read editor ID (EDID)
  char buffer[512];
  if (!loadString512FromStream(input, editorID, buffer, cEDID, true, bytesRead))
    return false;

  // read OBND
  input.read(reinterpret_cast<char*>(&subRecName), 4);
  bytesRead += 4;
  if (subRecName != cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  // OBND's length
  input.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength != 12)
  {
    std::cerr << "Error: Sub record OBND of SLGM has invalid length ("
              << subLength << " bytes). Should be 12 bytes.\n";
    return false;
  }
  // read OBND's stuff
  input.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record OBND of SLGM!\n";
    return false;
  }

  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  keywords.clear();
  bool hasReadDATA = false;
  bool hasReadSOUL = false;
  bool hasReadSLCP = false;
  linkedToFormID = 0;
  while (bytesRead < readSize)
  {
    // read next sub record header
    input.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: SLGM seems to have more than one FULL sub record.\n";
             return false;
           }
           if (!name.loadFromStream(input, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: SLGM seems to have more than one MODL sub record.\n";
             return false;
           }
           // read model path (MODL)
           if (!loadString512FromStream(input, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: SLGM seems to have more than one MODT sub record.\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(input, cMODT, false))
           {
             std::cerr << "Error while reading sub record MODT of SLGM!\n";
             return false;
           }
           bytesRead = bytesRead + 2 /* length */ + unknownMODT.size() /* data size */;
           break;
      case cKSIZ:
           if (!loadKeywords(input, keywords, bytesRead))
             return false;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: SLGM seems to have more than one DATA sub record.\n";
             return false;
           }
           // DATA's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: Sub record DATA of SLGM has invalid length ("
                       << subLength << " bytes). Should be eight bytes.\n";
             return false;
           }
           // read DATA
           input.read(reinterpret_cast<char*>(&value), 4);
           input.read(reinterpret_cast<char*>(&weight), 4);
           bytesRead += 8;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record DATA of SLGM!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cSOUL:
           if (hasReadSOUL)
           {
             std::cerr << "Error: SLGM seems to have more than one SOUL sub record.\n";
             return false;
           }
           // SOUL's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 1)
           {
             std::cerr << "Error: Sub record SOUL of SLGM has invalid length ("
                       << subLength << " bytes). Should be one byte.\n";
             return false;
           }
           // read SOUL
           input.read(reinterpret_cast<char*>(&soulInside), 1);
           bytesRead += 1;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record SOUL of SLGM!\n";
             return false;
           }
           hasReadSOUL = true;
           break;
      case cSLCP:
           if (hasReadSLCP)
           {
             std::cerr << "Error: SLGM seems to have more than one SLCP sub record.\n";
             return false;
           }
           // SLCP's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 1)
           {
             std::cerr << "Error: Sub record SLCP of SLGM has invalid length ("
                       << subLength << " bytes). Should be one byte.\n";
             return false;
           }
           // read SLCP
           input.read(reinterpret_cast<char*>(&capacity), 1);
           bytesRead += 1;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record SLCP of SLGM!\n";
             return false;
           }
           hasReadSLCP = true;
           break;
      case cNAM0:
           if (linkedToFormID != 0)
           {
             std::cerr << "Error: SLGM seems to have more than one NAM0 sub record.\n";
             return false;
           }
           // read NAM0
           if (!loadUint32SubRecordFromStream(input, cNAM0, linkedToFormID, false))
             return false;
           bytesRead += 6;
           // check content
           if (linkedToFormID == 0)
           {
             std::cerr << "Error: Sub record NAM0 of SLGM is zero!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: Unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only FULL, MODL, MODT, KSIZ, DATA, "
                     << "SOUL, SLCP or NAM0 are allowed here!\n";
           return false;
           break;
    }
  }

  // presence checks
  if (modelPath.empty() || !hasReadDATA || !hasReadSOUL || !hasReadSLCP)
  {
    std::cerr << "Error while reading SLGM record: At least one required sub record is missing!\n";
    return false;
  }

  return input.good();
}

uint32_t SoulGemRecord::getRecordType() const
{
  return cSLGM;
}

} // namespace
