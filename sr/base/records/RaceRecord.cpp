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

#include "RaceRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

/* RaceData's functions */

RaceRecord::RaceData::RaceData()
: unknown16({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  heightMale(0.0f),
  heightFemale(0.0f),
  weightMale(0.0f),
  weightFemale(0.0f),
  unknown96({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  unknown36(std::nullopt)
{
}

bool RaceRecord::RaceData::operator==(const RaceRecord::RaceData& other) const
{
  return (unknown16 == other.unknown16) && (heightMale == other.heightMale)
      && (heightFemale == other.heightFemale) && (weightMale == other.weightMale)
      && (weightFemale == other.weightFemale) && (unknown96 == other.unknown96)
      && (unknown36 == other.unknown36);
}

void RaceRecord::RaceData::clear()
{
  unknown16.fill(0);
  heightMale = 0.0f;
  heightFemale = 0.0f;
  weightMale = 0.0f;
  weightFemale = 0.0f;
  unknown96.fill(0);
  unknown36.reset();
}

/* RaceRecord's functions */

RaceRecord::RaceRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  description(LocalizedString()),
  spellFormIDs(std::vector<uint32_t>()),
  unknownWNAM(std::nullopt),
  unknownBODT(BinarySubRecord()),
  unknownBOD2(BinarySubRecord()),
  keywords(std::vector<uint32_t>()),
  data(RaceData()),
  subBlocks(std::vector<SubBlock>())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool RaceRecord::equals(const RaceRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (name == other.name) && (description == other.description)
      && (spellFormIDs == other.spellFormIDs) && (unknownWNAM == other.unknownWNAM)
      && (unknownBODT == other.unknownBODT) && (unknownBOD2 == other.unknownBOD2)
      && (keywords == other.keywords) && (data == other.data)
      && (subBlocks == other.subBlocks);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t RaceRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;

  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + name.getWriteSize() /* FULL */
        + description.getWriteSize() /* DESC */
        + 4 /* DATA */ + 2 /* 2 bytes for length */ + getDataLength() /* fixed length of 128 or 164 bytes */;
  if (!spellFormIDs.empty())
  {
    writeSize += 4 /*SPCT*/ + 2 /* 2 bytes for length */ + 4 /* size */
               + spellFormIDs.size() * (4 /*SPLO*/ + 2 /* 2 bytes for length */ + 4 /* size */);
  }
  if (unknownWNAM.has_value())
  {
    writeSize += 4 /* WNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length of 4 bytes */;
  }
  if (unknownBODT.isPresent())
  {
    writeSize += 4 /* BODT */ + 2 /* 2 bytes for length */ + unknownBODT.size() /* length */;
  }
  if (unknownBOD2.isPresent())
  {
    writeSize += 4 /* BOD2 */ + 2 /* 2 bytes for length */ + unknownBOD2.size() /* length */;
  }
  if (!keywords.empty())
  {
    writeSize += 4 /*KSIZ*/ + 2 /* 2 bytes for length */ + 4 /* fixed length of four bytes */
               + 4 /*KWDA*/ + 2 /* 2 bytes for length */ + 4 * keywords.size() /* size */;
  }
  for (const auto& block: subBlocks)
  {
    writeSize += 4 /*header*/ + 2 /* 2 bytes for length */
               + block.subData.size() /* length */;
  }
  return writeSize;
}

bool RaceRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cRACE), 4);
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

  // write description (DESC)
  if (!description.saveToStream(output, cDESC))
    return false;

  if (!spellFormIDs.empty())
  {
    // write spell count (SPCT)
    output.write(reinterpret_cast<const char*>(&cSPCT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    const uint32_t spell_count = spellFormIDs.size();
    output.write(reinterpret_cast<const char*>(&spell_count), 4);

    for (const auto spellId: spellFormIDs)
    {
      // write SPLO
      output.write(reinterpret_cast<const char*>(&cSPLO), 4);
      subLength = 4;
      output.write(reinterpret_cast<const char*>(&subLength), 2);
      output.write(reinterpret_cast<const char*>(&spellId), 4);
    }
  }

  if (unknownWNAM.has_value())
  {
    // write WNAM
    output.write(reinterpret_cast<const char*>(&cWNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownWNAM.value()), 4);
  }

  if (unknownBODT.isPresent())
  {
    // write BODT
    if (!unknownBODT.saveToStream(output, cBODT))
    {
      std::cerr << "Error while writing sub record BODT of RACE!\n";
      return false;
    }
  }

  if (unknownBOD2.isPresent())
  {
    // write BOD2
    if (!unknownBOD2.saveToStream(output, cBOD2))
    {
      std::cerr << "Error while writing sub record BOD2 of RACE!\n";
      return false;
    }
  }

  if (!keywords.empty())
  {
    // write number / size of keywords (KSIZ)
    output.write(reinterpret_cast<const char*>(&cKSIZ), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    const uint32_t k_Size = keywords.size();
    output.write(reinterpret_cast<const char*>(&k_Size), 4);

    // write keywords (KWDA)
    output.write(reinterpret_cast<const char*>(&cKWDA), 4);
    subLength = 4 * k_Size;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    for (const std::uint32_t keyword: keywords)
    {
      output.write(reinterpret_cast<const char*>(&keyword), 4);
    }
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = getDataLength();
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  //write DATA's stuff
  output.write(reinterpret_cast<const char*>(data.unknown16.data()), 16);
  output.write(reinterpret_cast<const char*>(&data.heightMale), 4);
  output.write(reinterpret_cast<const char*>(&data.heightFemale), 4);
  output.write(reinterpret_cast<const char*>(&data.weightMale), 4);
  output.write(reinterpret_cast<const char*>(&data.weightFemale), 4);
  output.write(reinterpret_cast<const char*>(data.unknown96.data()), 96);
  if (data.unknown36.has_value() && (getDataLength() > 128))
  {
    output.write(reinterpret_cast<const char*>(data.unknown36.value().data()), 36);
  }

  for (const auto& block: subBlocks)
  {
    if (!block.subData.saveToStream(output, block.subType))
    {
      std::cerr << "Error while writing (binary) subrecord "
                << IntTo4Char(block.subType) << " of RACE!\n";
      return false;
    }
  }

  return output.good();
}
#endif

bool RaceRecord::loadFromStream(std::istream& input, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(input, readSize))
    return false;
  if (isDeleted())
    return true;

  uint32_t bytesRead = 0;

  // read editor ID (EDID)
  char buffer[512];
  if (!loadString512FromStream(input, editorID, buffer, cEDID, true, bytesRead))
  {
    std::cerr << "Error while reading sub record EDID of RACE!\n";
    return false;
  }

  uint32_t subRecName = 0;
  uint16_t subLength = 0;

  name.reset();
  description.reset();
  spellFormIDs.clear();
  uint32_t the_count, temp_uint32;
  unknownWNAM.reset();
  unknownBODT.setPresence(false);
  unknownBOD2.setPresence(false);
  keywords.clear();
  bool hasReadDATA = false;
  subBlocks.clear();
  while ((bytesRead < readSize) && (!((unknownBODT.isPresent() || unknownBOD2.isPresent()) && hasReadDATA && description.isPresent())))
  {
    // read next sub record name
    input.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: RACE seems to have more than one FULL sub record!\n";
             return false;
           }
           if (!name.loadFromStream(input, cFULL, false, bytesRead, localized, table, buffer))
           {
             std::cerr << "Error while reading sub record FULL of RACE!\n";
             return false;
           }
           break;
      case cDESC:
           if (description.isPresent())
           {
             std::cerr << "Error: RACE seems to have more than one DESC sub record!\n";
             return false;
           }
           if (!description.loadFromStream(input, cDESC, false, bytesRead, localized, table, buffer))
           {
             std::cerr << "Error while reading sub record DESC of RACE!\n";
             return false;
           }
           break;
      case cSPCT:
           if (!spellFormIDs.empty())
           {
             std::cerr << "Error: RACE seems to have more than one SPCT sub record!\n";
             return false;
           }
           // SPCT's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 4)
           {
             std::cerr << "Error: Sub record SPCT of RACE has invalid length ("
                       << subLength << " bytes). Should be four bytes!\n";
             return false;
           }
           // read SPCT
           the_count = 0;
           input.read(reinterpret_cast<char*>(&the_count), 4);
           bytesRead += 4;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record SPCT of RACE!\n";
             return false;
           }
           if (the_count == 0)
           {
             std::cerr << "Error: Sub record SPCT of RACE is zero!\n";
             return false;
           }

           for (uint32_t i = 0; i < the_count; ++i)
           {
             // read SPLO
             input.read(reinterpret_cast<char*>(&subRecName), 4);
             bytesRead += 4;
             if (subRecName != cSPLO)
             {
               UnexpectedRecord(cSPLO, subRecName);
               return false;
             }
             // SPLO's length
             input.read(reinterpret_cast<char*>(&subLength), 2);
             bytesRead += 2;
             if (subLength != 4)
             {
               std::cerr << "Error: sub record SPLO of RACE has invalid length ("
                         << subLength << " bytes). Should be four bytes!\n";
               return false;
             }
             // read SPLO's content
             input.read(reinterpret_cast<char*>(&temp_uint32), 4);
             bytesRead += 4;
             if (!input.good())
             {
               std::cerr << "Error while reading sub record SPLO of RACE!\n";
               return false;
             }
             spellFormIDs.push_back(temp_uint32);
           }
           break;
      case cWNAM:
           if (unknownWNAM.has_value())
           {
             std::cerr << "Error: RACE seems to have more than one WNAM sub record!\n";
             return false;
           }
           unknownWNAM = 0;
           if (!loadUint32SubRecordFromStream(input, cWNAM, unknownWNAM.value(), false))
           {
             std::cerr << "Error while reading subrecord WNAM of RACE!\n";
             return false;
           }
           bytesRead += 6;
           break;
      case cBODT:
           if (unknownBODT.isPresent())
           {
             std::cerr << "Error: RACE seems to have more than one BODT sub record!\n";
             return false;
           }
           // read BODT
           if (!unknownBODT.loadFromStream(input, cBODT, false))
             return false;
           bytesRead += 2 + unknownBODT.size();
           // check length
           if (unknownBODT.size() != 12)
           {
             std::cerr << "Error: Sub record BODT of RACE has invalid length ("
                       << unknownBODT.size() << " bytes). Should be 12 bytes!\n";
             return false;
           }
           break;
      case cBOD2:
           if (unknownBOD2.isPresent())
           {
             std::cerr << "Error: RACE seems to have more than one BOD2 sub record!\n";
             return false;
           }
           // read BOD2
           if (!unknownBOD2.loadFromStream(input, cBOD2, false))
             return false;
           bytesRead += 2 + unknownBOD2.size();
           break;
      case cKSIZ:
           if (!loadKeywords(input, keywords, bytesRead))
           {
             return false;
           }
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: RACE seems to have more than one DATA sub record!\n";
             return false;
           }
           // DATA's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != getDataLength())
           {
             std::cerr << "Error: sub record DATA of RACE has invalid length ("
                       << subLength << " bytes). Should be " << getDataLength()
                       << " bytes!\n";
             return false;
           }
           //read DATA
           input.read(reinterpret_cast<char*>(data.unknown16.data()), 16);
           input.read(reinterpret_cast<char*>(&data.heightMale), 4);
           input.read(reinterpret_cast<char*>(&data.heightFemale), 4);
           input.read(reinterpret_cast<char*>(&data.weightMale), 4);
           input.read(reinterpret_cast<char*>(&data.weightFemale), 4);
           input.read(reinterpret_cast<char*>(data.unknown96.data()), 96);
           if (getDataLength() > 128)
           {
             data.unknown36 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0 };
             input.read(reinterpret_cast<char*>(data.unknown36.value().data()), 36);
           }
           else
           {
             data.unknown36.reset();
           }
           bytesRead += getDataLength();
           if (!input.good())
           {
             std::cerr << "Error while reading sub record DATA of RACE!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      default:
           std::cerr << "Error: Found unexpected sub record \"" << IntTo4Char(subRecName)
                     << "\", but only FULL, DESC, SPCT, WNAM, BODT, BOD2, KSIZ or DATA are allowed here!\n";
           return false;
    }
  }

  // presence checks
  if (!((unknownBODT.isPresent() || unknownBOD2.isPresent()) && hasReadDATA && description.isPresent()))
  {
    std::cerr << "Error: Sub record BODT/BOD2, DATA or DESC of RACE is missing!\n";
    return false;
  }

  // read all other sub blocks
  // now read the rest
  subBlocks.clear();
  SubBlock tempBlock;
  while (bytesRead < readSize)
  {
    // read next sub record's name
    input.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    tempBlock.subType = subRecName;
    if (!tempBlock.subData.loadFromStream(input, subRecName, false))
    {
      std::cerr << "Error while reading sub record " << IntTo4Char(subRecName)
                << " of RACE!\n";
      std::cerr << "Read size: " << readSize << ", bytes read: " << bytesRead << "\n";
      return false;
    }
    bytesRead = bytesRead + 2 + tempBlock.subData.size();
    subBlocks.push_back(tempBlock);
  }

  return input.good();
}

uint32_t RaceRecord::getRecordType() const
{
  return cRACE;
}

uint32_t RaceRecord::getDataLength() const
{
  if (headerVersion < 43)
    return 128;
  else
    return 164;
}

} // namespace
