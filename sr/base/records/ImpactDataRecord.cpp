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

#include "ImpactDataRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

/* SubrecordDODT's functions */

bool ImpactDataRecord::SubrecordDODT::operator==(const ImpactDataRecord::SubrecordDODT& other) const
{
  if (other.m_Presence!=m_Presence) return false;
  if (!m_Presence) return true;
  return ((minWidth==other.minWidth) and (maxWidth==other.maxWidth)
      and (minHeight==other.minHeight) and (maxHeight==other.maxHeight)
      and (depth==other.depth) and (shininess==other.shininess)
      and (parallaxScale==other.parallaxScale)
      and (parallaxPasses==other.parallaxPasses)
      and (colorRed==other.colorRed) and (colorGreen==other.colorGreen)
      and (colorBlue==other.colorBlue) and (zeroes==other.zeroes));
}

void ImpactDataRecord::SubrecordDODT::setPresence(const bool newPresence)
{
  m_Presence = newPresence;
}

/* Sound Level constants */
const uint32_t ImpactDataRecord::cSoundLevelLoud     = 0x00000000;
const uint32_t ImpactDataRecord::cSoundLevelNormal   = 0x00000001;
const uint32_t ImpactDataRecord::cSoundLevelSilent   = 0x00000002;
const uint32_t ImpactDataRecord::cSoundLevelVeryLoud = 0x00000003;

/* ImpactDataRecord's functions */

ImpactDataRecord::ImpactDataRecord()
: BasicRecord(), editorID(""),
  modelPath(""),
  //subrecord DATA
  effectDuration(0.0f),
  unknownDATA02(0),
  angleTreshold(0.0f),
  placementRadius(0.0f),
  soundLevel(cSoundLevelNormal),
  unknownDATA06(0),
  textureSetFormID(0),
  secondaryTextureSetFormID(0),
  impactSoundOneFormID(0),
  impactSoundTwoFormID(0),
  hazardFormID(0)
  //end of DATA
{
  unknownMODT.setPresence(false);
  decalData.setPresence(false);
}

ImpactDataRecord::~ImpactDataRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ImpactDataRecord::equals(const ImpactDataRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      //DATA
      and (effectDuration==other.effectDuration) and (unknownDATA02==other.unknownDATA02)
      and (angleTreshold==other.angleTreshold) and (placementRadius==other.placementRadius)
      and (soundLevel==other.soundLevel) and (unknownDATA06==other.unknownDATA06)
      //end of DATA
      and (decalData==other.decalData)
      and (textureSetFormID==other.textureSetFormID)
      and (secondaryTextureSetFormID==other.secondaryTextureSetFormID)
      and (impactSoundOneFormID==other.impactSoundOneFormID)
      and (impactSoundTwoFormID==other.impactSoundTwoFormID)
      and (hazardFormID==other.hazardFormID)
  );
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ImpactDataRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +24 /* fixed size of 24 bytes */;
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
               +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */
               +unknownMODT.getSize() /* length of subrecord */;
  }
  if (decalData.isPresent())
  {
    writeSize = writeSize +4 /* DODT */ +2 /* 2 bytes for length */ +36 /* length of subrecord */;
  }
  if (textureSetFormID!=0)
  {
    writeSize = writeSize +4 /* DNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes */;
  }
  if (secondaryTextureSetFormID!=0)
  {
    writeSize = writeSize +4 /* ENAM */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes */;
  }
  if (impactSoundOneFormID!=0)
  {
    writeSize = writeSize +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes */;
  }
  if (impactSoundTwoFormID!=0)
  {
    writeSize = writeSize +4 /* NAM1 */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes */;
  }
  if (hazardFormID!=0)
  {
    writeSize = writeSize +4 /* NAM2 */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes */;
  }
  return writeSize;
}

bool ImpactDataRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cIPCT, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (!modelPath.empty())
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //model path's length
    subLength = modelPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path
    output.write(modelPath.c_str(), subLength);
  }//if MODL

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing MODT of IPCT!";
      return false;
    }
  }//if MODT

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 24; //fixed length of
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) &effectDuration, 4);
  output.write((const char*) &unknownDATA02, 4);
  output.write((const char*) &angleTreshold, 4);
  output.write((const char*) &placementRadius, 4);
  output.write((const char*) &soundLevel, 4);
  output.write((const char*) &unknownDATA06, 4);

  if (decalData.isPresent())
  {
    //write DODT
    output.write((const char*) &cDODT, 4);
    //DODT's length
    subLength = 36; //fixed size
    output.write((const char*) &subLength, 2);
    //write DODT data
    output.write((const char*) &decalData.minWidth, 4);
    output.write((const char*) &decalData.maxWidth, 4);
    output.write((const char*) &decalData.minHeight, 4);
    output.write((const char*) &decalData.maxHeight, 4);
    output.write((const char*) &decalData.depth, 4);
    output.write((const char*) &decalData.shininess, 4);
    output.write((const char*) &decalData.parallaxScale, 4);
    output.write((const char*) &decalData.parallaxPasses, 4);
    output.write((const char*) &decalData.colorRed, 1);
    output.write((const char*) &decalData.colorGreen, 1);
    output.write((const char*) &decalData.colorBlue, 1);
    output.write((const char*) &decalData.zeroes, 1);
  }//if DODT

  if (textureSetFormID!=0)
  {
    //write DNAM
    output.write((const char*) &cDNAM, 4);
    //DNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write DNAM
    output.write((const char*) &textureSetFormID, 4);
  }//if DNAM

  if (secondaryTextureSetFormID!=0)
  {
    //write ENAM
    output.write((const char*) &cENAM, 4);
    //ENAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write ENAM
    output.write((const char*) &secondaryTextureSetFormID, 4);
  }//if ENAM

  if (impactSoundOneFormID!=0)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write SNAM
    output.write((const char*) &impactSoundOneFormID, 4);
  }//if SNAM

  if (impactSoundTwoFormID!=0)
  {
    //write NAM1
    output.write((const char*) &cNAM1, 4);
    //NAM1's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write NAM1
    output.write((const char*) &impactSoundTwoFormID, 4);
  }//if NAM1

  if (hazardFormID!=0)
  {
    //write NAM2
    output.write((const char*) &cNAM2, 4);
    //NAM2's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write NAM2
    output.write((const char*) &hazardFormID, 4);
  }//if NAM2

  return output.good();
}
#endif

bool ImpactDataRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead;

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
    std::cout <<"Error: sub record EDID of IPCT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of IPCT!\n";
    return false;
  }
  editorID = std::string(buffer);

  modelPath.clear();
  unknownMODT.setPresence(false);
  bool hasReadDATA = false;
  decalData.setPresence(false);
  textureSetFormID = 0;
  secondaryTextureSetFormID = 0;
  impactSoundOneFormID = 0;
  impactSoundTwoFormID = 0;
  hazardFormID = 0;

  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: IPCT seems to have two MODL subrecords!\n";
             return false;
           }
           //load MODL
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           //check content
           if (modelPath.empty())
           {
             std::cout << "Error: subrecord MODL of IPCT is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: IPCT seems to have two MODT subrecords!\n";
             return false;
           }
           //load MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while loading subrecord MODT of IPCT!\n";
             return false;
           }
           bytesRead += (2+unknownMODT.getSize());
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: IPCT seems to have two DATA subrecords!\n";
             return false;
           }
           //read DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=24)
           {
             std::cout << "Error: subrecord DATA of IPCT has invalid length ("
                       << subLength<<" bytes). Should be 24 bytes!\n";
             return false;
           }
           //load DATA
           in_File.read((char*) &effectDuration, 4);
           in_File.read((char*) &unknownDATA02, 4);
           in_File.read((char*) &angleTreshold, 4);
           in_File.read((char*) &placementRadius, 4);
           in_File.read((char*) &soundLevel, 4);
           in_File.read((char*) &unknownDATA06, 4);
           bytesRead += 24;
           if (!in_File.good())
           {
             std::cout << "Error while loading subrecord DATA of IPCT!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cDODT:
           if (decalData.isPresent())
           {
             std::cout << "Error: IPCT seems to have two DODT subrecords!\n";
             return false;
           }
           //read DODT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=36)
           {
             std::cout << "Error: subrecord DODT of IPCT has invalid length ("
                       << subLength<<" bytes). Should be 36 bytes!\n";
             return false;
           }
           //load DODT
           in_File.read((char*) &decalData.minWidth, 4);
           in_File.read((char*) &decalData.maxWidth, 4);
           in_File.read((char*) &decalData.minHeight, 4);
           in_File.read((char*) &decalData.maxHeight, 4);
           in_File.read((char*) &decalData.depth, 4);
           in_File.read((char*) &decalData.shininess, 4);
           in_File.read((char*) &decalData.parallaxScale, 4);
           in_File.read((char*) &decalData.parallaxPasses, 4);
           in_File.read((char*) &decalData.colorRed, 1);
           in_File.read((char*) &decalData.colorGreen, 1);
           in_File.read((char*) &decalData.colorBlue, 1);
           in_File.read((char*) &decalData.zeroes, 1);
           bytesRead += 36;
           if (!in_File.good())
           {
             std::cout << "Error while loading subrecord DODT of IPCT!\n";
             return false;
           }
           break;
      case cDNAM:
           if (textureSetFormID!=0)
           {
             std::cout << "Error: IPCT seems to have two DNAM subrecords!\n";
             return false;
           }
           //load DNAM
           if (!loadUint32SubRecordFromStream(in_File, cDNAM, textureSetFormID, false)) return false;
           bytesRead += 6;
           //check value
           if (textureSetFormID==0)
           {
             std::cout << "Error: subrecord DNAM of IPCT is zero!\n";
             return false;
           }
           break;
      case cENAM:
           if (secondaryTextureSetFormID!=0)
           {
             std::cout << "Error: IPCT seems to have two ENAM subrecords!\n";
             return false;
           }
           //load ENAM
           if (!loadUint32SubRecordFromStream(in_File, cENAM, secondaryTextureSetFormID, false)) return false;
           bytesRead += 6;
           //check value
           if (secondaryTextureSetFormID==0)
           {
             std::cout << "Error: subrecord ENAM of IPCT is zero!\n";
             return false;
           }
           break;
      case cSNAM:
           if (impactSoundOneFormID!=0)
           {
             std::cout << "Error: IPCT seems to have two SNAM subrecords!\n";
             return false;
           }
           //load SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, impactSoundOneFormID, false)) return false;
           bytesRead += 6;
           //check value
           if (impactSoundOneFormID==0)
           {
             std::cout << "Error: subrecord SNAM of IPCT is zero!\n";
             return false;
           }
           break;
      case cNAM1:
           if (impactSoundTwoFormID!=0)
           {
             std::cout << "Error: IPCT seems to have two NAM1 subrecords!\n";
             return false;
           }
           //load NAM1
           if (!loadUint32SubRecordFromStream(in_File, cNAM1, impactSoundTwoFormID, false)) return false;
           bytesRead += 6;
           //check value
           if (impactSoundTwoFormID==0)
           {
             std::cout << "Error: subrecord NAM1 of IPCT is zero!\n";
             return false;
           }
           break;
      case cNAM2:
           if (hazardFormID!=0)
           {
             std::cout << "Error: IPCT seems to have two NAM2 subrecords!\n";
             return false;
           }
           //load NAM1
           if (!loadUint32SubRecordFromStream(in_File, cNAM2, hazardFormID, false)) return false;
           bytesRead += 6;
           //check value
           if (hazardFormID==0)
           {
             std::cout << "Error: subrecord NAM2 of IPCT is zero!\n";
             return false;
           }
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only MODL, MODT, DATA, DODT, DNAM, ENAM, SNAM, NAM1 or NAM2 are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!hasReadDATA)
  {
    std::cout << "Error: at least one required subrecord of IPCT is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t ImpactDataRecord::getRecordType() const
{
  return cIPCT;
}

}//namespace
