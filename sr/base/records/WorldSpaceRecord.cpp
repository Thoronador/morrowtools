/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#include "WorldSpaceRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

WorldSpaceRecord::WorldSpaceRecord()
: BasicRecord(), editorID(""),
  unknownRNAMs(std::vector<BinarySubRecord>()),
  unknownMHDT(BinarySubRecord()),
  name(LocalizedString()),
  hasWCTR(false),
  centerCellX(0),
  centerCellY(0),
  interiorLightingFormID(0),
  encounterZoneFormID(0),
  climateFormID(0),
  waterFormID(0),
  LODWaterTypeFormID(0),
  hasNAM4(false),
  LODWaterHeight(0.0f),
  hasDNAM(false), unknownDNAM(0),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownMNAM(BinarySubRecord()),
  locationFormID(0),
  parentWorldSpaceFormID(0),
  hasPNAM(false), unknownPNAM(0),
  unknownONAM(BinarySubRecord()),
  distantLODMultiplier(0.0f),
  hasDATA(false), unknownDATA(0),
  hasNAM0(false), unknownNAM0(0),
  hasNAM9(false), unknownNAM9(0),
  musicFormID(0),
  HD_LOD_DiffuseTexture(""),
  HD_LOD_NormalTexture(""),
  unknownXWEM(""),
  unknownOFST(BinarySubRecordExtended())
{

}

WorldSpaceRecord::~WorldSpaceRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool WorldSpaceRecord::equals(const WorldSpaceRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownRNAMs==other.unknownRNAMs) and (unknownMHDT==other.unknownMHDT)
      and (name==other.name)
      and (hasWCTR==other.hasWCTR) and (((centerCellX==other.centerCellX) and (centerCellY==other.centerCellY)) or (!hasWCTR))
      and (interiorLightingFormID==other.interiorLightingFormID)
      and (encounterZoneFormID==other.encounterZoneFormID)
      and (climateFormID==other.climateFormID)
      and (waterFormID==other.waterFormID)
      and (LODWaterTypeFormID==other.LODWaterTypeFormID)
      and (hasNAM4==other.hasNAM4) and ((LODWaterHeight==other.LODWaterHeight) or (!hasNAM4))
      and (hasDNAM==other.hasDNAM) and ((unknownDNAM==other.unknownDNAM) or (!hasDNAM))
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (unknownMNAM==other.unknownMNAM)
      and (locationFormID==other.locationFormID)
      and (parentWorldSpaceFormID==other.parentWorldSpaceFormID)
      and (hasPNAM==other.hasPNAM) and ((unknownPNAM==other.unknownPNAM) or (!hasPNAM))
      and (unknownONAM==other.unknownONAM) and (distantLODMultiplier==other.distantLODMultiplier)
      and (hasDATA==other.hasDATA) and ((unknownDATA==other.unknownDATA) or (!hasDATA))
      and (hasNAM0==other.hasNAM0) and ((unknownNAM0==other.unknownNAM0) or (!hasNAM0))
      and (hasNAM9==other.hasNAM9) and ((unknownNAM9==other.unknownNAM9) or (!hasNAM9))
      and (musicFormID==other.musicFormID)
      and (HD_LOD_DiffuseTexture==other.HD_LOD_DiffuseTexture)
      and (HD_LOD_NormalTexture==other.HD_LOD_NormalTexture)
      and (unknownXWEM==other.unknownXWEM)
      and (unknownOFST==other.unknownOFST));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t WorldSpaceRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* NAMA */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  unsigned int i;
  for (i=0; i<unknownRNAMs.size(); ++i)
  {
    if (unknownRNAMs[i].isPresent())
    {
      writeSize = writeSize +4 /* RNAM */ +2 /* 2 bytes for length */ +unknownRNAMs[i].getSize() /* size */;
    }//if RNAM
  }//for
  if (unknownMHDT.isPresent())
  {
    writeSize = writeSize +4 /* MHDT */ +2 /* 2 bytes for length */ +unknownMHDT.getSize() /* size */;
  }//if MHDT
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  if (hasWCTR)
  {
    writeSize = writeSize +4 /* WCTR */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (interiorLightingFormID!=0)
  {
    writeSize = writeSize +4 /* LTMP */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (encounterZoneFormID!=0)
  {
    writeSize = writeSize +4 /* XEZN */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (climateFormID!=0)
  {
    writeSize = writeSize +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (waterFormID!=0)
  {
    writeSize = writeSize +4 /* NAM2 */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (LODWaterTypeFormID!=0)
  {
    writeSize = writeSize +4 /* NAM3 */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasNAM4)
  {
    writeSize = writeSize +4 /* NAM4 */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasDNAM)
  {
    writeSize = writeSize +4 /* DNAM */ +2 /* 2 bytes for length */ +8 /* fixed size */;
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
               +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* size */;
  }//if MODT
  if (unknownMNAM.isPresent())
  {
    writeSize = writeSize +4 /* MNAM */ +2 /* 2 bytes for length */ +unknownMNAM.getSize() /* size */;
  }//if MNAM
  if (locationFormID!=0)
  {
    writeSize = writeSize +4 /* XLCN */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (parentWorldSpaceFormID!=0)
  {
    writeSize = writeSize +4 /* WNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasPNAM)
  {
    writeSize = writeSize +4 /* PNAM */ +2 /* 2 bytes for length */ +2 /* fixed size */;
  }
  if (unknownONAM.isPresent())
  {
    writeSize = writeSize +4 /* ONAM */ +2 /* 2 bytes for length */ +unknownONAM.getSize() /* size */;
  }//if ONAM
  if (hasDATA)
  {
    writeSize = writeSize +4 /* DATA */ +2 /* 2 bytes for length */ +1 /* fixed size */;
  }
  if (hasNAM0)
  {
    writeSize = writeSize +4 /* NAM0 */ +2 /* 2 bytes for length */ +8 /* fixed size */;
  }
  if (hasNAM9)
  {
    writeSize = writeSize +4 /* NAM9 */ +2 /* 2 bytes for length */ +8 /* fixed size */;
  }
  if (musicFormID!=0)
  {
    writeSize = writeSize +4 /* ZNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!HD_LOD_DiffuseTexture.empty())
  {
    writeSize = writeSize + 4 /* TNAM */ +2 /* 2 bytes for length */
               +HD_LOD_DiffuseTexture.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (!HD_LOD_NormalTexture.empty())
  {
    writeSize = writeSize + 4 /* UNAM */ +2 /* 2 bytes for length */
               +HD_LOD_NormalTexture.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (!unknownXWEM.empty())
  {
    writeSize = writeSize + 4 /* XWEM */ +2 /* 2 bytes for length */
               +unknownXWEM.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownOFST.isPresent())
  {
    if (unknownOFST.getSize()<65536)
    {
      writeSize = writeSize +4 /* OFST */ +2 /* 2 bytes for length */ +unknownOFST.getSize() /* size */;
    }
    else
    {
      writeSize = writeSize +4 /* XXXX */ +2 /* 2 bytes for length */ +4 /* fixed size */
                 +4 /* OFST */ +2 /* 2 bytes for length */ +unknownOFST.getSize() /* size */;
    }
  }//if OFST
  return writeSize;
}

bool WorldSpaceRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cWRLD, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  unsigned int i;
  for (i=0; i<unknownRNAMs.size(); ++i)
  {
    if (unknownRNAMs[i].isPresent())
    {
      //write RNAM
      if (!unknownRNAMs[i].saveToStream(output, cRNAM))
      {
        std::cout << "Error while writing subrecord RNAM of WRLD!\n";
        return false;
      }
    }//if RNAM
  }//for RNAM

  if (unknownMHDT.isPresent())
  {
    //write MHDT
    if (!unknownMHDT.saveToStream(output, cMHDT))
    {
      std::cout << "Error while writing subrecord MHDT of WRLD!\n";
      return false;
    }
  }//if MHDT

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  if (hasWCTR)
  {
    //write WCTR
    output.write((const char*) &cWCTR, 4);
    //WCTR's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write WCTR
    output.write((const char*) &centerCellX, 2);
    output.write((const char*) &centerCellY, 2);
  }//if WCTR

  if (interiorLightingFormID!=0)
  {
    //write LTMP
    output.write((const char*) &cLTMP, 4);
    //LTMP's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write LTMP
    output.write((const char*) &interiorLightingFormID, 4);
  }//if LTMP

  if (encounterZoneFormID!=0)
  {
    //write XEZN
    output.write((const char*) &cXEZN, 4);
    //XEZN's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write XEZN
    output.write((const char*) &encounterZoneFormID, 4);
  }//if XEZN

  if (climateFormID!=0)
  {
    //write CNAM
    output.write((const char*) &cCNAM, 4);
    //CNAM's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write Climate's form ID
    output.write((const char*) &climateFormID, 4);
  }//if CNAM

  if (waterFormID!=0)
  {
    //write NAM2
    output.write((const char*) &cNAM2, 4);
    //NAM2's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write Water form ID
    output.write((const char*) &waterFormID, 4);
  }//if NAM2

  if (LODWaterTypeFormID!=0)
  {
    //write NAM3
    output.write((const char*) &cNAM3, 4);
    //NAM3's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write LOD Water Type
    output.write((const char*) &LODWaterTypeFormID, 4);
  }//if NAM3

  if (hasNAM4)
  {
    //write NAM4
    output.write((const char*) &cNAM4, 4);
    //NAM4's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write LOD Water Height
    output.write((const char*) &LODWaterHeight, 4);
  }//if NAM4

  if (hasDNAM)
  {
    //write DNAM
    output.write((const char*) &cDNAM, 4);
    //DNAM's length
    subLength = 8; // fixed
    output.write((const char*) &subLength, 2);
    //write DNAM
    output.write((const char*) &unknownDNAM, 8);
  }//if DNAM

  if (!modelPath.empty())
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //MODL's length
    subLength = modelPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path
    output.write(modelPath.c_str(), subLength);
  }

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of WRLD!\n";
      return false;
    }
  }//if MODT

  if (unknownMNAM.isPresent())
  {
    //write MNAM
    if (!unknownMNAM.saveToStream(output, cMNAM))
    {
      std::cout << "Error while writing subrecord MNAM of WRLD!\n";
      return false;
    }
  }//if MNAM

  if (locationFormID!=0)
  {
    //write XLCN
    output.write((const char*) &cXLCN, 4);
    //XLCN's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write XLCN
    output.write((const char*) &locationFormID, 4);
  }

  if (parentWorldSpaceFormID!=0)
  {
    //write WNAM
    output.write((const char*) &cWNAM, 4);
    //WNAM's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write WNAM
    output.write((const char*) &parentWorldSpaceFormID, 4);
  }

  if (hasPNAM)
  {
    //write PNAM
    output.write((const char*) &cPNAM, 4);
    //PNAM's length
    subLength = 2; // fixed
    output.write((const char*) &subLength, 2);
    //write PNAM
    output.write((const char*) &unknownPNAM, 2);
  }

  if (unknownONAM.isPresent())
  {
    //write ONAM
    if (!unknownONAM.saveToStream(output, cONAM))
    {
      std::cout << "Error while writing subrecord ONAM of WRLD!\n";
      return false;
    }
  }//if ONAM

  //write NAMA
  output.write((const char*) &cNAMA, 4);
  //NAMA's length
  subLength = 4; // fixed
  output.write((const char*) &subLength, 2);
  //write NAMA
  output.write((const char*) &distantLODMultiplier, 4);

  if (hasDATA)
  {
    //write DATA
    output.write((const char*) &cDATA, 4);
    //DATA's length
    subLength = 1; // fixed
    output.write((const char*) &subLength, 2);
    //write DATA
    output.write((const char*) &unknownDATA, 1);
  }

  if (hasNAM0)
  {
    //write NAM0
    output.write((const char*) &cNAM0, 4);
    //NAM0's length
    subLength = 8; // fixed
    output.write((const char*) &subLength, 2);
    //write NAM0
    output.write((const char*) &unknownNAM0, 8);
  }

  if (hasNAM9)
  {
    //write NAM9
    output.write((const char*) &cNAM9, 4);
    //NAM9's length
    subLength = 8; // fixed
    output.write((const char*) &subLength, 2);
    //write NAM9
    output.write((const char*) &unknownNAM9, 8);
  }

  if (musicFormID!=0)
  {
    //write ZNAM
    output.write((const char*) &cZNAM, 4);
    //ZNAM's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write ZNAM
    output.write((const char*) &musicFormID, 1);
  }

  if (!HD_LOD_DiffuseTexture.empty())
  {
    //write TNAM
    output.write((const char*) &cTNAM, 4);
    //TNAM's length
    subLength = HD_LOD_DiffuseTexture.length()+1;
    output.write((const char*) &subLength, 2);
    //write HD LOD Diffuse texture path
    output.write(HD_LOD_DiffuseTexture.c_str(), subLength);
  }

  if (!HD_LOD_NormalTexture.empty())
  {
    //write UNAM
    output.write((const char*) &cUNAM, 4);
    //UNAM's length
    subLength = HD_LOD_NormalTexture.length()+1;
    output.write((const char*) &subLength, 2);
    //write HD LOD Normal texture path
    output.write(HD_LOD_NormalTexture.c_str(), subLength);
  }

  if (!unknownXWEM.empty())
  {
    //write XWEM
    output.write((const char*) &cXWEM, 4);
    //XWEM's length
    subLength = unknownXWEM.length()+1;
    output.write((const char*) &subLength, 2);
    //write XWEM's path
    output.write(unknownXWEM.c_str(), subLength);
  }

  if (unknownOFST.isPresent())
  {
    //write OFST
    if (!unknownOFST.saveToStream(output, cOFST))
    {
      std::cout << "Error while writing subrecord OFST of WRLD!\n";
      return false;
    }
  }//if OFST

  return output.good();
}
#endif

bool WorldSpaceRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  if (isDeleted()) return true;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  bytesRead = 4;
  if (subRecName!=cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  //EDID's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength>511)
  {
    std::cout <<"Error: sub record EDID of WRLD is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of WRLD!\n";
    return false;
  }
  editorID = std::string(buffer);

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
  hasNAM4 = false;
  hasDNAM = false; unknownDNAM = 0;
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMNAM.setPresence(false);
  locationFormID = 0;
  parentWorldSpaceFormID = 0;
  hasPNAM = false;
  unknownONAM.setPresence(false);
  bool hasReadNAMA = false;
  hasDATA = false;
  hasNAM0 = false;
  hasNAM9 = false;
  musicFormID = 0;
  HD_LOD_DiffuseTexture.clear();
  HD_LOD_NormalTexture.clear();
  unknownXWEM.clear();
  uint32_t sizeXXXX = 0;
  unknownOFST.setPresence(false);
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cRNAM:
           //read RNAM
           if (!tempBin.loadFromStream(in_File, cRNAM, false))
           {
             std::cout << "Error while reading subrecord RNAM of WRLD!\n";
             return false;
           }
           bytesRead = bytesRead +2 /*length value*/ +tempBin.getSize() /*data size*/;
           unknownRNAMs.push_back(tempBin);
           break;
      case cMHDT:
           if (unknownMHDT.isPresent())
           {
             std::cout << "Error: WRLD seems to have more than one MHDT subrecord.\n";
             return false;
           }
           //read MHDT
           if (!unknownMHDT.loadFromStream(in_File, cMHDT, false))
           {
             std::cout << "Error while reading subrecord MHDT of WRLD!\n";
             return false;
           }
           bytesRead = bytesRead +2 /*length value*/ +unknownMHDT.getSize() /*data size*/;
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cout << "Error: WRLD seems to have more than one FULL subrecord.\n";
             return false;
           }
           //read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cWCTR:
           if (hasWCTR)
           {
             std::cout << "Error: WRLD seems to have more than one WCTR subrecord.\n";
             return false;
           }
           //WCTR's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record WCTR of WRLD has invalid length ("
                       <<subLength<<" bytes). Should be four bytes.\n";
             return false;
           }
           //read WCTR
           in_File.read((char*) &centerCellX, 2);
           in_File.read((char*) &centerCellY, 2);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord WCTR of WRLD!\n";
             return false;
           }
           bytesRead += 4;
           hasWCTR = true;
           break;
      case cLTMP:
           if (interiorLightingFormID!=0)
           {
             std::cout << "Error: WRLD seems to have more than one LTMP subrecord.\n";
             return false;
           }
           //read LTMP
           if (!loadUint32SubRecordFromStream(in_File, cLTMP, interiorLightingFormID, false)) return false;
           bytesRead += 6;
           if (interiorLightingFormID==0)
           {
             std::cout << "Error: subrecord LTMP of WRLD has value zero!\n";
             return false;
           }
           break;
      case cXEZN:
           if (encounterZoneFormID!=0)
           {
             std::cout << "Error: WRLD seems to have more than one XEZN subrecord.\n";
             return false;
           }
           //read XEZN
           if (!loadUint32SubRecordFromStream(in_File, cXEZN, encounterZoneFormID, false)) return false;
           bytesRead += 6;
           if (encounterZoneFormID==0)
           {
             std::cout << "Error: subrecord XEZN of WRLD has value zero!\n";
             return false;
           }
           break;
      case cCNAM:
           if (climateFormID!=0)
           {
             std::cout << "Error: WRLD seems to have more than one CNAM subrecord.\n";
             return false;
           }
           //read CNAM
           if (!loadUint32SubRecordFromStream(in_File, cCNAM, climateFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (climateFormID==0)
           {
             std::cout << "Error: subrecord CNAM of WRLD has value zero!\n";
             return false;
           }
           break;
      case cNAM2:
           if (waterFormID!=0)
           {
             std::cout << "Error: WRLD seems to have more than one NAM2 subrecord.\n";
             return false;
           }
           //read NAM2
           if (!loadUint32SubRecordFromStream(in_File, cNAM2, waterFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (waterFormID==0)
           {
             std::cout << "Error: subrecord NAM2 of WRLD has value zero!\n";
             return false;
           }
           break;
      case cNAM3:
           if (LODWaterTypeFormID!=0)
           {
             std::cout << "Error: WRLD seems to have more than one NAM3 subrecord.\n";
             return false;
           }
           //read NAM3
           if (!loadUint32SubRecordFromStream(in_File, cNAM3, LODWaterTypeFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (LODWaterTypeFormID==0)
           {
             std::cout << "Error: subrecord NAM3 of WRLD has value zero!\n";
             return false;
           }
           break;
      case cNAM4:
           if (hasNAM4)
           {
             std::cout << "Error: WRLD seems to have more than one NAM4 subrecord.\n";
             return false;
           }
           //NAM4's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record NAM4 of WRLD has invalid length ("
                       <<subLength<<" bytes). Should be four bytes.\n";
             return false;
           }
           //read NAM4's stuff
           in_File.read((char*) &LODWaterHeight, 4);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NAM4 of WRLD!\n";
             return false;
           }
           bytesRead += 4;
           hasNAM4 = true;
           break;
      case cDNAM:
           if (hasDNAM)
           {
             std::cout << "Error: WRLD seems to have more than one DNAM subrecord.\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record DNAM of WRLD has invalid length ("
                       <<subLength<<" bytes). Should be eight bytes.\n";
             return false;
           }
           //read DNAM's stuff
           in_File.read((char*) &unknownDNAM, 8);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of WRLD!\n";
             return false;
           }
           bytesRead += 8;
           hasDNAM = false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: WRLD seems to have more than one MODL subrecord.\n";
             return false;
           }
           //load MODL
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           if (modelPath.empty())
           {
             std::cout << "Error: subrecord MODL of WRLD is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: WRLD seems to have more than one MODT subrecord.\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of WRLD!\n";
             return false;
           }
           bytesRead = bytesRead +2 /*length value*/ +unknownMODT.getSize() /*data size*/;
           break;
      case cMNAM:
           if (unknownMNAM.isPresent())
           {
             std::cout << "Error: WRLD seems to have more than one MNAM subrecord.\n";
             return false;
           }
           //read MNAM
           if (!unknownMNAM.loadFromStream(in_File, cMNAM, false))
           {
             std::cout << "Error while reading subrecord MNAM of WRLD!\n";
             return false;
           }
           bytesRead = bytesRead +2 /*length value*/ +unknownMNAM.getSize() /*data size*/;
           break;
      case cXLCN:
           if (locationFormID!=0)
           {
             std::cout << "Error: WRLD seems to have more than one XLCN subrecord.\n";
             return false;
           }
           //read XLCN
           if (!loadUint32SubRecordFromStream(in_File, cXLCN, locationFormID, false)) return false;
           bytesRead += 6;
           if (locationFormID==0)
           {
             std::cout << "Error: subrecord XLCN of WRLD has value zero!\n";
             return false;
           }
           break;
      case cWNAM:
           if (parentWorldSpaceFormID!=0)
           {
             std::cout << "Error: WRLD seems to have more than one WNAM subrecord.\n";
             return false;
           }
           //read WNAM
           if (!loadUint32SubRecordFromStream(in_File, cWNAM, parentWorldSpaceFormID, false)) return false;
           bytesRead += 6;
           if (parentWorldSpaceFormID==0)
           {
             std::cout << "Error: subrecord WNAM of WRLD has value zero!\n";
             return false;
           }
           break;
      case cPNAM:
           if (hasPNAM)
           {
             std::cout << "Error: WRLD seems to have more than one PNAM subrecord.\n";
             return false;
           }
           //PNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: sub record PNAM of WRLD has invalid length ("
                       <<subLength<<" bytes). Should be two bytes.\n";
             return false;
           }
           //read PNAM's stuff
           in_File.read((char*) &unknownPNAM, 2);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord PNAM of WRLD!\n";
             return false;
           }
           bytesRead += 2;
           hasPNAM = false;
           break;
      case cONAM:
           if (unknownONAM.isPresent())
           {
             std::cout << "Error: WRLD seems to have more than one ONAM subrecord.\n";
             return false;
           }
           //read ONAM
           if (!unknownONAM.loadFromStream(in_File, cONAM, false))
           {
             std::cout << "Error while reading subrecord ONAM of WRLD!\n";
             return false;
           }
           //ONAM's length
           if (unknownONAM.getSize()!=16)
           {
             std::cout <<"Error: sub record ONAM of WRLD has invalid length ("
                       <<unknownONAM.getSize()<<" bytes). Should be 16 bytes.\n";
             return false;
           }
           bytesRead = bytesRead +2 /*length value*/ +unknownONAM.getSize() /*data size*/;
           break;
      case cNAMA:
           if (hasReadNAMA)
           {
             std::cout << "Error: WRLD seems to have more than one NAMA subrecord.\n";
             return false;
           }
           //NAMA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record NAMA of WRLD has invalid length ("
                       <<subLength<<" bytes). Should be four bytes.\n";
             return false;
           }
           //read NAMA's stuff
           in_File.read((char*) &distantLODMultiplier, 4);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NAMA of WRLD!\n";
             return false;
           }
           bytesRead += 4;
           hasReadNAMA = true;
           break;
      case cDATA:
           if (hasDATA)
           {
             std::cout << "Error: WRLD seems to have more than one DATA subrecord.\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: sub record DATA of WRLD has invalid length ("
                       <<subLength<<" bytes). Should be one byte.\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &unknownDATA, 1);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of WRLD!\n";
             return false;
           }
           bytesRead += 1;
           hasDATA = false;
           break;
      case cNAM0:
           if (hasNAM0)
           {
             std::cout << "Error: WRLD seems to have more than one NAM0 subrecord.\n";
             return false;
           }
           //NAM0's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record NAM0 of WRLD has invalid length ("
                       <<subLength<<" bytes). Should be eight bytes.\n";
             return false;
           }
           //read NAM0's stuff
           in_File.read((char*) &unknownNAM0, 8);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NAM0 of WRLD!\n";
             return false;
           }
           bytesRead += 8;
           hasNAM0 = false;
           break;
      case cNAM9:
           if (hasNAM9)
           {
             std::cout << "Error: WRLD seems to have more than one NAM9 subrecord.\n";
             return false;
           }
           //NAM9's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record NAM9 of WRLD has invalid length ("
                       <<subLength<<" bytes). Should be eight bytes.\n";
             return false;
           }
           //read NAM9's stuff
           in_File.read((char*) &unknownNAM9, 8);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NAM9 of WRLD!\n";
             return false;
           }
           bytesRead += 8;
           hasNAM9 = false;
           break;
      case cZNAM:
           if (musicFormID!=0)
           {
             std::cout << "Error: WRLD seems to have more than one ZNAM subrecord.\n";
             return false;
           }
           //read ZNAM
           if (!loadUint32SubRecordFromStream(in_File, cZNAM, musicFormID, false)) return false;
           bytesRead += 6;
           if (musicFormID==0)
           {
             std::cout << "Error: subrecord ZNAM of WRLD has value zero!\n";
             return false;
           }
           break;
      case cTNAM:
           if (!HD_LOD_DiffuseTexture.empty())
           {
             std::cout << "Error: WRLD seems to have more than one TNAM subrecord.\n";
             return false;
           }
           //read TNAM
           if (!loadString512FromStream(in_File, HD_LOD_DiffuseTexture, buffer, cTNAM, false, bytesRead))
             return false;
           //check content
           if (HD_LOD_DiffuseTexture.empty())
           {
             std::cout << "Error: subrecord TNAM of WRLD is empty!\n";
             return false;
           }
           break;
      case cUNAM:
           if (!HD_LOD_NormalTexture.empty())
           {
             std::cout << "Error: WRLD seems to have more than one UNAM subrecord.\n";
             return false;
           }
           //read UNAM
           if (!loadString512FromStream(in_File, HD_LOD_NormalTexture, buffer, cUNAM, false, bytesRead))
             return false;
           //check content
           if (HD_LOD_NormalTexture.empty())
           {
             std::cout << "Error: subrecord UNAM of WRLD is empty!\n";
             return false;
           }
           break;
      case cXWEM:
           if (!unknownXWEM.empty())
           {
             std::cout << "Error: WRLD seems to have more than one XWEM subrecord.\n";
             return false;
           }
           //read XWEM
           if (!loadString512FromStream(in_File, unknownXWEM, buffer, cXWEM, false, bytesRead))
             return false;
           //check content
           if (unknownXWEM.empty())
           {
             std::cout << "Error: subrecord XWEM of WRLD is empty!\n";
             return false;
           }
           break;
      case cXXXX:
           if (sizeXXXX!=0)
           {
             std::cout << "Error: WRLD seems to have more than one XXXX subrecord in a row.\n";
             return false;
           }
           //read XXXX
           if (!loadUint32SubRecordFromStream(in_File, cXXXX, sizeXXXX, false)) return false;
           bytesRead += 6;
           if (0==sizeXXXX)
           {
             std::cout << "Error: subrecord XXXX of WRLD has value zero!\n";
             return false;
           }
           break;
      case cOFST:
           if (unknownOFST.isPresent())
           {
             std::cout << "Error: WRLD seems to have more than one OFST subrecord.\n";
             return false;
           }
           //read OFST
           if (0==sizeXXXX)
           {
             if (!unknownOFST.loadFromStream(in_File, cOFST, false))
             {
               std::cout << "Error while reading subrecord OFST of WRLD!\n";
               return false;
             }
           }
           else
           {
             if (!unknownOFST.loadFromStreamExtended(in_File, cOFST, false, sizeXXXX))
             {
               std::cout << "Error while reading subrecord OFST of WRLD!\n";
               return false;
             }
             sizeXXXX = 0;
           }//else - load extended version of subrecord

           bytesRead = bytesRead +2 /*length value*/ +unknownOFST.getSize() /*data size*/;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only RNAM, MHDT, FULL, WCTR, LTMP, XEZN,"
                     << " CNAM, NAM2, NAM3, NAM4, DNAM, MODL, MODT, MNAM, XLCN,"
                     << " WNAM, PNAM, ONAM, NAMA, DATA, NAM0, NAM9, ZNAM, TNAM,"
                     << " UNAM, XWEM, XXXX or OFST are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadNAMA and unknownOFST.isPresent()))
  {
    std::cout << "Error: while reading WRLD record: at least one required subrecord is missing!\n";
    std::cout << "NAMA: "<<hasReadNAMA<<"    OFST: "<<unknownOFST.isPresent()<<"\n";
    return false;
  }

  return in_File.good();
}

uint32_t WorldSpaceRecord::getRecordType() const
{
  return cWRLD;
}

} //namespace
