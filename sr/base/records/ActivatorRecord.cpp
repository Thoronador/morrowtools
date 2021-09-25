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

#include "ActivatorRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

ActivatorRecord::destStruct::destStruct()
: hasDSTD(false),
  unknownDSTD({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  destroyedModelPath(""),
  unknownDMDT(BinarySubRecord()),
  unknownDMDS(BinarySubRecord())
{
}

bool ActivatorRecord::destStruct::operator==(const ActivatorRecord::destStruct& other) const
{
  return (hasDSTD == other.hasDSTD) && ((unknownDSTD == other.unknownDSTD) || !hasDSTD)
      && (destroyedModelPath == other.destroyedModelPath) && (unknownDMDT == other.unknownDMDT)
      && (unknownDMDS == other.unknownDMDS);
}

void ActivatorRecord::destStruct::reset()
{
  hasDSTD = false;
  unknownDSTD.fill(0);
  destroyedModelPath = "";
  unknownDMDT.setPresence(false);
  unknownDMDS.setPresence(false);
}

ActivatorRecord::Colour::Colour()
: red(0),
  green(0),
  blue(0)
{
}

bool ActivatorRecord::Colour::operator==(const ActivatorRecord::Colour& other) const
{
  return (red == other.red) && (green == other.green) && (blue == other.blue);
}

/* ActivatorRecord's functions */

ActivatorRecord::ActivatorRecord()
: BasicRecord(),
  editorID(""),
  unknownVMAD(BinarySubRecord()),
  unknownOBND({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  name(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownMODS(BinarySubRecord()),
  unknownDEST(std::nullopt),
  destructionStructures(std::vector<destStruct>()),
  keywords(std::vector<uint32_t>()),
  defaultPrimitiveColour({}),
  loopingSoundFormID(0),
  activateSoundFormID(0),
  waterTypeFormID(0),
  activateTextOverride(LocalizedString()),
  unknownFNAM(std::nullopt),
  interactionKeywordFormID(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool ActivatorRecord::equals(const ActivatorRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
    && (unknownVMAD == other.unknownVMAD)
    && (unknownOBND == other.unknownOBND)
    && (name == other.name)
    && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
    && (unknownMODS == other.unknownMODS)
    && (unknownDEST == other.unknownDEST)
    && (destructionStructures == other.destructionStructures)
    && (keywords == other.keywords)
    && (defaultPrimitiveColour == other.defaultPrimitiveColour)
    && (unknownFNAM == other.unknownFNAM)
    && (loopingSoundFormID == other.loopingSoundFormID)
    && (activateSoundFormID == other.activateSoundFormID)
    && (waterTypeFormID == other.waterTypeFormID)
    && (activateTextOverride == other.activateTextOverride)
    && (interactionKeywordFormID == other.interactionKeywordFormID);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ActivatorRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize;
  writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
      + editorID.length() + 1 /* length of string +1 byte for NUL-termination */
      + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /* VMAD */ + 2 /* 2 bytes for length */ + unknownVMAD.size();
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize + 4 /* MODL */ + 2 /* 2 bytes for length */
        + modelPath.length() + 1 /* length of string +1 byte for NUL-termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */ + unknownMODT.size();
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize + 4 /* MODS */ + 2 /* 2 bytes for length */ + unknownMODS.size();
  }
  if (unknownDEST.has_value())
  {
    writeSize = writeSize + 4 /* DEST */ + 2 /* 2 bytes for length */ + 8 /* fixed size */;
  }
  for (const auto& structure: destructionStructures)
  {
    if (structure.hasDSTD)
    {
      writeSize = writeSize + 4 /* DSTD */ + 2 /* 2 bytes for length */ + 20 /* fixed size */;
    }
    if (!structure.destroyedModelPath.empty())
    {
      writeSize = writeSize + 4 /* DMDL */ + 2 /* 2 bytes for length */
          + structure.destroyedModelPath.length() + 1 /* length of string +1 byte for NUL-termination */;
    }
    if (structure.unknownDMDT.isPresent())
    {
      writeSize = writeSize + 4 /* DMDT */ + 2 /* 2 bytes for length */ + structure.unknownDMDT.size();
    }
    if (structure.unknownDMDS.isPresent())
    {
      writeSize = writeSize + 4 /* DMDS */ + 2 /* 2 bytes for length */ + structure.unknownDMDS.size();
    }
    writeSize = writeSize + 4 /* DSTF */ + 2 /* 2 bytes for length */ + 0 /* zero size */;
  }
  if (!keywords.empty())
  {
    writeSize = writeSize + 4 /* KSIZ */ + 2 /* 2 bytes for length */ + 4 /* fixed length */
        + 4 /* KWDA */ + 2 /* 2 bytes for length */ + 4 * keywords.size();
  }
  if (defaultPrimitiveColour.has_value())
  {
    writeSize = writeSize +4 /* PNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (loopingSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* SNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (activateSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* VNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (waterTypeFormID != 0)
  {
    writeSize = writeSize + 4 /* WNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (activateTextOverride.isPresent())
  {
    writeSize += activateTextOverride.getWriteSize();
  }
  if (unknownFNAM.has_value())
  {
    writeSize = writeSize + 4 /* FNAM */ + 2 /* 2 bytes for length */ + 2 /* fixed size */;
  }
  if (interactionKeywordFormID != 0)
  {
    writeSize = writeSize + 4 /* KNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  return writeSize;
}

bool ActivatorRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cACTI), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  // write VMAD
  if (unknownVMAD.isPresent())
  {
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cerr << "Error while writing subrecord VMAD of ACTI!\n";
      return false;
    }
  }

  // write OBND
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  subLength = 12; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  if (name.isPresent())
  {
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
      std::cerr << "Error while writing subrecord MODT of ACTI!\n";
      return false;
    }
  }

  if (unknownMODS.isPresent())
  {
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cerr << "Error while writing subrecord MODS of ACTI!\n";
      return false;
    }
  }

  if (unknownDEST.has_value())
  {
    // write DEST
    output.write(reinterpret_cast<const char*>(&cDEST), 4);
    subLength = 8; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownDEST.value()), 8);
  }

  for (const auto& structure: destructionStructures)
  {
    if (structure.hasDSTD)
    {
      // write DSTD
      output.write(reinterpret_cast<const char*>(&cDSTD), 4);
      subLength = 20; // fixed length
      output.write(reinterpret_cast<const char*>(&subLength), 2);
      // write DSTD's data
      output.write(reinterpret_cast<const char*>(structure.unknownDSTD.data()), 20);
    }

    if (!structure.destroyedModelPath.empty())
    {
      // write destroyed model path (DMDL)
      output.write(reinterpret_cast<const char*>(&cDMDL), 4);
      subLength = structure.destroyedModelPath.length() + 1;
      output.write(reinterpret_cast<const char*>(&subLength), 2);
      output.write(structure.destroyedModelPath.c_str(), subLength);
    }

    if (structure.unknownDMDT.isPresent())
    {
      if (!structure.unknownDMDT.saveToStream(output, cDMDT))
      {
        std::cerr << "Error while writing subrecord DMDT of ACTI!\n";
        return false;
      }
    }

    if (structure.unknownDMDS.isPresent())
    {
      if (!structure.unknownDMDS.saveToStream(output, cDMDS))
      {
        std::cerr << "Error while writing subrecord DMDS of ACTI!\n";
        return false;
      }
    }

    // write DSTF
    output.write(reinterpret_cast<const char*>(&cDSTF), 4);
    // DSTF's length
    subLength = 0; //fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
  }

  if (!keywords.empty())
  {
    // write keyword size (KSIZ)
    output.write(reinterpret_cast<const char*>(&cKSIZ), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    const uint32_t k_Size = keywords.size();
    output.write(reinterpret_cast<const char*>(&k_Size), 4);

    // write keyword array (KWDA)
    output.write(reinterpret_cast<const char*>(&cKWDA), 4);
    subLength = 4 * keywords.size();
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    for (const uint32_t keyword: keywords)
    {
      output.write(reinterpret_cast<const char*>(&keyword), 4);
    }
  }

  if (defaultPrimitiveColour.has_value())
  {
    // write PNAM
    output.write(reinterpret_cast<const char*>(&cPNAM), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&defaultPrimitiveColour.value().red), 1);
    output.write(reinterpret_cast<const char*>(&defaultPrimitiveColour.value().green), 1);
    output.write(reinterpret_cast<const char*>(&defaultPrimitiveColour.value().blue), 1);
    const uint8_t zero = 0;
    output.write(reinterpret_cast<const char*>(&zero), 1);
  }

  if (loopingSoundFormID != 0)
  {
    // write looping sound form ID (SNAM)
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&loopingSoundFormID), 4);
  }

  if (activateSoundFormID != 0)
  {
    // write activate sound form ID (VNAM)
    output.write(reinterpret_cast<const char*>(&cVNAM), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&activateSoundFormID), 4);
  }

  if (waterTypeFormID != 0)
  {
    // write water type's form ID (WNAM)
    output.write(reinterpret_cast<const char*>(&cWNAM), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&waterTypeFormID), 4);
  }

  if (activateTextOverride.isPresent())
  {
    // write RNAM
    if (!activateTextOverride.saveToStream(output, cRNAM))
      return false;
  }

  if (unknownFNAM.has_value())
  {
    // write FNAM
    output.write(reinterpret_cast<const char*>(&cFNAM), 4);
    subLength = 2; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownFNAM.value()), 2);
  }

  if (interactionKeywordFormID != 0)
  {
    // write interaction keyword's form ID (KNAM)
    output.write(reinterpret_cast<const char*>(&cKNAM), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&interactionKeywordFormID), 4);
  }

  return output.good();
}
#endif

bool ActivatorRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  if (isDeleted())
    return true;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  // read editor ID (EDID)
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
    return false;

  unknownVMAD.setPresence(false);
  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  destStruct tempDestStruct;
  bool hasBegunDestStruct = false;
  destructionStructures.clear();
  unknownDEST = std::nullopt;
  keywords.clear();
  interactionKeywordFormID = 0;
  loopingSoundFormID = 0;
  activateSoundFormID = 0;
  waterTypeFormID = 0;
  activateTextOverride.reset();
  bool hasReadOBND = false;
  defaultPrimitiveColour = {};
  unknownFNAM = std::nullopt;

  while (bytesRead < readSize)
  {
    // read next record
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cerr << "Error: ACTI seems to have more than one VMAD subrecord.\n";
             return false;
           }
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
             return false;
           bytesRead += (2 + unknownVMAD.size());
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cerr << "Error: ACTI seems to have more than one OBND subrecord.\n";
             return false;
           }
           // OBND's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: sub record OBND of ACTI has invalid length ("
                       << subLength << " bytes). Should be 12 bytes.\n";
             return false;
           }
           // read OBND
           in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord OBND of ACTI!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: ACTI seems to have more than one FULL subrecord.\n";
             return false;
           }
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: ACTI seems to have more than one MODL subrecord.\n";
             return false;
           }
           // read model path (MODL)
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           if (modelPath.empty())
           {
             std::cerr << "Error: Subrecord MODL of ACTI is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: ACTI seems to have more than one MODT subrecord.\n";
             return false;
           }
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading subrecord MODT of ACTI!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODT.size();
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cerr << "Error: ACTI seems to have more than one MODS subrecord.\n";
             return false;
           }
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
           {
             std::cerr << "Error while reading subrecord MODS of ACTI!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODS.size();
           break;
      case cDEST:
           if (unknownDEST.has_value())
           {
             std::cerr << "Error: ACTI seems to have more than one DEST subrecord.\n";
             return false;
           }
           // DEST's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: sub record DEST of ACTI has invalid length ("
                       << subLength << " bytes). Should be 8 bytes.\n";
             return false;
           }
           // read DEST
           unknownDEST = 0;
           in_File.read(reinterpret_cast<char*>(&unknownDEST.value()), 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DEST of ACTI!\n";
             return false;
           }
           break;
      case cDSTD:
           if (!hasBegunDestStruct)
           {
             tempDestStruct.reset();
           }
           if (tempDestStruct.hasDSTD)
           {
             std::cerr << "Error: ACTI seems to have more than one DSTD subrecord.\n";
             return false;
           }
           // DSTD's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 20)
           {
             std::cerr <<"Error: sub record DSTD of ACTI has invalid length ("
                       << subLength << " bytes). Should be 20 bytes.\n";
             return false;
           }
           // read DSTD
           in_File.read(reinterpret_cast<char*>(tempDestStruct.unknownDSTD.data()), 20);
           bytesRead += 20;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DSTD of ACTI!\n";
             return false;
           }
           tempDestStruct.hasDSTD = true;
           hasBegunDestStruct = true;
           break;
      case cDMDL:
           if (!hasBegunDestStruct)
           {
             tempDestStruct.reset();
           }
           if (!tempDestStruct.destroyedModelPath.empty())
           {
             std::cerr << "Error: ACTI seems to have more than one DMDL subrecord.\n";
             return false;
           }
           if (!loadString512FromStream(in_File, tempDestStruct.destroyedModelPath, buffer, cDMDL, false, bytesRead))
             return false;
           hasBegunDestStruct = true;
           break;
      case cDMDT:
           if (!hasBegunDestStruct)
           {
             tempDestStruct.reset();
           }
           if (tempDestStruct.unknownDMDT.isPresent())
           {
             std::cerr << "Error: ACTI seems to have more than one DMDT subrecord.\n";
             return false;
           }
           if (!tempDestStruct.unknownDMDT.loadFromStream(in_File, cDMDT, false))
           {
             std::cerr << "Error while reading subrecord DMDT of ACTI!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + tempDestStruct.unknownDMDT.size();
           hasBegunDestStruct = true;
           break;
      case cDMDS:
           if (!hasBegunDestStruct)
           {
             tempDestStruct.reset();
           }
           if (tempDestStruct.unknownDMDS.isPresent())
           {
             std::cerr << "Error: ACTI seems to have more than one DMDS subrecord.\n";
             return false;
           }
           if (!tempDestStruct.unknownDMDS.loadFromStream(in_File, cDMDS, false))
           {
             std::cerr << "Error while reading subrecord DMDS of ACTI!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + tempDestStruct.unknownDMDS.size();
           hasBegunDestStruct = true;
           break;
      case cDSTF:
           if (!hasBegunDestStruct)
           {
             std::cerr << "Error while reading record of type ACTI: no data prior to DSTF.\n";
             return false;
           }
           // DSTF's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 0)
           {
             std::cerr << "Error: sub record DSTF of ACTI has invalid length ("
                       << subLength << " bytes). Should be zero bytes!\n";
             return false;
           }
           destructionStructures.push_back(tempDestStruct);
           hasBegunDestStruct = false;
           break;
      case cKSIZ:
           if (!loadKeywords(in_File, keywords, bytesRead))
             return false;
           break;
      case cPNAM:
           if (defaultPrimitiveColour.has_value())
           {
             std::cerr << "Error: ACTI seems to have more than one PNAM subrecord.\n";
             return false;
           }
           // PNAM's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 4)
           {
             std::cerr << "Error: sub record PNAM of ACTI has invalid length ("
                       << subLength << " bytes). Should be four bytes!\n";
             return false;
           }
           // read PNAM
           defaultPrimitiveColour = Colour();
           in_File.read(reinterpret_cast<char*>(&defaultPrimitiveColour.value().red), 1);
           in_File.read(reinterpret_cast<char*>(&defaultPrimitiveColour.value().green), 1);
           in_File.read(reinterpret_cast<char*>(&defaultPrimitiveColour.value().blue), 1);
           in_File.seekg(1, std::ios_base::cur); // skip fourth byte, it is zero anyways
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord PNAM of ACTI!\n";
             return false;
           }
           bytesRead += 4;
           break;
      case cSNAM:
           if (loopingSoundFormID != 0)
           {
             std::cerr << "Error: ACTI seems to have more than one SNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, loopingSoundFormID, false))
             return false;
           bytesRead += 6;
           if (loopingSoundFormID == 0)
           {
             std::cerr << "Error: subrecord SNAM of ACTI has value zero!\n";
             return false;
           }
           break;
      case cVNAM:
           if (activateSoundFormID != 0)
           {
             std::cerr << "Error: ACTI seems to have more than one VNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cVNAM, activateSoundFormID, false))
             return false;
           bytesRead += 6;
           if (activateSoundFormID == 0)
           {
             std::cerr << "Error: subrecord VNAM of ACTI has value zero!\n";
             return false;
           }
           break;
      case cWNAM:
           if (waterTypeFormID != 0)
           {
             std::cerr << "Error: ACTI seems to have more than one WNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cWNAM, waterTypeFormID, false))
             return false;
           bytesRead += 6;
           if (waterTypeFormID == 0)
           {
             std::cerr << "Error: subrecord WNAM of ACTI has value zero!\n";
             return false;
           }
           break;
      case cRNAM:
           if (activateTextOverride.isPresent())
           {
             std::cerr << "Error: ACTI seems to have more than one RNAM subrecord.\n";
             return false;
           }
           if (!activateTextOverride.loadFromStream(in_File, cRNAM, false, bytesRead, localized, table, buffer))
             return false;
           if (localized && (activateTextOverride.getIndex() == 0))
           {
             std::cerr << "Error: subrecord RNAM of ACTI has value zero!\n";
             return false;
           }
           break;
      case cFNAM:
           if (unknownFNAM.has_value())
           {
             std::cerr << "Error: ACTI seems to have more than one FNAM subrecord.\n";
             return false;
           }
           // FNAM's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 2)
           {
             std::cerr << "Error: sub record FNAM of ACTI has invalid length ("
                       << subLength << " bytes). Should be two bytes.\n";
             return false;
           }
           // read FNAM
           unknownFNAM = 0;
           in_File.read(reinterpret_cast<char*>(&unknownFNAM.value()), 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord FNAM of ACTI!\n";
             return false;
           }
           break;
      case cKNAM:
           if (interactionKeywordFormID != 0)
           {
             std::cerr << "Error: ACTI seems to have more than one KNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cKNAM, interactionKeywordFormID, false))
             return false;
           bytesRead += 6;
           if (interactionKeywordFormID == 0)
           {
             std::cerr << "Error: subrecord KNAM of ACTI has value zero!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: found unexpected subrecord \"" << IntTo4Char(subRecName)
                     << "\", but only VMAD, OBND, FULL, PNAM, FNAM, SNAM or KNAM are allowed here!\n";
           return false;
    }
  }

  // check presence of all required subrecords
  if (!hasReadOBND)
  {
    std::cerr << "Error: At least one required subrecord of ACTI was not found!\n";
    return false;
  }
  if (hasBegunDestStruct)
  {
    std::cerr << "Error: Unfinished DSTF stuff in ACTI!\n";
    return false;
  }
  return in_File.good();
}

uint32_t ActivatorRecord::getRecordType() const
{
  return cACTI;
}

} // namespace
