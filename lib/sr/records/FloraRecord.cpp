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

#include "FloraRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

FloraRecord::FloraRecord()
: BasicRecord(),
  editorID(""),
  unknownVMAD(BinarySubRecord()),
  unknownOBND(std::array<uint8_t, 12>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })),
  name(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownMODS(BinarySubRecord()),
  unknownPNAM(0),
  activateTextOverride(LocalizedString()),
  unknownFNAM(0),
  ingredientFormID(0),
  harvestSoundFormID(0),
  unknownPFPC(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool FloraRecord::equals(const FloraRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownVMAD == other.unknownVMAD)
      && (unknownOBND == other.unknownOBND)
      && (name == other.name) && (modelPath == other.modelPath)
      && (unknownMODT == other.unknownMODT) && (unknownMODS == other.unknownMODS)
      && (unknownPNAM == other.unknownPNAM)
      && (activateTextOverride == other.activateTextOverride)
      && (unknownFNAM == other.unknownFNAM)
      && (ingredientFormID == other.ingredientFormID)
      && (harvestSoundFormID == other.harvestSoundFormID)
      && (unknownPFPC == other.unknownPFPC);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t FloraRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size */
        + name.getWriteSize() /* FULL */
        + 4 /* PNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
        + 4 /* FNAM */ + 2 /* 2 bytes for length */ + 2 /* fixed size */
        + 4 /* PFPC */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /* VMAD */ + 2 /* 2 bytes for length */ + unknownVMAD.size() /* size */;
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
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize + 4 /* MODS */ + 2 /* 2 bytes for length */ + unknownMODS.size() /* size */;
  }
  if (activateTextOverride.isPresent())
  {
    writeSize += activateTextOverride.getWriteSize();
  }
  if (ingredientFormID != 0)
  {
    writeSize = writeSize + 4 /* PFIG */ + 2 /* 2 bytes for length */ + 4 /* size */;
  }
  if (harvestSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* SNAM */ + 2 /* 2 bytes for length */ + 4 /* size */;
  }
  return writeSize;
}

bool FloraRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cFLOR), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  //write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  if (unknownVMAD.isPresent())
  {
    // write VMAD
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cerr << "Error while writing sub record VMAD of FLOR!\n";
      return false;
    }
  }

  // write object bounds (OBND)
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  subLength = 12; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  // write FULL
  if (!name.saveToStream(output, cFULL))
    return false;

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
    // write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing sub record MODT of FLOR!\n";
      return false;
    }
  }

  if (unknownMODS.isPresent())
  {
    // write MODS
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cerr << "Error while writing sub record MODS of FLOR!\n";
      return false;
    }
  }

  // write PNAM
  output.write(reinterpret_cast<const char*>(&cPNAM), 4);
  subLength = 4; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownPNAM), 4);

  if (activateTextOverride.isPresent())
  {
    // write RNAM
    if (!activateTextOverride.saveToStream(output, cRNAM))
      return false;
  }

  // write FNAM
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  subLength = 2; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownFNAM), 2);

  if (ingredientFormID != 0)
  {
    // write Ingredient form ID (PFIG)
    output.write(reinterpret_cast<const char*>(&cPFIG), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&ingredientFormID), 4);
  }

  if (harvestSoundFormID != 0)
  {
    // write Harvest Sound's form ID (SNAM)
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&harvestSoundFormID), 4);
  }

  // write PFPC
  output.write(reinterpret_cast<const char*>(&cPFPC), 4);
  subLength = 4; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownPFPC), 4);

  return output.good();
}
#endif

bool FloraRecord::loadFromStream(std::istream& input, const bool localized, const StringTable& table)
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

  unknownVMAD.setPresence(false);
  bool hasReadOBND = false;
  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  bool hasReadPNAM = false;
  unknownPNAM = 0;
  activateTextOverride.reset();
  bool hasReadFNAM = false;
  unknownFNAM = 0;
  ingredientFormID = 0;
  harvestSoundFormID = 0;
  bool hasReadPFPC = false;
  unknownPFPC = 0;
  while (bytesRead < readSize)
  {
    // read next header
    input.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cerr << "Error: FLOR seems to have more than one VMAD sub record.\n";
             return false;
           }
           // read VMAD
           if (!unknownVMAD.loadFromStream(input, cVMAD, false))
           {
             std::cerr << "Error while reading sub record VMAD of FLOR!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownVMAD.size();
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cerr << "Error: FLOR seems to have more than one OBND sub record.\n";
             return false;
           }
           // OBND's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: Sub record OBND of FLOR has invalid length ("
                       << subLength << " bytes). Should be 12 bytes.\n";
             return false;
           }
           // read OBND
           input.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
           bytesRead += 12;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record OBND of FLOR!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: FLOR seems to have more than one FULL sub record.\n";
             return false;
           }
           // read FULL
           if (!name.loadFromStream(input, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: FLOR seems to have more than one MODL sub record.\n";
             return false;
           }
           // read model path
           if (!loadString512FromStream(input, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           if (modelPath.empty())
           {
             std::cerr << "Error: Sub record MODL of FLOR is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: FLOR seems to have more than one MODT sub record.\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(input, cMODT, false))
           {
             std::cerr << "Error while reading sub record MODT of FLOR!\n";
             return false;
           }
           bytesRead += (2 + unknownMODT.size());
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cerr << "Error: FLOR seems to have more than one MODS sub record.\n";
             return false;
           }
           // read MODS
           if (!unknownMODS.loadFromStream(input, cMODS, false))
           {
             std::cerr << "Error while reading sub record MODS of FLOR!\n";
             return false;
           }
           bytesRead += (2 + unknownMODS.size());
           break;
      case cPNAM:
           if (hasReadPNAM)
           {
             std::cerr << "Error: FLOR seems to have more than one PNAM sub record.\n";
             return false;
           }
           // read PNAM
           if (!loadUint32SubRecordFromStream(input, cPNAM, unknownPNAM, false))
             return false;
           bytesRead += 6;
           hasReadPNAM = true;
           break;
      case cRNAM:
           if (activateTextOverride.isPresent())
           {
             std::cerr << "Error: FLOR seems to have more than one RNAM sub record.\n";
             return false;
           }
           // read RNAM
           if (!activateTextOverride.loadFromStream(input, cRNAM, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cerr << "Error: FLOR seems to have more than one FNAM sub record.\n";
             return false;
           }
           // FNAM's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 2)
           {
             std::cerr << "Error: Sub record FNAM of FLOR has invalid length ("
                       << subLength << " bytes). Should be two bytes.\n";
             return false;
           }
           // read FNAM
           input.read(reinterpret_cast<char*>(&unknownFNAM), 2);
           bytesRead += 2;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record FNAM of FLOR!\n";
             return false;
           }
           hasReadFNAM = true;
           break;
      case cPFIG:
           if (ingredientFormID != 0)
           {
             std::cerr << "Error: FLOR seems to have more than one PFIG sub record.\n";
             return false;
           }
           // read PFIG
           if (!loadUint32SubRecordFromStream(input, cPFIG, ingredientFormID, false))
             return false;
           bytesRead += 6;
           // check content
           if (ingredientFormID == 0)
           {
             std::cerr << "Error: Sub record PFIG of FLOR is zero!\n";
             return false;
           }
           break;
      case cSNAM:
           if (harvestSoundFormID != 0)
           {
             std::cerr << "Error: FLOR seems to have more than one SNAM sub record.\n";
             return false;
           }
           // read SNAM
           if (!loadUint32SubRecordFromStream(input, cSNAM, harvestSoundFormID, false))
             return false;
           bytesRead += 6;
           // check content
           if (harvestSoundFormID == 0)
           {
             std::cerr << "Error: Sub record SNAM of FLOR is zero!\n";
             return false;
           }
           break;
      case cPFPC:
           if (hasReadPFPC)
           {
             std::cerr << "Error: FLOR seems to have more than one PFPC sub record.\n";
             return false;
           }
           // read PFPC
           if (!loadUint32SubRecordFromStream(input, cPFPC, unknownPFPC, false))
             return false;
           bytesRead += 6;
           hasReadPFPC = true;
           break;
      default:
           std::cerr << "Error: Found unexpected sub record \"" << IntTo4Char(subRecName)
                     << "\", but only VMAD, OBND, MODL, MODT, MODS, PNAM, RNAM,"
                     << " FNAM, PFIG, SNAM or PFPC are allowed here!\n";
           return false;
    }
  }

  // presence checks
  if (!hasReadOBND || !name.isPresent() || !hasReadPNAM || !hasReadFNAM
      || !hasReadPFPC)
  {
    std::cerr << "Error: At least one required sub record of FLOR is missing!\n";
    return false;
  }

  return input.good();
}

uint32_t FloraRecord::getRecordType() const
{
  return cFLOR;
}

} // namespace
