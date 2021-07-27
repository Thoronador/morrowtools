/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "ArmourRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

ArmourRecord::ArmourRecord()
: BasicRecord(), editorID(""),
  unknownVMAD(BinarySubRecord()),
  name(LocalizedString()),
  enchantingFormID(0),
  modelPath(""),
  unknownMO2T(BinarySubRecord()),
  unknownMO2S(BinarySubRecord()),
  mod4Path(""),
  unknownMO4T(BinarySubRecord()),
  unknownMO4S(BinarySubRecord()),
  unknownBODT(BinarySubRecord()),
  unknownBOD2(BinarySubRecord()),
  equipTypeFormID(0),
  blockBashImpactDataSetFormID(0),
  alternateBlockMaterialFormID(0),
  pickupSoundFormID(0),
  putdownSoundFormID(0),
  unknownRNAM(0),
  keywordArray(std::vector<uint32_t>()),
  description(LocalizedString()),
  models(std::vector<uint32_t>()),
  value(0),
  weight(0.0f),
  unknownDNAM(0),
  templateArmorFormID(0)
{
  memset(unknownOBND, 0, 12);
}

ArmourRecord::~ArmourRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ArmourRecord::equals(const ArmourRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (name==other.name)
      and (enchantingFormID==other.enchantingFormID)
      and (modelPath==other.modelPath) and (unknownMO2T==other.unknownMO2T)
      and (unknownMO2S==other.unknownMO2S)
      and (mod4Path==other.mod4Path) and (unknownMO4T==other.unknownMO4T)
      and (unknownMO4S==other.unknownMO4S) and (unknownBODT==other.unknownBODT)
      and (unknownBOD2==other.unknownBOD2)
      and (equipTypeFormID==other.equipTypeFormID)
      and (blockBashImpactDataSetFormID==other.blockBashImpactDataSetFormID)
      and (alternateBlockMaterialFormID==other.alternateBlockMaterialFormID)
      and (pickupSoundFormID==other.pickupSoundFormID)
      and (putdownSoundFormID==other.putdownSoundFormID)
      and (unknownRNAM==other.unknownRNAM) and (keywordArray==other.keywordArray)
      and (description==other.description)
      and (models==other.models)
      and (value==other.value) and (weight==other.weight)
      and (unknownDNAM==other.unknownDNAM)
      and (templateArmorFormID==other.templateArmorFormID));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ArmourRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length */
        +4 /* RNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */
        +4 /* DATA */ +2 /* 2 bytes for length */ +8 /* fixed length */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /* VMAD */ +2 /* 2 bytes for length */ +unknownVMAD.getSize() /* length */;
  }//if VMAD
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }

  if (enchantingFormID!=0)
  {
    writeSize = writeSize +4 /* EITM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has EITM
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MOD2 */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMO2T.isPresent())
  {
    writeSize = writeSize +4 /* MO2T */ +2 /* 2 bytes for length */ +unknownMO2T.getSize() /* length */;
  }//if MO2T
  if (unknownMO2S.isPresent())
  {
    writeSize = writeSize +4 /* MO2S */ +2 /* 2 bytes for length */ +unknownMO2S.getSize() /* length */;
  }//if MO2S
  if (!mod4Path.empty())
  {
    writeSize = writeSize +4 /* MOD4 */ +2 /* 2 bytes for length */
        +mod4Path.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMO4T.isPresent())
  {
    writeSize = writeSize +4 /* MO4T */ +2 /* 2 bytes for length */ +unknownMO4T.getSize() /* length */;
  }//if MO4T
  if (unknownMO4S.isPresent())
  {
    writeSize = writeSize +4 /* MO4S */ +2 /* 2 bytes for length */ +unknownMO4S.getSize() /* length */;
  }//if MO4S
  if (unknownBODT.isPresent())
  {
    writeSize = writeSize +4 /* BODT */ +2 /* 2 bytes for length */ +unknownBODT.getSize() /* length */;
  }//if BODT
  if (unknownBOD2.isPresent())
  {
    writeSize = writeSize +4 /* BOD2 */ +2 /* 2 bytes for length */ +unknownBOD2.getSize() /* length */;
  }//if BOD2
  if (equipTypeFormID!=0)
  {
    writeSize = writeSize +4 /* ETYP */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has ETYP
  if (blockBashImpactDataSetFormID!=0)
  {
    writeSize = writeSize +4 /* BIDS */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has BIDS
  if (alternateBlockMaterialFormID!=0)
  {
    writeSize = writeSize +4 /* BAMT */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has BAMT
  if (pickupSoundFormID!=0)
  {
    writeSize = writeSize +4 /* YNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has YNAM
  if (putdownSoundFormID!=0)
  {
    writeSize = writeSize +4 /* ZNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has ZNAM
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed length */
                          +4 /* KWDA */ +2 /* 2 bytes for length */ +4*keywordArray.size() /*data length*/ ;
  }//if keywords
  if (description.isPresent())
  {
    writeSize += description.getWriteSize();
  }//if has DESC
  if (!models.empty())
  {
    writeSize = writeSize +models.size()*(4 /* MODL */ +2 /* 2 bytes for length */ +4 /* fixed length */);
  }//if keywords
  if (templateArmorFormID!=0)
  {
    writeSize = writeSize +4 /* TNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has TNAM
  return writeSize;
}

bool ArmourRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cARMO, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (unknownVMAD.isPresent())
  {
    //write VMAD
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cout << "Error while writing VMAD of ARMO!";
      return false;
    }
  }//if VMAD

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write OBND
  output.write((const char*) unknownOBND, 12);

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }//if has FULL

  if (enchantingFormID!=0)
  {
    //write EITM
    output.write((const char*) &cEITM, 4);
    //EITM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write Enchanting form ID
    output.write((const char*) &enchantingFormID, 4);
  }//if has EITM

  if (!modelPath.empty())
  {
    //write MOD2
    output.write((const char*) &cMOD2, 4);
    //MOD2's length
    subLength = modelPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path 2
    output.write(modelPath.c_str(), subLength);
  }//if model path not empty

  if (unknownMO2T.isPresent())
  {
    //write MO2T
    if (!unknownMO2T.saveToStream(output, cMO2T))
    {
      std::cout << "Error while writing MO2T of ARMO!";
      return false;
    }
  }//if MO2T

  if (unknownMO2S.isPresent())
  {
    //write MO2S
    if (!unknownMO2S.saveToStream(output, cMO2S))
    {
      std::cout << "Error while writing MO2S of ARMO!";
      return false;
    }
  }//if MO2S

  if (!mod4Path.empty())
  {
    //write MOD4
    output.write((const char*) &cMOD4, 4);
    //MOD4's length
    subLength = mod4Path.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path 4
    output.write(mod4Path.c_str(), subLength);
  }//if mod4

  if (unknownMO4T.isPresent())
  {
    //write MO4T
    if (!unknownMO4T.saveToStream(output, cMO4T))
    {
      std::cout << "Error while writing MO4T of ARMO!";
      return false;
    }
  }//if MO4T

  if (unknownMO4S.isPresent())
  {
    //write MO4S
    if (!unknownMO4S.saveToStream(output, cMO4S))
    {
      std::cout << "Error while writing MO4S of ARMO!";
      return false;
    }
  }//if MO4S

  if (unknownBODT.isPresent())
  {
    //write BODT
    if (!unknownBODT.saveToStream(output, cBODT))
    {
      std::cout << "Error while writing BODT of ARMO!";
      return false;
    }
  }//if BODT

  if (unknownBOD2.isPresent())
  {
    //write BOD2
    if (!unknownBOD2.saveToStream(output, cBOD2))
    {
      std::cout << "Error while writing BOD2 of ARMO!";
      return false;
    }
  }//if BOD2

  if (equipTypeFormID!=0)
  {
    //write ETYP
    output.write((const char*) &cETYP, 4);
    //ETYP's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write Equip Type's form ID
    output.write((const char*) &equipTypeFormID, 4);
  }//if has ETYP

  if (blockBashImpactDataSetFormID!=0)
  {
    //write BIDS
    output.write((const char*) &cBIDS, 4);
    //BIDS's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write Block Bash Impact Data Set's form ID
    output.write((const char*) &blockBashImpactDataSetFormID, 4);
  }//if has BIDS

  if (alternateBlockMaterialFormID!=0)
  {
    //write BAMT
    output.write((const char*) &cBAMT, 4);
    //BAMT's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write Alternate Block Material's form ID
    output.write((const char*) &alternateBlockMaterialFormID, 4);
  }//if has BAMT

  if (pickupSoundFormID!=0)
  {
    //write YNAM
    output.write((const char*) &cYNAM, 4);
    //YNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write Pickup Sound form ID
    output.write((const char*) &pickupSoundFormID, 4);
  }//if has YNAM

  if (putdownSoundFormID!=0)
  {
    //write ZNAM
    output.write((const char*) &cZNAM, 4);
    //ZNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write Putdown Sound Form ID
    output.write((const char*) &putdownSoundFormID, 4);
  }//if has ZNAM

  //write RNAM
  output.write((const char*) &cRNAM, 4);
  //RNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write RNAM's data
  output.write((const char*) &unknownRNAM, 4);

  uint32_t i;
  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write KSIZ's data
    const uint32_t k_Size = keywordArray.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*k_Size; //fixed
    output.write((const char*) &subLength, 2);
    //write KWDA's data
    for (i=0; i<k_Size; ++i)
    {
      output.write((const char*) &(keywordArray.at(i)), 4);
    }//for
  }//if keyword array

  if (description.isPresent())
  {
    //write DESC
    if (!description.saveToStream(output, cDESC))
      return false;
  }//if has DESC

  for (i=0; i<models.size(); ++i)
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //MODL's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write MODL's data
    output.write((const char*) &(models[i]), 4);
  }//for MODL

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 8; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) &value, 4);
  output.write((const char*) &weight, 4);

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write DNAM's data
  output.write((const char*) &unknownDNAM, 4);

  if (templateArmorFormID!=0)
  {
    //write TNAM
    output.write((const char*) &cTNAM, 4);
    //TNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write Template Armor's form ID
    output.write((const char*) &templateArmorFormID, 4);
  }//if has TNAM

  return output.good();
}
#endif

bool ArmourRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cout <<"Error: sub record EDID of ARMO is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of ARMO!\n";
    return false;
  }
  editorID = std::string(buffer);

  unknownVMAD.setPresence(false);
  bool hasReadOBND = false;
  name.reset();
  enchantingFormID = 0;
  modelPath.clear();
  unknownMO2T.setPresence(false);
  unknownMO2S.setPresence(false);
  mod4Path.clear();
  unknownMO4T.setPresence(false);
  unknownBODT.setPresence(false);
  unknownBOD2.setPresence(false);
  equipTypeFormID = 0;
  blockBashImpactDataSetFormID = 0;
  alternateBlockMaterialFormID = 0;
  pickupSoundFormID = 0;
  putdownSoundFormID = 0;
  bool hasReadRNAM = false;
  keywordArray.clear();
  uint32_t k_Size, i, temp;
  description.reset();
  models.clear();
  bool hasReadDATA = false;
  bool hasReadDNAM = false;
  templateArmorFormID = 0;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cout << "Error: ARMO seems to have more than one VMAD subrecord!\n";
             return false;
           }
           //read VMAD
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cout << "Error while reading subrecord VMAD of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownVMAD.getSize();
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cout << "Error: ARMO seems to have more than one OBND subrecord!\n";
             return false;
           }
           //OBND's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record OBND of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read OBND's stuff
           in_File.read((char*) unknownOBND, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord OBND of ARMO!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cout << "Error: record ARMO seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cEITM:
           if (enchantingFormID!=0)
           {
             std::cout << "Error: record ARMO seems to have more than one EITM subrecord!\n";
             return false;
           }
           //read EITM
           if (!loadUint32SubRecordFromStream(in_File, cEITM, enchantingFormID, false)) return false;
           bytesRead += 6;
           if (enchantingFormID==0)
           {
             std::cout <<"Error: sub record EITM of ARMO is zero!\n";
             return false;
           }
           break;
      case cMOD2:
           if (!modelPath.empty())
           {
             std::cout << "Error: record ARMO seems to have more than one MOD2 subrecord!\n";
             return false;
           }
           //MOD2's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MOD2 of ARMO is longer than 511 characters!\n";
             return false;
           }
           //read MOD2's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MOD2 of ARMO!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMO2T:
           if (unknownMO2T.isPresent())
           {
             std::cout << "Error: ARMO seems to have more than one MO2T subrecord!\n";
             return false;
           }
           //read MO2T
           if (!unknownMO2T.loadFromStream(in_File, cMO2T, false))
           {
             std::cout << "Error while reading subrecord MO2T of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMO2T.getSize();
           break;
      case cMO2S:
           if (unknownMO2S.isPresent())
           {
             std::cout << "Error: ARMO seems to have more than one MO2S subrecord!\n";
             return false;
           }
           //read MO2S
           if (!unknownMO2S.loadFromStream(in_File, cMO2S, false))
           {
             std::cout << "Error while reading subrecord MO2S of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMO2S.getSize();
           break;
      case cMOD4:
           if (!mod4Path.empty())
           {
             std::cout << "Error: record ARMO seems to have more than one MOD4 subrecord!\n";
             return false;
           }
           //MOD4's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MOD4 of ARMO is longer than 511 characters!\n";
             return false;
           }
           //read MOD4's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MOD4 of ARMO!\n";
             return false;
           }
           mod4Path = std::string(buffer);
           break;
      case cMO4T:
           if (unknownMO4T.isPresent())
           {
             std::cout << "Error: ARMO seems to have more than one MO4T subrecord!\n";
             return false;
           }
           //read MO4T
           if (!unknownMO4T.loadFromStream(in_File, cMO4T, false))
           {
             std::cout << "Error while reading subrecord MO4T of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMO4T.getSize();
           break;
      case cMO4S:
           if (unknownMO4S.isPresent())
           {
             std::cout << "Error: ARMO seems to have more than one MO4S subrecord!\n";
             return false;
           }
           //read MO4S
           if (!unknownMO4S.loadFromStream(in_File, cMO4S, false))
           {
             std::cout << "Error while reading subrecord MO4S of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMO4S.getSize();
           break;
      case cBODT:
           if (unknownBODT.isPresent())
           {
             std::cout << "Error: record ARMO seems to have more than one BODT subrecord!\n";
             return false;
           }
           //read BODT
           if (!unknownBODT.loadFromStream(in_File, cBODT, false))
           {
             std::cout << "Error while reading subrecord BODT of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownBODT.getSize();
           //check BODT's length
           if ((unknownBODT.getSize()!=12) and (unknownBODT.getSize()!=8))
           {
             std::cout <<"Error: sub record BODT of ARMO has invalid length ("
                       <<unknownBODT.getSize()<<" bytes). Should be 8 or 12 bytes!\n";
             return false;
           }
           break;
      case cBOD2:
           if (unknownBOD2.isPresent())
           {
             std::cout << "Error: ARMO seems to have more than one BOD2 subrecord!\n";
             return false;
           }
           //read BOD2
           if (!unknownBOD2.loadFromStream(in_File, cBOD2, false))
             return false;
           bytesRead += (2+unknownBOD2.getSize());
           break;
      case cETYP:
           if (equipTypeFormID!=0)
           {
             std::cout << "Error: record ARMO seems to have more than one ETYP subrecord!\n";
             return false;
           }
           //read ETYP
           if (!loadUint32SubRecordFromStream(in_File, cETYP, equipTypeFormID, false)) return false;
           bytesRead += 6;
           if (equipTypeFormID==0)
           {
             std::cout <<"Error: sub record ETYP of ARMO is zero!\n";
             return false;
           }
           break;
      case cBIDS:
           if (blockBashImpactDataSetFormID!=0)
           {
             std::cout << "Error: record ARMO seems to have more than one BIDS subrecord!\n";
             return false;
           }
           //read BIDS
           if (!loadUint32SubRecordFromStream(in_File, cBIDS, blockBashImpactDataSetFormID, false)) return false;
           bytesRead += 6;
           if (blockBashImpactDataSetFormID==0)
           {
             std::cout <<"Error: sub record BIDS of ARMO is zero!\n";
             return false;
           }
           break;
      case cBAMT:
           if (alternateBlockMaterialFormID!=0)
           {
             std::cout << "Error: record ARMO seems to have more than one BAMT subrecord!\n";
             return false;
           }
           //read BAMT
           if (!loadUint32SubRecordFromStream(in_File, cBAMT, alternateBlockMaterialFormID, false)) return false;
           bytesRead += 6;
           if (alternateBlockMaterialFormID==0)
           {
             std::cout <<"Error: sub record BAMT of ARMO is zero!\n";
             return false;
           }
           break;
      case cYNAM:
           if (pickupSoundFormID!=0)
           {
             std::cout << "Error: ARMO seems to have more than one YNAM subrecord!\n";
             return false;
           }
           //read YNAM
           if (!loadUint32SubRecordFromStream(in_File, cYNAM, pickupSoundFormID, false)) return false;
           bytesRead += 6;
           if (pickupSoundFormID==0)
           {
             std::cout <<"Error: subrecord YNAM of ARMO is zero!\n";
             return false;
           }
           break;
      case cZNAM:
           if (putdownSoundFormID!=0)
           {
             std::cout << "Error: ARMO seems to have more than one ZNAM subrecord!\n";
             return false;
           }
           //read ZNAM
           if (!loadUint32SubRecordFromStream(in_File, cZNAM, putdownSoundFormID, false)) return false;
           bytesRead += 6;
           if (putdownSoundFormID==0)
           {
             std::cout << "Error: subrecord ZNAM of ARMO is zero!\n";
             return false;
           }
           break;
      case cRNAM:
           if (hasReadRNAM)
           {
             std::cout << "Error: ARMO seems to have more than one RNAM subrecord!\n";
             return false;
           }
           //RNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord RNAM of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read RNAM's stuff
           in_File.read((char*) &unknownRNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord RNAM of ARMO!\n";
             return false;
           }//if
           hasReadRNAM = true;
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: recurd ARMO seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //KSIZ's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord KSIZ of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read KSIZ's stuff
           k_Size = 0;
           in_File.read((char*) &k_Size, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord KSIZ of ARMO!\n";
             return false;
           }//if

           //read KWDA
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cKWDA)
           {
             UnexpectedRecord(cKWDA, subRecName);
             return false;
           }
           //KWDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4*k_Size)
           {
             std::cout <<"Error: sub record KWDA of ARMO has invalid length ("
                       <<subLength<<" bytes. Should be "<<4*k_Size<<" bytes!\n";
             return false;
           }
           //read KWDA's stuff
           for (i=0; i<k_Size; ++i)
           {
             in_File.read((char*) &temp, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord KWDA of ARMO!\n";
               return false;
             }//if
             keywordArray.push_back(temp);
           }//for
           break;
      case cDESC:
           if (description.isPresent())
           {
             std::cout << "Error: ARMO seems to have more than one DESC subrecord!\n";
             return false;
           }
           //read DESC
           if (!description.loadFromStream(in_File, cDESC, false, bytesRead, localized, table, buffer))
           {
             std::cout << "Error while reading subrecord DESC of ARMO!\n";
             return false;
           }//if
           break;
      case cMODL:
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord MODL of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read MODL's stuff
           temp = 0;
           in_File.read((char*) &temp, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of ARMO!\n";
             return false;
           }//if
           models.push_back(temp);
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: record ARMO seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record DATA of ARMO has invalid length ("
                       <<subLength<<" bytes. Should be 8 bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &value, 4);
           in_File.read((char*) &weight, 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of ARMO!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cout << "Error: ARMO seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord DNAM of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read DNAM's stuff
           in_File.read((char*) &unknownDNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of ARMO!\n";
             return false;
           }//if
           hasReadDNAM = true;
           break;
      case cTNAM:
           if (templateArmorFormID!=0)
           {
             std::cout << "Error: ARMO seems to have more than one TNAM subrecord!\n";
             return false;
           }
           //read TNAM
           if (!loadUint32SubRecordFromStream(in_File, cTNAM, templateArmorFormID, false)) return false;
           bytesRead += 6;
           if (templateArmorFormID==4)
           {
             std::cout <<"Error: subrecord TNAM of ARMO is zero!\n";
             return false;
           }
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only VMAD, OBND, FULL, EITM, MOD2, MO2T,"
                     << " MOD4, MO4T, BODT, BOD2, ETYP, BIDS, BAMT, YNAM, ZNAM,"
                     << " RNAM, KSIZ, DESC, MODL, DATA, DNAM, or TNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadOBND and (unknownBODT.isPresent() or unknownBOD2.isPresent()) and hasReadRNAM and hasReadDATA and hasReadDNAM))
  {
    std::cout << "Error: subrecord OBND, BODT/BOD2, DATA, DNAM or RNAM of ARMO is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t ArmourRecord::getRecordType() const
{
  return cARMO;
}

} //namespace
