/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021  Thoronador

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

#include "WorldSpaceRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

WorldSpaceRecord::WorldSpaceRecord()
: BasicRecord(),
  editorID(""),
  unknownRNAMs(std::vector<BinarySubRecord>()),
  unknownMHDT(BinarySubRecord()),
  name(LocalizedString()),
  hasWCTR(false),
  centerCellX(0),
  centerCellY(0),
  interiorLightingFormID(0),
  encounterZoneFormID(0),
  climateFormID(0),
  locationFormID(0),
  parentWorldSpaceFormID(0),
  unknownPNAM(std::nullopt),
  waterFormID(0),
  LODWaterTypeFormID(0),
  LODWaterHeight(std::nullopt),
  unknownDNAM(std::nullopt),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownMNAM(BinarySubRecord()),
  unknownONAM(BinarySubRecord()),
  distantLODMultiplier(0.0f),
  unknownDATA(std::nullopt),
  unknownNAM0(std::nullopt),
  unknownNAM9(std::nullopt),
  musicFormID(0),
  HD_LOD_DiffuseTexture(""),
  HD_LOD_NormalTexture(""),
  unknownXWEM(""),
  unknownOFST(BinarySubRecordExtended())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool WorldSpaceRecord::equals(const WorldSpaceRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownRNAMs == other.unknownRNAMs) && (unknownMHDT == other.unknownMHDT)
      && (name == other.name)
      && (hasWCTR == other.hasWCTR) && (((centerCellX == other.centerCellX) && (centerCellY == other.centerCellY)) || (!hasWCTR))
      && (interiorLightingFormID == other.interiorLightingFormID)
      && (encounterZoneFormID == other.encounterZoneFormID)
      && (climateFormID == other.climateFormID)
      && (locationFormID == other.locationFormID)
      && (parentWorldSpaceFormID == other.parentWorldSpaceFormID)
      && (unknownPNAM == other.unknownPNAM)
      && (waterFormID == other.waterFormID)
      && (LODWaterTypeFormID == other.LODWaterTypeFormID)
      && (LODWaterHeight == other.LODWaterHeight)
      && (unknownDNAM == other.unknownDNAM)
      && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
      && (unknownMNAM == other.unknownMNAM)
      && (unknownONAM == other.unknownONAM)
      && (distantLODMultiplier == other.distantLODMultiplier)
      && (unknownDATA == other.unknownDATA)
      && (unknownNAM0 == other.unknownNAM0)
      && (unknownNAM9 == other.unknownNAM9)
      && (musicFormID == other.musicFormID)
      && (HD_LOD_DiffuseTexture == other.HD_LOD_DiffuseTexture)
      && (HD_LOD_NormalTexture == other.HD_LOD_NormalTexture)
      && (unknownXWEM == other.unknownXWEM)
      && (unknownOFST == other.unknownOFST);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t WorldSpaceRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* NAMA */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  for (const auto& rnam: unknownRNAMs)
  {
    if (rnam.isPresent())
    {
      writeSize = writeSize + 4 /* RNAM */ + 2 /* 2 bytes for length */ + rnam.size() /* size */;
    }
  }
  if (unknownMHDT.isPresent())
  {
    writeSize = writeSize + 4 /* MHDT */ + 2 /* 2 bytes for length */ + unknownMHDT.size() /* size */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }
  if (hasWCTR)
  {
    writeSize = writeSize + 4 /* WCTR */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (interiorLightingFormID != 0)
  {
    writeSize = writeSize + 4 /* LTMP */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (encounterZoneFormID != 0)
  {
    writeSize = writeSize + 4 /* XEZN */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (climateFormID != 0)
  {
    writeSize = writeSize + 4 /* CNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (locationFormID != 0)
  {
    writeSize = writeSize + 4 /* XLCN */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (parentWorldSpaceFormID != 0)
  {
    writeSize = writeSize + 4 /* WNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (unknownPNAM.has_value())
  {
    writeSize = writeSize + 4 /* PNAM */ + 2 /* 2 bytes for length */ + 2 /* fixed size */;
  }
  if (waterFormID != 0)
  {
    writeSize = writeSize + 4 /* NAM2 */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (LODWaterTypeFormID != 0)
  {
    writeSize = writeSize + 4 /* NAM3 */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (LODWaterHeight.has_value())
  {
    writeSize = writeSize + 4 /* NAM4 */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (unknownDNAM.has_value())
  {
    writeSize = writeSize + 4 /* DNAM */ + 2 /* 2 bytes for length */ + 8 /* fixed size */;
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
  if (unknownMNAM.isPresent())
  {
    writeSize = writeSize + 4 /* MNAM */ + 2 /* 2 bytes for length */ + unknownMNAM.size() /* size */;
  }
  if (unknownONAM.isPresent())
  {
    writeSize = writeSize + 4 /* ONAM */ + 2 /* 2 bytes for length */ + unknownONAM.size() /* size */;
  }
  if (unknownDATA.has_value())
  {
    writeSize = writeSize + 4 /* DATA */ + 2 /* 2 bytes for length */ + 1 /* fixed size */;
  }
  if (unknownNAM0.has_value())
  {
    writeSize = writeSize + 4 /* NAM0 */ + 2 /* 2 bytes for length */ + 8 /* fixed size */;
  }
  if (unknownNAM9.has_value())
  {
    writeSize = writeSize + 4 /* NAM9 */ + 2 /* 2 bytes for length */ + 8 /* fixed size */;
  }
  if (musicFormID != 0)
  {
    writeSize = writeSize + 4 /* ZNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (!HD_LOD_DiffuseTexture.empty())
  {
    writeSize = writeSize + 4 /* TNAM */ + 2 /* 2 bytes for length */
        + HD_LOD_DiffuseTexture.length() + 1 /* length of path +1 byte for NUL termination */;
  }
  if (!HD_LOD_NormalTexture.empty())
  {
    writeSize = writeSize + 4 /* UNAM */ + 2 /* 2 bytes for length */
        + HD_LOD_NormalTexture.length() + 1 /* length of path +1 byte for NUL termination */;
  }
  if (!unknownXWEM.empty())
  {
    writeSize = writeSize + 4 /* XWEM */ + 2 /* 2 bytes for length */
        + unknownXWEM.length() + 1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownOFST.isPresent())
  {
    if (unknownOFST.size() < 65536)
    {
      writeSize = writeSize + 4 /* OFST */ + 2 /* 2 bytes for length */ + unknownOFST.size() /* size */;
    }
    else
    {
      writeSize = writeSize + 4 /* XXXX */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
                 + 4 /* OFST */ + 2 /* 2 bytes for length */ + unknownOFST.size() /* size */;
    }
  }
  return writeSize;
}

bool WorldSpaceRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cWRLD), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  for (const auto& rnam: unknownRNAMs)
  {
    if (rnam.isPresent())
    {
      if (!rnam.saveToStream(output, cRNAM))
      {
        std::cerr << "Error while writing subrecord RNAM of WRLD!\n";
        return false;
      }
    }
  }

  if (unknownMHDT.isPresent())
  {
    if (!unknownMHDT.saveToStream(output, cMHDT))
    {
      std::cerr << "Error while writing subrecord MHDT of WRLD!\n";
      return false;
    }
  }

  if (name.isPresent())
  {
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  if (hasWCTR)
  {
    // write WCTR
    output.write(reinterpret_cast<const char*>(&cWCTR), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&centerCellX), 2);
    output.write(reinterpret_cast<const char*>(&centerCellY), 2);
  }

  if (interiorLightingFormID != 0)
  {
    // write LTMP
    output.write(reinterpret_cast<const char*>(&cLTMP), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&interiorLightingFormID), 4);
  }

  if (encounterZoneFormID != 0)
  {
    // write encounter zone form ID (XEZN)
    output.write(reinterpret_cast<const char*>(&cXEZN), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&encounterZoneFormID), 4);
  }

  if (climateFormID != 0)
  {
    // write climate's form ID (CNAM)
    output.write(reinterpret_cast<const char*>(&cCNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&climateFormID), 4);
  }

  if (locationFormID != 0)
  {
    // write location form ID (XLCN)
    output.write(reinterpret_cast<const char*>(&cXLCN), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&locationFormID), 4);
  }

  if (parentWorldSpaceFormID != 0)
  {
    // write parent world space's form ID (WNAM)
    output.write(reinterpret_cast<const char*>(&cWNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&parentWorldSpaceFormID), 4);
  }

  if (unknownPNAM.has_value())
  {
    // write PNAM
    output.write(reinterpret_cast<const char*>(&cPNAM), 4);
    subLength = 2;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownPNAM.value()), 2);
  }

  if (waterFormID != 0)
  {
    // write water type form ID (NAM2)
    output.write(reinterpret_cast<const char*>(&cNAM2), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&waterFormID), 4);
  }

  if (LODWaterTypeFormID != 0)
  {
    // write LOD water type (NAM3)
    output.write(reinterpret_cast<const char*>(&cNAM3), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&LODWaterTypeFormID), 4);
  }

  if (LODWaterHeight.has_value())
  {
    // write LOD water height (NAM4)
    output.write(reinterpret_cast<const char*>(&cNAM4), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&LODWaterHeight), 4);
  }

  if (unknownDNAM.has_value())
  {
    // write DNAM
    output.write(reinterpret_cast<const char*>(&cDNAM), 4);
    subLength = 8;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownDNAM.value()), 8);
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
      std::cerr << "Error while writing subrecord MODT of WRLD!\n";
      return false;
    }
  }

  if (unknownMNAM.isPresent())
  {
    if (!unknownMNAM.saveToStream(output, cMNAM))
    {
      std::cerr << "Error while writing subrecord MNAM of WRLD!\n";
      return false;
    }
  }

  if (unknownONAM.isPresent())
  {
    if (!unknownONAM.saveToStream(output, cONAM))
    {
      std::cerr << "Error while writing subrecord ONAM of WRLD!\n";
      return false;
    }
  }

  // write distant LOD multiplier (NAMA)
  output.write(reinterpret_cast<const char*>(&cNAMA), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&distantLODMultiplier), 4);

  if (unknownDATA.has_value())
  {
    // write DATA
    output.write(reinterpret_cast<const char*>(&cDATA), 4);
    subLength = 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownDATA.value()), 1);
  }

  if (unknownNAM0.has_value())
  {
    // write NAM0
    output.write(reinterpret_cast<const char*>(&cNAM0), 4);
    subLength = 8;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownNAM0.value()), 8);
  }

  if (unknownNAM9.has_value())
  {
    // write NAM9
    output.write(reinterpret_cast<const char*>(&cNAM9), 4);
    subLength = 8;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownNAM9.value()), 8);
  }

  if (musicFormID != 0)
  {
    // write music form ID (ZNAM)
    output.write(reinterpret_cast<const char*>(&cZNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&musicFormID), 4);
  }

  if (!HD_LOD_DiffuseTexture.empty())
  {
    // write HD LOD Diffuse texture path (TNAM)
    output.write(reinterpret_cast<const char*>(&cTNAM), 4);
    subLength = HD_LOD_DiffuseTexture.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(HD_LOD_DiffuseTexture.c_str(), subLength);
  }

  if (!HD_LOD_NormalTexture.empty())
  {
    // write HD LOD Normal texture path (UNAM)
    output.write(reinterpret_cast<const char*>(&cUNAM), 4);
    subLength = HD_LOD_NormalTexture.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(HD_LOD_NormalTexture.c_str(), subLength);
  }

  if (!unknownXWEM.empty())
  {
    // write XWEM
    output.write(reinterpret_cast<const char*>(&cXWEM), 4);
    subLength = unknownXWEM.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(unknownXWEM.c_str(), subLength);
  }

  if (unknownOFST.isPresent())
  {
    if (!unknownOFST.saveToStream(output, cOFST))
    {
      std::cerr << "Error while writing subrecord OFST of WRLD!\n";
      return false;
    }
  }

  return output.good();
}
#endif

bool WorldSpaceRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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

  unknownRNAMs.clear();
  BinarySubRecord tempBin;
  unknownMHDT.setPresence(false);
  name.reset();
  hasWCTR = false; centerCellX = 0; centerCellY = 0;
  interiorLightingFormID = 0;
  encounterZoneFormID = 0;
  climateFormID = 0;
  waterFormID = 0;
  LODWaterTypeFormID = 0;
  LODWaterHeight.reset();
  unknownDNAM.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMNAM.setPresence(false);
  locationFormID = 0;
  parentWorldSpaceFormID = 0;
  unknownPNAM.reset();
  unknownONAM.setPresence(false);
  bool hasReadNAMA = false;
  unknownDATA.reset();
  unknownNAM0.reset();
  unknownNAM9.reset();
  musicFormID = 0;
  HD_LOD_DiffuseTexture.clear();
  HD_LOD_NormalTexture.clear();
  unknownXWEM.clear();
  uint32_t sizeXXXX = 0;
  unknownOFST.setPresence(false);
  while (bytesRead < readSize)
  {
    // read next subrecord
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cRNAM:
           if (!tempBin.loadFromStream(in_File, cRNAM, false))
           {
             std::cerr << "Error while reading subrecord RNAM of WRLD!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + tempBin.size();
           unknownRNAMs.push_back(tempBin);
           break;
      case cMHDT:
           if (unknownMHDT.isPresent())
           {
             std::cerr << "Error: WRLD seems to have more than one MHDT subrecord.\n";
             return false;
           }
           if (!unknownMHDT.loadFromStream(in_File, cMHDT, false))
           {
             std::cerr << "Error while reading subrecord MHDT of WRLD!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMHDT.size();
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: WRLD seems to have more than one FULL subrecord.\n";
             return false;
           }
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cWCTR:
           if (hasWCTR)
           {
             std::cerr << "Error: WRLD seems to have more than one WCTR subrecord.\n";
             return false;
           }
           // WCTR's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 4)
           {
             std::cerr << "Error: Sub record WCTR of WRLD has invalid length ("
                       << subLength << " bytes). Should be four bytes.\n";
             return false;
           }
           // read WCTR
           in_File.read(reinterpret_cast<char*>(&centerCellX), 2);
           in_File.read(reinterpret_cast<char*>(&centerCellY), 2);
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord WCTR of WRLD!\n";
             return false;
           }
           bytesRead += 4;
           hasWCTR = true;
           break;
      case cLTMP:
           if (interiorLightingFormID != 0)
           {
             std::cerr << "Error: WRLD seems to have more than one LTMP subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cLTMP, interiorLightingFormID, false))
             return false;
           bytesRead += 6;
           if (interiorLightingFormID == 0)
           {
             std::cerr << "Error: Subrecord LTMP of WRLD has value zero!\n";
             return false;
           }
           break;
      case cXEZN:
           if (encounterZoneFormID != 0)
           {
             std::cerr << "Error: WRLD seems to have more than one XEZN subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cXEZN, encounterZoneFormID, false))
             return false;
           bytesRead += 6;
           if (encounterZoneFormID == 0)
           {
             std::cerr << "Error: Subrecord XEZN of WRLD has value zero!\n";
             return false;
           }
           break;
      case cCNAM:
           if (climateFormID != 0)
           {
             std::cerr << "Error: WRLD seems to have more than one CNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cCNAM, climateFormID, false))
             return false;
           bytesRead += 6;
           if (climateFormID == 0)
           {
             std::cerr << "Error: Subrecord CNAM of WRLD has value zero!\n";
             return false;
           }
           break;
      case cNAM2:
           if (waterFormID != 0)
           {
             std::cerr << "Error: WRLD seems to have more than one NAM2 subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cNAM2, waterFormID, false))
             return false;
           bytesRead += 6;
           if (waterFormID == 0)
           {
             std::cerr << "Error: Subrecord NAM2 of WRLD has value zero!\n";
             return false;
           }
           break;
      case cNAM3:
           if (LODWaterTypeFormID != 0)
           {
             std::cerr << "Error: WRLD seems to have more than one NAM3 subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cNAM3, LODWaterTypeFormID, false))
             return false;
           bytesRead += 6;
           if (LODWaterTypeFormID == 0)
           {
             std::cerr << "Error: Subrecord NAM3 of WRLD has value zero!\n";
             return false;
           }
           break;
      case cNAM4:
           if (LODWaterHeight.has_value())
           {
             std::cerr << "Error: WRLD seems to have more than one NAM4 subrecord.\n";
             return false;
           }
           // read NAM4's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 4)
           {
             std::cerr << "Error: Sub record NAM4 of WRLD has invalid length ("
                       << subLength << " bytes). Should be four bytes.\n";
             return false;
           }
           // read NAM4's stuff
           LODWaterHeight = 0.0f;
           in_File.read(reinterpret_cast<char*>(&LODWaterHeight.value()), 4);
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord NAM4 of WRLD!\n";
             return false;
           }
           bytesRead += 4;
           break;
      case cDNAM:
           if (unknownDNAM.has_value())
           {
             std::cerr << "Error: WRLD seems to have more than one DNAM subrecord.\n";
             return false;
           }
           // read DNAM's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: Sub record DNAM of WRLD has invalid length ("
                       << subLength << " bytes). Should be eight bytes.\n";
             return false;
           }
           // read DNAM's stuff
           unknownDNAM = 0;
           in_File.read(reinterpret_cast<char*>(&unknownDNAM.value()), 8);
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DNAM of WRLD!\n";
             return false;
           }
           bytesRead += 8;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: WRLD seems to have more than one MODL subrecord.\n";
             return false;
           }
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           if (modelPath.empty())
           {
             std::cerr << "Error: Subrecord MODL of WRLD is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: WRLD seems to have more than one MODT subrecord.\n";
             return false;
           }
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading subrecord MODT of WRLD!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODT.size();
           break;
      case cMNAM:
           if (unknownMNAM.isPresent())
           {
             std::cerr << "Error: WRLD seems to have more than one MNAM subrecord.\n";
             return false;
           }
           if (!unknownMNAM.loadFromStream(in_File, cMNAM, false))
           {
             std::cerr << "Error while reading subrecord MNAM of WRLD!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMNAM.size();
           break;
      case cXLCN:
           if (locationFormID != 0)
           {
             std::cerr << "Error: WRLD seems to have more than one XLCN subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cXLCN, locationFormID, false))
             return false;
           bytesRead += 6;
           if (locationFormID == 0)
           {
             std::cerr << "Error: Subrecord XLCN of WRLD has value zero!\n";
             return false;
           }
           break;
      case cWNAM:
           if (parentWorldSpaceFormID != 0)
           {
             std::cerr << "Error: WRLD seems to have more than one WNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cWNAM, parentWorldSpaceFormID, false))
             return false;
           bytesRead += 6;
           if (parentWorldSpaceFormID == 0)
           {
             std::cerr << "Error: Subrecord WNAM of WRLD has value zero!\n";
             return false;
           }
           break;
      case cPNAM:
           if (unknownPNAM.has_value())
           {
             std::cerr << "Error: WRLD seems to have more than one PNAM subrecord.\n";
             return false;
           }
           // read PNAM's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 2)
           {
             std::cerr << "Error: Sub record PNAM of WRLD has invalid length ("
                       << subLength << " bytes). Should be two bytes.\n";
             return false;
           }
           // read PNAM's stuff
           unknownPNAM = 0;
           in_File.read(reinterpret_cast<char*>(&unknownPNAM.value()), 2);
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord PNAM of WRLD!\n";
             return false;
           }
           bytesRead += 2;
           break;
      case cONAM:
           if (unknownONAM.isPresent())
           {
             std::cerr << "Error: WRLD seems to have more than one ONAM subrecord.\n";
             return false;
           }
           if (!unknownONAM.loadFromStream(in_File, cONAM, false))
           {
             std::cerr << "Error while reading subrecord ONAM of WRLD!\n";
             return false;
           }
           // ONAM's length
           if (unknownONAM.size() != 16)
           {
             std::cerr << "Error: Sub record ONAM of WRLD has invalid length ("
                       << unknownONAM.size() << " bytes). Should be 16 bytes.\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownONAM.size();
           break;
      case cNAMA:
           if (hasReadNAMA)
           {
             std::cerr << "Error: WRLD seems to have more than one NAMA subrecord.\n";
             return false;
           }
           // read NAMA's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 4)
           {
             std::cerr << "Error: Sub record NAMA of WRLD has invalid length ("
                       << subLength << " bytes). Should be four bytes.\n";
             return false;
           }
           // read NAMA's stuff
           in_File.read(reinterpret_cast<char*>(&distantLODMultiplier), 4);
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord NAMA of WRLD!\n";
             return false;
           }
           bytesRead += 4;
           hasReadNAMA = true;
           break;
      case cDATA:
           if (unknownDATA.has_value())
           {
             std::cerr << "Error: WRLD seems to have more than one DATA subrecord.\n";
             return false;
           }
           // read DATA's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 1)
           {
             std::cerr << "Error: Sub record DATA of WRLD has invalid length ("
                       << subLength << " bytes). Should be one byte.\n";
             return false;
           }
           // read DATA's stuff
           unknownDATA = 0x00;
           in_File.read(reinterpret_cast<char*>(&unknownDATA.value()), 1);
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of WRLD!\n";
             return false;
           }
           bytesRead += 1;
           break;
      case cNAM0:
           if (unknownNAM0.has_value())
           {
             std::cerr << "Error: WRLD seems to have more than one NAM0 subrecord.\n";
             return false;
           }
           // read NAM0's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: Sub record NAM0 of WRLD has invalid length ("
                       << subLength << " bytes). Should be eight bytes.\n";
             return false;
           }
           // read NAM0's stuff
           unknownNAM0 = 0;
           in_File.read(reinterpret_cast<char*>(&unknownNAM0.value()), 8);
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord NAM0 of WRLD!\n";
             return false;
           }
           bytesRead += 8;
           break;
      case cNAM9:
           if (unknownNAM9.has_value())
           {
             std::cerr << "Error: WRLD seems to have more than one NAM9 subrecord.\n";
             return false;
           }
           // read NAM9's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: Sub record NAM9 of WRLD has invalid length ("
                       << subLength << " bytes). Should be eight bytes.\n";
             return false;
           }
           // read NAM9's stuff
           unknownNAM9 = 0;
           in_File.read(reinterpret_cast<char*>(&unknownNAM9.value()), 8);
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord NAM9 of WRLD!\n";
             return false;
           }
           bytesRead += 8;
           break;
      case cZNAM:
           if (musicFormID != 0)
           {
             std::cerr << "Error: WRLD seems to have more than one ZNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cZNAM, musicFormID, false))
             return false;
           bytesRead += 6;
           if (musicFormID == 0)
           {
             std::cerr << "Error: Subrecord ZNAM of WRLD has value zero!\n";
             return false;
           }
           break;
      case cTNAM:
           if (!HD_LOD_DiffuseTexture.empty())
           {
             std::cerr << "Error: WRLD seems to have more than one TNAM subrecord.\n";
             return false;
           }
           if (!loadString512FromStream(in_File, HD_LOD_DiffuseTexture, buffer, cTNAM, false, bytesRead))
             return false;
           // check content
           if (HD_LOD_DiffuseTexture.empty())
           {
             std::cerr << "Error: Subrecord TNAM of WRLD is empty!\n";
             return false;
           }
           break;
      case cUNAM:
           if (!HD_LOD_NormalTexture.empty())
           {
             std::cerr << "Error: WRLD seems to have more than one UNAM subrecord.\n";
             return false;
           }
           if (!loadString512FromStream(in_File, HD_LOD_NormalTexture, buffer, cUNAM, false, bytesRead))
             return false;
           // check content
           if (HD_LOD_NormalTexture.empty())
           {
             std::cerr << "Error: Subrecord UNAM of WRLD is empty!\n";
             return false;
           }
           break;
      case cXWEM:
           if (!unknownXWEM.empty())
           {
             std::cerr << "Error: WRLD seems to have more than one XWEM subrecord.\n";
             return false;
           }
           if (!loadString512FromStream(in_File, unknownXWEM, buffer, cXWEM, false, bytesRead))
             return false;
           // check content
           if (unknownXWEM.empty())
           {
             std::cerr << "Error: Subrecord XWEM of WRLD is empty!\n";
             return false;
           }
           break;
      case cXXXX:
           if (sizeXXXX != 0)
           {
             std::cerr << "Error: WRLD seems to have more than one XXXX subrecord in a row.\n";
             return false;
           }
           // read XXXX
           if (!loadUint32SubRecordFromStream(in_File, cXXXX, sizeXXXX, false))
             return false;
           bytesRead += 6;
           if (0 == sizeXXXX)
           {
             std::cerr << "Error: Subrecord XXXX of WRLD has value zero!\n";
             return false;
           }
           break;
      case cOFST:
           if (unknownOFST.isPresent())
           {
             std::cerr << "Error: WRLD seems to have more than one OFST subrecord.\n";
             return false;
           }
           // read OFST
           if (0 == sizeXXXX)
           {
             if (!unknownOFST.loadFromStream(in_File, cOFST, false))
             {
               std::cerr << "Error while reading subrecord OFST of WRLD!\n";
               return false;
             }
           }
           else
           {
             // load extended version of subrecord
             if (!unknownOFST.loadFromStreamExtended(in_File, cOFST, false, sizeXXXX))
             {
               std::cerr << "Error while reading subrecord OFST of WRLD!\n";
               return false;
             }
             sizeXXXX = 0;
           }

           bytesRead = bytesRead + 2 + unknownOFST.size();
           break;
      default:
           std::cerr << "Error: Unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only RNAM, MHDT, FULL, WCTR, LTMP, XEZN,"
                     << " CNAM, NAM2, NAM3, NAM4, DNAM, MODL, MODT, MNAM, XLCN,"
                     << " WNAM, PNAM, ONAM, NAMA, DATA, NAM0, NAM9, ZNAM, TNAM,"
                     << " UNAM, XWEM, XXXX or OFST are allowed here!\n";
           return false;
           break;
    }
  }

  // presence checks
  if (!hasReadNAMA || !unknownOFST.isPresent())
  {
    std::cerr << "Error while reading WRLD record: At least one required subrecord is missing!\n";
    std::cerr << "NAMA: " << hasReadNAMA << "    OFST: " << unknownOFST.isPresent() << "\n";
    return false;
  }

  return in_File.good();
}

uint32_t WorldSpaceRecord::getRecordType() const
{
  return cWRLD;
}

} // namespace
