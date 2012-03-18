/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
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
: BasicRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false);
  memset(unknownOBND, 0, 12);
  hasFULL = false;
  nameStringID = 0;
  modelPath = "";
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  hasEITM = false;
  unknownEITM = 0;
  hasEAMT = false;
  unknownEAMT = 0;
  hasETYP = false;
  unknownETYP = 0;
  hasBIDS = false;
  unknownBIDS = 0;
  hasBAMT = false;
  unknownBAMT = 0;
  keywordArray.clear();
  descriptionStringID = 0;
  unknownNNAM = "";
  hasINAM = false;
  unknownINAM = 0;
  hasWNAM = false;
  unknownWNAM = 0;
  hasTNAM = false;
  unknownTNAM = 0;
  hasUNAM = false;
  unknownUNAM = 0;
  hasNAM9 = false;
  unknownNAM9 = 0;
  hasNAM8 = false;
  unknownNAM8 = 0;
  hasSNAM = false;
  unknownSNAM = 0;
  //DATA
  value = 0;
  weight = 0.0f;
  baseDamage = 0;
  //end of DATA
  memset(unknownDNAM, 0, 100);
  memset(unknownCRDT, 0, 16);
  unknownVNAM = 0;
  hasCNAM = false;
  unknownCNAM = 0;
}

WeaponRecord::WeaponRecord(const WeaponRecord& other)
{
  copyBasicMembers(other);
  editorID = other.editorID;
  unknownVMAD = other.unknownVMAD;
  memcpy(unknownOBND, other.unknownOBND, 12);
  hasFULL = other.hasFULL;
  nameStringID = other.nameStringID;
  modelPath = other.modelPath;
  unknownMODT = other.unknownMODT;
  unknownMODS = other.unknownMODS;
  hasEITM = other.hasEITM;
  unknownEITM = other.unknownEITM;
  hasEAMT = other.hasEAMT;
  unknownEAMT = other.unknownEAMT;
  hasETYP = other.hasETYP;
  unknownETYP = other.unknownETYP;
  hasBIDS = other.hasBIDS;
  unknownBIDS = other.unknownBIDS;
  hasBAMT = other.hasBAMT;
  unknownBAMT = other.unknownBAMT;
  keywordArray = other.keywordArray;
  descriptionStringID = other.descriptionStringID;
  unknownNNAM = other.unknownNNAM;
  hasINAM = other.hasINAM;
  unknownINAM = other.unknownINAM;
  hasWNAM = other.hasWNAM;
  unknownWNAM = other.unknownWNAM;
  hasTNAM = other.hasTNAM;
  unknownTNAM = other.unknownTNAM;
  hasUNAM = other.hasUNAM;
  unknownUNAM = other.unknownUNAM;
  hasNAM9 = other.hasNAM9;
  unknownNAM9 = other.unknownNAM9;
  hasNAM8 = other.hasNAM8;
  unknownNAM8 = other.unknownNAM8;
  hasSNAM = other.hasSNAM;
  unknownSNAM = other.unknownSNAM;
  value = other.value;
  weight = other.weight;
  baseDamage = other.baseDamage;
  memcpy(unknownDNAM, other.unknownDNAM, 100);
  memcpy(unknownCRDT, other.unknownCRDT, 16);
  unknownVNAM = other.unknownVNAM;
  hasCNAM = other.hasCNAM;
  unknownCNAM = other.unknownCNAM;
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
  hasEITM = other.hasEITM;
  unknownEITM = other.unknownEITM;
  hasEAMT = other.hasEAMT;
  unknownEAMT = other.unknownEAMT;
  hasETYP = other.hasETYP;
  unknownETYP = other.unknownETYP;
  hasBIDS = other.hasBIDS;
  unknownBIDS = other.unknownBIDS;
  hasBAMT = other.hasBAMT;
  unknownBAMT = other.unknownBAMT;
  keywordArray = other.keywordArray;
  descriptionStringID = other.descriptionStringID;
  unknownNNAM = other.unknownNNAM;
  hasINAM = other.hasINAM;
  unknownINAM = other.unknownINAM;
  hasWNAM = other.hasWNAM;
  unknownWNAM = other.unknownWNAM;
  hasTNAM = other.hasTNAM;
  unknownTNAM = other.unknownTNAM;
  hasUNAM = other.hasUNAM;
  unknownUNAM = other.unknownUNAM;
  hasNAM9 = other.hasNAM9;
  unknownNAM9 = other.unknownNAM9;
  hasNAM8 = other.hasNAM8;
  unknownNAM8 = other.unknownNAM8;
  hasSNAM = other.hasSNAM;
  unknownSNAM = other.unknownSNAM;
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

int32_t WeaponRecord::getRecordType() const
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
  if ((hasEITM!=other.hasEITM) or ((unknownEITM!=other.unknownEITM) and hasEITM)
    or (hasEAMT!=other.hasEAMT) or ((unknownEAMT!=other.unknownEAMT) and hasEAMT)
    or (hasETYP!=other.hasETYP) or ((unknownETYP!=other.unknownETYP) and hasETYP)
    or (hasBIDS!=other.hasBIDS) or ((unknownBIDS!=other.unknownBIDS) and hasBIDS)
    or ((hasBAMT!=other.hasBAMT) and hasBAMT) or (unknownBAMT!=other.unknownBAMT))
  {
    return false;
  }
  if ((keywordArray!=other.keywordArray) or (descriptionStringID!=other.descriptionStringID)
    or (unknownNNAM!=other.unknownNNAM)
    or (hasINAM!=other.hasINAM) or ((unknownINAM!=other.unknownINAM) and hasINAM)
    or (hasWNAM!=other.hasWNAM) or ((unknownWNAM!=other.unknownWNAM) and hasWNAM)
    or (hasTNAM!=other.hasTNAM) or ((unknownTNAM!=other.unknownTNAM) and hasTNAM)
    or (hasUNAM!=other.hasUNAM) or ((unknownUNAM!=other.unknownUNAM) and hasUNAM)
    or (hasNAM9!=other.hasNAM9) or ((unknownNAM9!=other.unknownNAM9) and hasNAM9)
    or (hasNAM8!=other.hasNAM8) or ((unknownNAM8!=other.unknownNAM8) and hasNAM8)
    or (hasCNAM!=other.hasCNAM) or ((unknownCNAM!=other.unknownCNAM) and hasCNAM)
    or (hasSNAM!=other.hasSNAM) or ((unknownSNAM!=other.unknownSNAM) and hasSNAM))
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
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of name +1 byte for NUL termination */
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
  if (hasEITM)
  {
    writeSize = writeSize +4 /* EITM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasEAMT)
  {
    writeSize = writeSize +4 /* EAMT */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasETYP)
  {
    writeSize = writeSize +4 /* ETYP */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasBIDS)
  {
    writeSize = writeSize +4 /* BIDS */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasBAMT)
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
  if (hasINAM)
  {
    writeSize = writeSize +4 /* INAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasWNAM)
  {
    writeSize = writeSize +4 /* WNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasTNAM)
  {
    writeSize = writeSize +4 /* TNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasUNAM)
  {
    writeSize = writeSize +4 /* UNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasNAM9)
  {
    writeSize = writeSize +4 /* NAM9 */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasNAM8)
  {
    writeSize = writeSize +4 /* NAM8 */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasSNAM)
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
  output.write((char*) &subLength, 2);
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
  output.write((char*) &subLength, 2);
  //write OBND
  output.write((const char*) unknownOBND, 12);

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write FULL
    output.write((const char*) &nameStringID, 4);
  }//if has FULL subrecord

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  subLength = modelPath.length()+1;
  output.write((char*) &subLength, 2);
  //write model path
  output.write(modelPath.c_str(), subLength);

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

  if (hasEITM)
  {
    //write EITM
    output.write((const char*) &cEITM, 4);
    //EITM's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write EITM
    output.write((const char*) &unknownEITM, 4);
  }//if has EITM subrecord

  if (hasEAMT)
  {
    //write EAMT
    output.write((const char*) &cEAMT, 4);
    //EAMT's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write EAMT
    output.write((const char*) &unknownEAMT, 4);
  }//if has EAMT subrecord

  if (hasETYP)
  {
    //write ETYP
    output.write((const char*) &cETYP, 4);
    //ETYP's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write ETYP
    output.write((const char*) &unknownETYP, 4);
  }//if has ETYP subrecord

  if (hasBIDS)
  {
    //write BIDS
    output.write((const char*) &cBIDS, 4);
    //BIDS's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write BIDS
    output.write((const char*) &unknownBIDS, 4);
  }//if has BIDS subrecord

  if (hasBAMT)
  {
    //write BAMT
    output.write((const char*) &cBAMT, 4);
    //BAMT's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write BAMT
    output.write((const char*) &unknownBAMT, 4);
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
    output.write((char*) &subLength, 2);
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
  output.write((char*) &subLength, 2);
  //write DESC
  output.write((const char*) &descriptionStringID, 4);

  if(!unknownNNAM.empty())
  {
    //write NNAM
    output.write((const char*) &cNNAM, 4);
    //KWDA's length
    subLength = unknownNNAM.length()+1; //fixed size
    output.write((char*) &subLength, 2);
    //write actual data
    output.write((const char*) unknownNNAM.c_str(), subLength);
  }//if NNAM

  if (hasINAM)
  {
    //write INAM
    output.write((const char*) &cINAM, 4);
    //INAM's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write INAM
    output.write((const char*) &unknownINAM, 4);
  }//if has INAM subrecord

  if (hasWNAM)
  {
    //write WNAM
    output.write((const char*) &cWNAM, 4);
    //WNAM's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write WNAM
    output.write((const char*) &unknownWNAM, 4);
  }//if has WNAM subrecord

  if (hasTNAM)
  {
    //write TNAM
    output.write((const char*) &cTNAM, 4);
    //TNAM's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write TNAM
    output.write((const char*) &unknownTNAM, 4);
  }//if has TNAM subrecord

  if (hasUNAM)
  {
    //write UNAM
    output.write((const char*) &cUNAM, 4);
    //UNAM's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write UNAM
    output.write((const char*) &unknownUNAM, 4);
  }//if has UNAM subrecord

  if (hasNAM9)
  {
    //write NAM9
    output.write((const char*) &cNAM9, 4);
    //NAM9's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write NAM9
    output.write((const char*) &unknownNAM9, 4);
  }//if has NAM9 subrecord

  if (hasNAM8)
  {
    //write NAM8
    output.write((const char*) &cNAM8, 4);
    //NAM8's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write NAM8
    output.write((const char*) &unknownNAM8, 4);
  }//if has NAM8 subrecord

  if (hasSNAM)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write SNAM
    output.write((const char*) &unknownSNAM, 4);
  }//if has SNAM subrecord

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 10;
  output.write((char*) &subLength, 2);
  //write DATA
  output.write((const char*) &value, 4);
  output.write((const char*) &weight, 4);
  output.write((const char*) &baseDamage, 2);

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 100;
  output.write((char*) &subLength, 2);
  //write DNAM
  output.write((const char*) unknownDNAM, 100);

  //write CRDT
  output.write((const char*) &cCRDT, 4);
  //CRDT's length
  subLength = 16;
  output.write((char*) &subLength, 2);
  //write CRDT
  output.write((const char*) unknownCRDT, 16);

  //write VNAM
  output.write((const char*) &cVNAM, 4);
  //VNAM's length
  subLength = 4; //fixed size
  output.write((char*) &subLength, 2);
  //write VNAM
  output.write((const char*) &unknownVNAM, 4);

  if (hasCNAM)
  {
    //write CNAM
    output.write((const char*) &cCNAM, 4);
    //CNAM's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
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
  int32_t subRecName;
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
  unknownMODS.setPresence(false);
  hasETYP = false;
  hasBIDS = false;
  hasBAMT = false;
  keywordArray.clear();
  uint32_t k_Size, i, fid;
  bool hasReadDESC = false;
  unknownNNAM.clear();
  hasSNAM = false;
  bool hasReadDATA = false;
  hasINAM = false;
  hasWNAM = false;
  hasTNAM = false;
  hasUNAM = false;
  hasNAM9 = false;
  hasNAM8 = false;
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
             std::cout <<"Error: sub record OBND of WEAP has invalid length ("<<subLength
                       <<" bytes). Should be 12 bytes.\n";
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
           if (hasEITM)
           {
             std::cout << "Error: WEAP seems to have more than one EITM subrecord!\n";
             return false;
           }
           //read EITM
           if (!loadUint32SubRecordFromStream(in_File, cEITM, unknownEITM, false)) return false;
           bytesRead += 6;
           hasEITM = true;
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
             std::cout <<"Error: sub record EAMT of WEAP has invalid length ("<<subLength
                       <<" bytes). Should be 2 bytes.\n";
             return false;
           }
           //read EAMT
           in_File.read((char*) &unknownEAMT, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord EAMT of WEAP!\n";
             return false;
           }
           hasEAMT = true;
           break;
      case cETYP:
           if (hasETYP)
           {
             std::cout << "Error: WEAP seems to have more than one ETYP subrecord!\n";
             return false;
           }
           //read ETYP
           if (!loadUint32SubRecordFromStream(in_File, cETYP, unknownETYP, false)) return false;
           bytesRead += 6;
           hasETYP = true;
           break;
      case cBIDS:
           if (hasBIDS)
           {
             std::cout << "Error: WEAP seems to have more than one BIDS subrecord!\n";
             return false;
           }
           //read BIDS
           if (!loadUint32SubRecordFromStream(in_File, cBIDS, unknownBIDS, false)) return false;
           bytesRead += 6;
           hasBIDS = true;
           break;
      case cBAMT:
           if (hasBAMT)
           {
             std::cout << "Error: WEAP seems to have more than one BAMT subrecord!\n";
             return false;
           }
           //read BAMT
           if (!loadUint32SubRecordFromStream(in_File, cBAMT, unknownBAMT, false)) return false;
           bytesRead += 6;
           hasBAMT = true;
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
           if (hasINAM)
           {
             std::cout << "Error: WEAP seems to have more than one INAM subrecord!\n";
             return false;
           }
           //read INAM
           if (!loadUint32SubRecordFromStream(in_File, cINAM, unknownINAM, false)) return false;
           hasINAM = true;
           bytesRead += 6;
           break;
      case cWNAM:
           if (hasWNAM)
           {
             std::cout << "Error: WEAP seems to have more than one WNAM subrecord!\n";
             return false;
           }
           //read WNAM
           if (!loadUint32SubRecordFromStream(in_File, cWNAM, unknownWNAM, false)) return false;
           hasWNAM = true;
           bytesRead += 6;
           break;
      case cTNAM:
           if (hasTNAM)
           {
             std::cout << "Error: WEAP seems to have more than one TNAM subrecord!\n";
             return false;
           }
           //read TNAM
           if (!loadUint32SubRecordFromStream(in_File, cTNAM, unknownTNAM, false)) return false;
           bytesRead += 6;
           hasTNAM = true;
           break;
      case cUNAM:
           if (hasUNAM)
           {
             std::cout << "Error: WEAP seems to have more than one UNAM subrecord!\n";
             return false;
           }
           //read UNAM
           if (!loadUint32SubRecordFromStream(in_File, cUNAM, unknownUNAM, false)) return false;
           bytesRead += 6;
           hasUNAM = true;
           break;
      case cNAM9:
           if (hasNAM9)
           {
             std::cout << "Error: WEAP seems to have more than one NAM9 subrecord!\n";
             return false;
           }
           //read NAM9
           if (!loadUint32SubRecordFromStream(in_File, cNAM9, unknownNAM9, false)) return false;
           bytesRead += 6;
           hasNAM9 = true;
           break;
      case cNAM8:
           if (hasNAM8)
           {
             std::cout << "Error: WEAP seems to have more than one NAM8 subrecord!\n";
             return false;
           }
           //read NAM8
           if (!loadUint32SubRecordFromStream(in_File, cNAM8, unknownNAM8, false)) return false;
           bytesRead += 6;
           hasNAM8 = true;
           break;
      case cSNAM:
           if (hasSNAM)
           {
             std::cout << "Error: WEAP seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, unknownSNAM, false)) return false;
           hasSNAM = true;
           bytesRead += 6;
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
