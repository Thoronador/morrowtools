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

#include "WeaponRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"
#include <cstring>

namespace SRTP
{

//type constants
const unsigned int WeaponRecord::cTypeSwordOneHand = 1;
const unsigned int WeaponRecord::cTypeDagger = 2;
const unsigned int WeaponRecord::cTypeAxeOneHand = 3;
const unsigned int WeaponRecord::cTypeMaceOneHand = 4;
const unsigned int WeaponRecord::cTypeSwordTwoHand = 5;
const unsigned int WeaponRecord::cTypeBluntTwoHand = 6; //axes, warhammers
const unsigned int WeaponRecord::cTypeBow = 7;
const unsigned int WeaponRecord::cTypeStaves = 8;

WeaponRecord::WeaponRecord()
: BasicRecord(), editorID(""),
  hasFULL(false), nameStringID(0),
  modelPath(""),
  enchantingFormID(0),
  hasEAMT(false),
  enchantmentAmount(0),
  equipTypeFormID(0),
  blockBashImpactDataSetFormID(0),
  alternateBlockMaterialFormID(0),
  descriptionStringID(0),
  unknownNNAM(""),
  impactDataSetFormID(0),
  firstPersonModelObjectFormID(0),
  attackFailSoundFormID(0),
  idleSoundFormID(0),
  equipSoundFormID(0),
  unequipSoundFormID(0),
  attackSoundFormID(0),
  //DATA
  value(0),
  weight(0.0f),
  baseDamage(0),
  //end of DATA
  unknownVNAM(0),
  hasCNAM(false), unknownCNAM(0)
{
  unknownVMAD.setPresence(false);
  memset(unknownOBND, 0, 12);
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  keywordArray.clear();
  memset(unknownDNAM, 0, 100);
  memset(unknownCRDT, 0, 16);
}

WeaponRecord::WeaponRecord(const WeaponRecord& other)
: editorID(other.editorID),
  unknownVMAD(other.unknownVMAD),
  hasFULL(other.hasFULL), nameStringID(other.nameStringID),
  modelPath(other.modelPath),
  unknownMODT(other.unknownMODT),
  unknownMODS(other.unknownMODS),
  enchantingFormID(other.enchantingFormID),
  hasEAMT(other.hasEAMT), enchantmentAmount(other.enchantmentAmount),
  equipTypeFormID(other.equipTypeFormID),
  blockBashImpactDataSetFormID(other.blockBashImpactDataSetFormID),
  alternateBlockMaterialFormID(other.alternateBlockMaterialFormID),
  keywordArray(other.keywordArray),
  descriptionStringID(other.descriptionStringID),
  unknownNNAM(other.unknownNNAM),
  impactDataSetFormID(other.impactDataSetFormID),
  firstPersonModelObjectFormID(other.firstPersonModelObjectFormID),
  attackFailSoundFormID(other.attackFailSoundFormID),
  idleSoundFormID(other.idleSoundFormID),
  equipSoundFormID(other.equipSoundFormID),
  unequipSoundFormID(other.unequipSoundFormID),
  attackSoundFormID(other.attackSoundFormID),
  value(other.value),
  weight(other.weight),
  baseDamage(other.baseDamage),
  unknownVNAM(other.unknownVNAM),
  hasCNAM(other.hasCNAM), unknownCNAM(other.unknownCNAM)
{
  copyBasicMembers(other);
  memcpy(unknownOBND, other.unknownOBND, 12);
  memcpy(unknownDNAM, other.unknownDNAM, 100);
  memcpy(unknownCRDT, other.unknownCRDT, 16);
}

WeaponRecord& WeaponRecord::operator=(const WeaponRecord& other)
{
  if (this == &other) return *this;

  copyBasicMembers(other);
  editorID = other.editorID;
  unknownVMAD = other.unknownVMAD;
  memcpy(unknownOBND, other.unknownOBND, 12);
  hasFULL = other.hasFULL;
  nameStringID = other.nameStringID;
  modelPath = other.modelPath;
  unknownMODT = other.unknownMODT;
  unknownMODS = other.unknownMODS;
  enchantingFormID = other.enchantingFormID;
  hasEAMT = other.hasEAMT;
  enchantmentAmount = other.enchantmentAmount;
  equipTypeFormID = other.equipTypeFormID;
  blockBashImpactDataSetFormID = other.blockBashImpactDataSetFormID;
  alternateBlockMaterialFormID = other.alternateBlockMaterialFormID;
  keywordArray = other.keywordArray;
  descriptionStringID = other.descriptionStringID;
  unknownNNAM = other.unknownNNAM;
  impactDataSetFormID = other.impactDataSetFormID;
  firstPersonModelObjectFormID = other.firstPersonModelObjectFormID;
  attackFailSoundFormID = other.attackFailSoundFormID;
  idleSoundFormID = other.idleSoundFormID;
  equipSoundFormID = other.equipSoundFormID;
  unequipSoundFormID = other.unequipSoundFormID;
  attackSoundFormID = other.attackSoundFormID;
  value = other.value;
  weight = other.weight;
  baseDamage = other.baseDamage;
  memcpy(unknownDNAM, other.unknownDNAM, 100);
  memcpy(unknownCRDT, other.unknownCRDT, 16);
  unknownVNAM = other.unknownVNAM;
  hasCNAM = other.hasCNAM;
  unknownCNAM = other.unknownCNAM;
  return *this;
}

WeaponRecord::~WeaponRecord()
{
  //empty
}

uint32_t WeaponRecord::getRecordType() const
{
  return cWEAP;
}

#ifndef SR_NO_RECORD_EQUALITY
bool WeaponRecord::equals(const WeaponRecord& other) const
{
  if ((editorID!=other.editorID) or (unknownVMAD!=other.unknownVMAD)
      or (memcmp(unknownOBND, other.unknownOBND, 12)!=0)
      or (hasFULL!=other.hasFULL) or ((nameStringID!=other.nameStringID) and hasFULL)
      or (modelPath!=other.modelPath) or (unknownMODT!=other.unknownMODT)
      or (unknownMODS!=other.unknownMODS)or (!equalsBasic(other)))
  {
    return false;
  }
  if ((enchantingFormID!=other.enchantingFormID)
    or (hasEAMT!=other.hasEAMT) or ((enchantmentAmount!=other.enchantmentAmount) and hasEAMT)
    or (equipTypeFormID!=other.equipTypeFormID)
    or (blockBashImpactDataSetFormID!=other.blockBashImpactDataSetFormID)
    or (alternateBlockMaterialFormID!=other.alternateBlockMaterialFormID))
  {
    return false;
  }
  if ((keywordArray!=other.keywordArray) or (descriptionStringID!=other.descriptionStringID)
    or (unknownNNAM!=other.unknownNNAM)
    or (impactDataSetFormID!=other.impactDataSetFormID)
    or (firstPersonModelObjectFormID!=other.firstPersonModelObjectFormID)
    or (attackFailSoundFormID!=other.attackFailSoundFormID)
    or (idleSoundFormID!=other.idleSoundFormID)
    or (equipSoundFormID!=other.equipSoundFormID)
    or (unequipSoundFormID!=other.unequipSoundFormID)
    or (hasCNAM!=other.hasCNAM) or ((unknownCNAM!=other.unknownCNAM) and hasCNAM)
    or (attackSoundFormID!=other.attackSoundFormID))
  {
    return false;
  }
  return ((value==other.value) and (weight==other.weight)
      and (baseDamage==other.baseDamage)
      and (memcmp(unknownDNAM, other.unknownDNAM, 100)==0)
      and (memcmp(unknownCRDT, other.unknownCRDT, 16)==0)
      and (unknownVNAM==other.unknownVNAM));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t WeaponRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* DESC */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* DATA */ +2 /* 2 bytes for length */ +10 /* fixed size */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +100 /* fixed size */
        +4 /* CRDT */ +2 /* 2 bytes for length */ +16 /* fixed size */
        +4 /* VNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /* VMAD */ +2 /* 2 bytes for length */
               +unknownVMAD.getSize() /* subrecord size */;
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
               +modelPath.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */
               +unknownMODT.getSize() /* subrecord size */;
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize +4 /* MODS */ +2 /* 2 bytes for length */
               +unknownMODS.getSize() /* subrecord size */;
  }
  if (enchantingFormID!=0)
  {
    writeSize = writeSize +4 /* EITM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasEAMT)
  {
    writeSize = writeSize +4 /* EAMT */ +2 /* 2 bytes for length */ +2 /* fixed size */;
  }
  if (equipTypeFormID!=0)
  {
    writeSize = writeSize +4 /* ETYP */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (blockBashImpactDataSetFormID!=0)
  {
    writeSize = writeSize +4 /* BIDS */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (alternateBlockMaterialFormID!=0)
  {
    writeSize = writeSize +4 /* BAMT */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +4 /* KWDA */ +2 /* 2 bytes for length */ +keywordArray.size()*4 /* n*fixed size */;
  }
  if (!unknownNNAM.empty())
  {
    writeSize = writeSize +4 /* NNAM */ +2 /* 2 bytes for length */
               +unknownNNAM.length()+1 /* length of string +1 byte for NUL-terminaton */;
  }
  if (impactDataSetFormID!=0)
  {
    writeSize = writeSize +4 /* INAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (firstPersonModelObjectFormID!=0)
  {
    writeSize = writeSize +4 /* WNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (attackFailSoundFormID!=0)
  {
    writeSize = writeSize +4 /* TNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (idleSoundFormID!=0)
  {
    writeSize = writeSize +4 /* UNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (equipSoundFormID!=0)
  {
    writeSize = writeSize +4 /* NAM9 */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (unequipSoundFormID!=0)
  {
    writeSize = writeSize +4 /* NAM8 */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (attackSoundFormID!=0)
  {
    writeSize = writeSize +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasCNAM)
  {
    writeSize = writeSize +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }//if CNAM is present
  return writeSize;
}

bool WeaponRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cWEAP, 4);
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
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cout << "Error while writing subrecord VMAD of WEAP!\n";
      return false;
    }
  }//if VMAD

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12;
  output.write((const char*) &subLength, 2);
  //write OBND
  output.write((const char*) unknownOBND, 12);

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write FULL
    output.write((const char*) &nameStringID, 4);
  }//if has FULL subrecord

  if (!modelPath.empty())
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //MODL's length
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
      std::cout << "Error while writing subrecord MODT of WEAP!\n";
      return false;
    }
  }

  if (unknownMODS.isPresent())
  {
    //write MODS
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cout << "Error while writing subrecord MODS of WEAP!\n";
      return false;
    }
  }

  if (enchantingFormID!=0)
  {
    //write EITM
    output.write((const char*) &cEITM, 4);
    //EITM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write Enchanting form ID
    output.write((const char*) &enchantingFormID, 4);
  }//if has EITM subrecord

  if (hasEAMT)
  {
    //write EAMT
    output.write((const char*) &cEAMT, 4);
    //EAMT's length
    subLength = 2; //fixed size
    output.write((const char*) &subLength, 2);
    //write Enchantment Amount
    output.write((const char*) &enchantmentAmount, 2);
  }//if has EAMT subrecord

  if (equipTypeFormID!=0)
  {
    //write ETYP
    output.write((const char*) &cETYP, 4);
    //ETYP's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write Equip Type form ID
    output.write((const char*) &equipTypeFormID, 4);
  }//if has ETYP subrecord

  if (blockBashImpactDataSetFormID!=0)
  {
    //write BIDS
    output.write((const char*) &cBIDS, 4);
    //BIDS's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write Block Bash Impact Data Set's form ID
    output.write((const char*) &blockBashImpactDataSetFormID, 4);
  }//if has BIDS subrecord

  if (alternateBlockMaterialFormID!=0)
  {
    //write BAMT
    output.write((const char*) &cBAMT, 4);
    //BAMT's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write Alternate Block Material form ID
    output.write((const char*) &alternateBlockMaterialFormID, 4);
  }//if has BAMT subrecord

  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write keyword size
    const uint32_t k_Size = keywordArray.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*k_Size; //fixed size
    output.write((const char*) &subLength, 2);
    //write actual data
    uint32_t i;
    for (i=0; i<k_Size; ++i)
    {
      output.write((const char*) &(keywordArray[i]), 4);
    }//for
  }//if keyword array not empty

  //write DESC
  output.write((const char*) &cDESC, 4);
  //DESC's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write DESC
  output.write((const char*) &descriptionStringID, 4);

  if(!unknownNNAM.empty())
  {
    //write NNAM
    output.write((const char*) &cNNAM, 4);
    //KWDA's length
    subLength = unknownNNAM.length()+1; //fixed size
    output.write((const char*) &subLength, 2);
    //write actual data
    output.write((const char*) unknownNNAM.c_str(), subLength);
  }//if NNAM

  if (impactDataSetFormID!=0)
  {
    //write INAM
    output.write((const char*) &cINAM, 4);
    //INAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write Impact Data Set form ID
    output.write((const char*) &impactDataSetFormID, 4);
  }//if has INAM subrecord

  if (firstPersonModelObjectFormID!=0)
  {
    //write WNAM
    output.write((const char*) &cWNAM, 4);
    //WNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write 1st Person Model Object form ID
    output.write((const char*) &firstPersonModelObjectFormID, 4);
  }//if has WNAM subrecord

  if (attackFailSoundFormID!=0)
  {
    //write TNAM
    output.write((const char*) &cTNAM, 4);
    //TNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write Attack Fail sound form ID
    output.write((const char*) &attackFailSoundFormID, 4);
  }//if has TNAM subrecord

  if (idleSoundFormID!=0)
  {
    //write UNAM
    output.write((const char*) &cUNAM, 4);
    //UNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write Idle Sound form ID
    output.write((const char*) &idleSoundFormID, 4);
  }//if has UNAM subrecord

  if (equipSoundFormID!=0)
  {
    //write NAM9
    output.write((const char*) &cNAM9, 4);
    //NAM9's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write Equip Sound form ID
    output.write((const char*) &equipSoundFormID, 4);
  }//if has NAM9 subrecord

  if (unequipSoundFormID!=0)
  {
    //write NAM8
    output.write((const char*) &cNAM8, 4);
    //NAM8's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write UnEquip Sound form ID
    output.write((const char*) &unequipSoundFormID, 4);
  }//if has NAM8 subrecord

  if (attackSoundFormID!=0)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write Attack Sound form ID
    output.write((const char*) &attackSoundFormID, 4);
  }//if has SNAM subrecord

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 10;
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) &value, 4);
  output.write((const char*) &weight, 4);
  output.write((const char*) &baseDamage, 2);

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 100;
  output.write((const char*) &subLength, 2);
  //write DNAM
  output.write((const char*) unknownDNAM, 100);

  //write CRDT
  output.write((const char*) &cCRDT, 4);
  //CRDT's length
  subLength = 16;
  output.write((const char*) &subLength, 2);
  //write CRDT
  output.write((const char*) unknownCRDT, 16);

  //write VNAM
  output.write((const char*) &cVNAM, 4);
  //VNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write VNAM
  output.write((const char*) &unknownVNAM, 4);

  if (hasCNAM)
  {
    //write CNAM
    output.write((const char*) &cCNAM, 4);
    //CNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write CNAM
    output.write((const char*) &unknownCNAM, 4);
  }//if has CNAM subrecord

  return output.good();
}
#endif

bool WeaponRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of WEAP is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of WEAP!\n";
    return false;
  }
  editorID = std::string(buffer);

  unknownVMAD.setPresence(false);
  bool hasReadOBND = false;
  hasFULL = false;
  nameStringID = 0;
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  enchantingFormID = 0;
  equipTypeFormID = 0;
  blockBashImpactDataSetFormID = 0;
  alternateBlockMaterialFormID = 0;
  keywordArray.clear();
  uint32_t k_Size, i, fid;
  bool hasReadDESC = false;
  unknownNNAM.clear();
  attackSoundFormID = 0;
  bool hasReadDATA = false;
  impactDataSetFormID = 0;
  firstPersonModelObjectFormID = 0;
  attackFailSoundFormID = 0;
  idleSoundFormID = 0;
  equipSoundFormID = 0;
  unequipSoundFormID = 0;
  hasCNAM = false;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cout << "Error: WEAP seems to have more than one VMAD subrecord!\n";
             return false;
           }
           //read VMAD
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cout << "Error while reading subrecord VMAD of WEAP!\n";
             return false;
           }
           bytesRead = bytesRead +2+unknownVMAD.getSize();
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cout << "Error: WEAP seems to have more than one OBND subrecord!\n";
             return false;
           }
           //OBND's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout << "Error: sub record OBND of WEAP has invalid length ("
                       << subLength << " bytes). Should be 12 bytes.\n";
             return false;
           }
           //read OBND
           in_File.read((char*) unknownOBND, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord OBND of WEAP!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: WEAP seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, nameStringID, false)) return false;
           bytesRead += 6;
           hasFULL = true;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: WEAP seems to have more than one MODL subrecord!\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of WEAP is longer than 511 characters!\n";
             return false;
           }
           //read MODL path
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of WEAP!\n";
             return false;
           }
           modelPath = std::string(buffer);

           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, true))
           {
             std::cout << "Error while reading subrecord MODT of WEAP!\n";
             return false;
           }
           bytesRead = bytesRead + 4+2 +unknownMODT.getSize();
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cout << "Error: WEAP seems to have more than one MODS subrecord!\n";
             return false;
           }
           //read MODS
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
           {
             std::cout << "Error while reading subrecord MODS of WEAP!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMODS.getSize();
           break;
      case cEITM:
           if (enchantingFormID!=0)
           {
             std::cout << "Error: WEAP seems to have more than one EITM subrecord!\n";
             return false;
           }
           //read EITM
           if (!loadUint32SubRecordFromStream(in_File, cEITM, enchantingFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (enchantingFormID==0)
           {
             std::cout << "Error: subrecord EITM of WEAP is zero!\n";
             return false;
           }
           break;
      case cEAMT:
           if (hasEAMT)
           {
             std::cout << "Error: WEAP seems to have more than one EAMT subrecord!\n";
             return false;
           }
           //EAMT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: sub record EAMT of WEAP has invalid length ("
                       <<subLength <<" bytes). Should be two bytes.\n";
             return false;
           }
           //read EAMT
           in_File.read((char*) &enchantmentAmount, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord EAMT of WEAP!\n";
             return false;
           }
           hasEAMT = true;
           break;
      case cETYP:
           if (equipTypeFormID!=0)
           {
             std::cout << "Error: WEAP seems to have more than one ETYP subrecord!\n";
             return false;
           }
           //read ETYP
           if (!loadUint32SubRecordFromStream(in_File, cETYP, equipTypeFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (equipTypeFormID==0)
           {
             std::cout << "Error: subrecord ETYP of WEAP is zero!\n";
             return false;
           }
           break;
      case cBIDS:
           if (blockBashImpactDataSetFormID!=0)
           {
             std::cout << "Error: WEAP seems to have more than one BIDS subrecord!\n";
             return false;
           }
           //read BIDS
           if (!loadUint32SubRecordFromStream(in_File, cBIDS, blockBashImpactDataSetFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (blockBashImpactDataSetFormID==0)
           {
             std::cout << "Error: subrecord BIDS of WEAP is zero!\n";
             return false;
           }
           break;
      case cBAMT:
           if (alternateBlockMaterialFormID!=0)
           {
             std::cout << "Error: WEAP seems to have more than one BAMT subrecord!\n";
             return false;
           }
           //read BAMT
           if (!loadUint32SubRecordFromStream(in_File, cBAMT, alternateBlockMaterialFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (alternateBlockMaterialFormID==0)
           {
             std::cout << "Error: subrecord BAMT of WEAP is zero!\n";
             return false;
           }
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: WEAP seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //read KSIZ
           k_Size = 0;
           if (!loadUint32SubRecordFromStream(in_File, cKSIZ, k_Size, false)) return false;
           bytesRead += 6;

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
             std::cout <<"Error: sub record KWDA of WEAP has invalid length ("<<subLength
                       <<" bytes). Should be "<<4*k_Size<<" bytes.\n";
             return false;
           }
           //read KWDA
           keywordArray.clear();
           for (i=0; i<k_Size; ++i)
           {
             fid = 0;
             in_File.read((char*) &fid, 4);
             bytesRead += 4;
             keywordArray.push_back(subRecName);
           }
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord KWDA of WEAP!\n";
             return false;
           }
           break;
      case cDESC:
           if (hasReadDESC)
           {
             std::cout << "Error: WEAP seems to have more than one DESC subrecord!\n";
             return false;
           }
           //read DESC
           if (!loadUint32SubRecordFromStream(in_File, cDESC, descriptionStringID, false)) return false;
           bytesRead += 6;
           hasReadDESC = true;
           break;
      case cNNAM:
           if (!unknownNNAM.empty())
           {
             std::cout << "Error: WEAP seems to have more than one NNAM subrecord!\n";
             return false;
           }
           //NNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record NNAM of WEAP is longer than 511 characters!\n";
             return false;
           }
           //read NNAM (node name?)
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NNAM of WEAP!\n";
             return false;
           }
           unknownNNAM = std::string(buffer);
           break;
      case cINAM:
           if (impactDataSetFormID!=0)
           {
             std::cout << "Error: WEAP seems to have more than one INAM subrecord!\n";
             return false;
           }
           //read INAM
           if (!loadUint32SubRecordFromStream(in_File, cINAM, impactDataSetFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (impactDataSetFormID==0)
           {
             std::cout << "Error: subrecord INAM of WEAP is zero!\n";
             return false;
           }
           break;
      case cWNAM:
           if (firstPersonModelObjectFormID!=0)
           {
             std::cout << "Error: WEAP seems to have more than one WNAM subrecord!\n";
             return false;
           }
           //read WNAM
           if (!loadUint32SubRecordFromStream(in_File, cWNAM, firstPersonModelObjectFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (firstPersonModelObjectFormID==0)
           {
             std::cout << "Error: subrecord WNAM of WEAP is zero!\n";
             return false;
           }
           break;
      case cTNAM:
           if (attackFailSoundFormID!=0)
           {
             std::cout << "Error: WEAP seems to have more than one TNAM subrecord!\n";
             return false;
           }
           //read TNAM
           if (!loadUint32SubRecordFromStream(in_File, cTNAM, attackFailSoundFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (attackFailSoundFormID==0)
           {
             std::cout << "Error: subrecord TNAM of WEAP is zero!\n";
             return false;
           }
           break;
      case cUNAM:
           if (idleSoundFormID!=0)
           {
             std::cout << "Error: WEAP seems to have more than one UNAM subrecord!\n";
             return false;
           }
           //read UNAM
           if (!loadUint32SubRecordFromStream(in_File, cUNAM, idleSoundFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (idleSoundFormID==0)
           {
             std::cout << "Error: subrecord UNAM of WEAP is zero!\n";
             return false;
           }
           break;
      case cNAM9:
           if (equipSoundFormID!=0)
           {
             std::cout << "Error: WEAP seems to have more than one NAM9 subrecord!\n";
             return false;
           }
           //read NAM9
           if (!loadUint32SubRecordFromStream(in_File, cNAM9, equipSoundFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (equipSoundFormID==0)
           {
             std::cout << "Error: subrecord NAM9 of WEAP is zero!\n";
             return false;
           }
           break;
      case cNAM8:
           if (unequipSoundFormID!=0)
           {
             std::cout << "Error: WEAP seems to have more than one NAM8 subrecord!\n";
             return false;
           }
           //read NAM8
           if (!loadUint32SubRecordFromStream(in_File, cNAM8, unequipSoundFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (unequipSoundFormID==0)
           {
             std::cout << "Error: subrecord NAM9 of WEAP is zero!\n";
             return false;
           }
           break;
      case cSNAM:
           if (attackSoundFormID!=0)
           {
             std::cout << "Error: WEAP seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, attackSoundFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (attackSoundFormID==0)
           {
             std::cout << "Error: subrecord SNAM of WEAP is zero!\n";
             return false;
           }
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: WEAP seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=10)
           {
             std::cout <<"Error: sub record DATA of WEAP has invalid length ("<<subLength
                       <<" bytes). Should be 10 bytes.\n";
             return false;
           }
           //read DATA's content
           in_File.read((char*) &value, 4);
           in_File.read((char*) &weight, 4);
           in_File.read((char*) &baseDamage, 2);
           bytesRead += 10;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of WEAP!\n";
             return false;
           }
           hasReadDATA = true;

           //read DNAM
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cDNAM)
           {
             UnexpectedRecord(cDNAM, subRecName);
             return false;
           }
           //DNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=100)
           {
             std::cout <<"Error: sub record DNAM of WEAP has invalid length ("<<subLength
                       <<" bytes). Should be 100 bytes.\n";
             return false;
           }
           //read DNAM's content
           in_File.read((char*) unknownDNAM, 100);
           bytesRead += 100;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of WEAP!\n";
             return false;
           }

           //read CRDT
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cCRDT)
           {
             UnexpectedRecord(cCRDT, subRecName);
             return false;
           }
           //CRDT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=16)
           {
             std::cout <<"Error: sub record CRDT of WEAP has invalid length ("<<subLength
                       <<" bytes). Should be 16 bytes.\n";
             return false;
           }
           //read CRDT's content
           in_File.read((char*) unknownCRDT, 16);
           bytesRead += 16;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CRDT of WEAP!\n";
             return false;
           }

           //read VNAM
           if (!loadUint32SubRecordFromStream(in_File, cVNAM, unknownVNAM, true)) return false;
           bytesRead += 10;
           break;
      case cCNAM:
           if (hasCNAM)
           {
             std::cout << "Error: WEAP seems to have more than one CNAM subrecord!\n";
             return false;
           }
           //read CNAM
           if (!loadUint32SubRecordFromStream(in_File, cCNAM, unknownCNAM, false)) return false;
           hasCNAM = true;
           bytesRead += 6;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only KSIZ, DESC, INAM, WNAM or CNAM are allowed!\n";
           return false;
           break;
    }//swi
  }//while

  if (!(hasReadOBND and hasReadDESC and hasReadDATA))
  {
    std::cout << "Error: WEAP's OBND or DESC or DATA subrecord is missing!\n";
    return false;
  }

  return in_File.good();
}

} //namespace
