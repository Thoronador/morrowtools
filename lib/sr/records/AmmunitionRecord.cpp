/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2025  Dirk Stolle

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

#include "AmmunitionRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

AmmunitionRecord::AmmunitionRecord()
: BasicRecord(),
  editorID(""),
  unknownOBND(std::array<uint8_t, 12>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })),
  name(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  pickupSoundFormID(0),
  putdownSoundFormID(0),
  description(LocalizedString()),
  keywords(std::vector<uint32_t>()),
  // DATA
  projectileFormID(0),
  DATAflags(0),
  baseDamage(0.0f),
  value(0),
  weight(std::nullopt)
  // end of DATA
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool AmmunitionRecord::equals(const AmmunitionRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownOBND == other.unknownOBND)
      && (name == other.name)
      && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
      && (pickupSoundFormID == other.pickupSoundFormID)
      && (putdownSoundFormID == other.putdownSoundFormID)
      && (description==other.description)
      && (keywords == other.keywords) && (projectileFormID == other.projectileFormID)
      && (DATAflags == other.DATAflags) && (baseDamage == other.baseDamage)
      && (value == other.value) && (weight == other.weight);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t AmmunitionRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of string +1 byte for NUL-termination */
        + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size */
        + 4 /* YNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
        + 4 /* ZNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
        + description.getWriteSize() /* DESC */
        + 4 /* DATA */ + 2 /* 2 bytes for length */ + 16 /* fixed size */;
  if (!modelPath.empty())
  {
    writeSize = writeSize + 4 /* MODL */ + 2 /* 2 bytes for length */
               + modelPath.length() + 1 /* length of string +1 byte for NUL-termination */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */
               + unknownMODT.size();
  }
  if (!keywords.empty())
  {
    writeSize = writeSize + 4 /* KSIZ */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
        + 4 /* KWDA */ + 2 /* 2 bytes for length */ + keywords.size() * 4 /* fixed size per keyword */;
  }
  if (weight.has_value())
  {
    // Weight adds four bytes to DATA sub record.
    writeSize += 4;
  }
  return writeSize;
}

bool AmmunitionRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cAMMO), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length()+1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  // write OBND
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  subLength = 12; // fixed size
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
      std::cerr << "Error while writing sub record MODT of AMMO!\n";
      return false;
    }
  }

  // write Pickup Sound form ID (YNAM)
  output.write(reinterpret_cast<const char*>(&cYNAM), 4);
  subLength = 4; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&pickupSoundFormID), 4);

  // write Putdown Sound form ID (ZNAM)
  output.write(reinterpret_cast<const char*>(&cZNAM), 4);
  subLength = 4; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&putdownSoundFormID), 4);

  // write DESC
  if (!description.saveToStream(output, cDESC))
  {
    std::cerr << "Error while writing sub record DESC of AMMO!\n";
    return false;
  }

  if (!keywords.empty())
  {
    // write KSIZ
    output.write(reinterpret_cast<const char*>(&cKSIZ), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write KSIZ's data
    const uint32_t k_Size = keywords.size();
    output.write(reinterpret_cast<const char*>(&k_Size), 4);

    // write KWDA
    output.write(reinterpret_cast<const char*>(&cKWDA), 4);
    subLength = 4 * k_Size;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    for (const uint32_t keyword: keywords)
    {
      output.write(reinterpret_cast<const char*>(&keyword), 4);
    }
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = weight.has_value() ? 20 : 16;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write DATA's stuff
  output.write(reinterpret_cast<const char*>(&projectileFormID), 4);
  output.write(reinterpret_cast<const char*>(&DATAflags), 4);
  output.write(reinterpret_cast<const char*>(&baseDamage), 4);
  output.write(reinterpret_cast<const char*>(&value), 4);
  if (weight.has_value())
    output.write(reinterpret_cast<const char*>(&weight.value()), 4);

  return output.good();
}
#endif

bool AmmunitionRecord::loadFromStream(std::istream& input, const bool localized, const StringTable& table)
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
    std::cerr << "Error: Sub record OBND of AMMO has invalid length("
              << subLength << " bytes). Should be 12 bytes!\n";
    return false;
  }
  // read OBND's stuff
  input.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record OBND of AMMO!\n";
    return false;
  }

  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  pickupSoundFormID = 0;
  putdownSoundFormID = 0;
  description.reset();
  keywords.clear();
  bool hasReadDATA = false;

  while (bytesRead < readSize)
  {
    // read next sub record name
    input.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: Record AMMO seems to have more than one FULL sub record!\n";
             return false;
           }
           // read FULL
           if (!name.loadFromStream(input, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: Record AMMO seems to have more than one MODL sub record!\n";
             return false;
           }
           // read model path (MODL)
           if (!loadString512FromStream(input, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           if (modelPath.empty())
           {
             std::cerr << "Error: Sub record MODL of AMMO is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: Record AMMO seems to have more than one MODT sub record!\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(input, cMODT, false))
           {
             std::cerr << "Error while reading sub record MODT of AMMO!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODT.size();
           break;
      case cYNAM:
           if (pickupSoundFormID != 0)
           {
             std::cerr << "Error: Record AMMO seems to have more than one YNAM sub record!\n";
             return false;
           }
           // read YNAM
           if (!loadUint32SubRecordFromStream(input, cYNAM, pickupSoundFormID, false))
             return false;
           bytesRead += 6;
           if (pickupSoundFormID == 0)
           {
             std::cerr << "Error: Sub record YNAM of AMMO is zero!\n";
             return false;
           }
           break;
      case cZNAM:
           if (putdownSoundFormID != 0)
           {
             std::cerr << "Error: Record AMMO seems to have more than one ZNAM sub record!\n";
             return false;
           }
           // read ZNAM
           if (!loadUint32SubRecordFromStream(input, cZNAM, putdownSoundFormID, false))
             return false;
           bytesRead += 6;
           if (putdownSoundFormID == 0)
           {
             std::cerr << "Error: Sub record ZNAM of AMMO is zero!\n";
             return false;
           }
           break;
      case cDESC:
           if (description.isPresent())
           {
             std::cerr << "Error: Record AMMO seems to have more than one DESC sub record!\n";
             return false;
           }
           // read DESC
           if (!description.loadFromStream(input, cDESC, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cKSIZ:
           if (!loadKeywords(input, keywords, bytesRead))
             return false;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: Record AMMO seems to have more than one DATA sub record!\n";
             return false;
           }
           // DATA's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if ((subLength != 16) && (subLength != 20))
           {
             std::cerr << "Error: Sub record DATA of AMMO has invalid length ("
                       << subLength << " bytes). Should be 16 or 20 bytes!\n";
             return false;
           }
           // read DATA's stuff
           input.read(reinterpret_cast<char*>(&projectileFormID), 4);
           input.read(reinterpret_cast<char*>(&DATAflags), 4);
           input.read(reinterpret_cast<char*>(&baseDamage), 4);
           input.read(reinterpret_cast<char*>(&value), 4);
           if (subLength >= 20)
           {
             weight = 0.0f;
             input.read(reinterpret_cast<char*>(&weight.value()), 4);
           }
           bytesRead += subLength;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record DATA of AMMO!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      default:
           std::cerr << "Error: Unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only MODL, MODT, YNAM, ZNAM, DESC, KSIZ or DATA are allowed!\n";
           return false;
           break;
    }
  }

  // check
  if (!hasReadDATA || !description.isPresent())
  {
    std::cerr << "Error: At least one required sub record of AMMO is missing!\n";
    return false;
  }

  return input.good();
}

uint32_t AmmunitionRecord::getRecordType() const
{
  return cAMMO;
}

} // namespace
