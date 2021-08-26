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

#include "AlchemyPotionRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

/* flag constants */
const uint32_t AlchemyPotionRecord::cFlagNoAutoCalc = 0x00000001;
const uint32_t AlchemyPotionRecord::cFlagFoodItem   = 0x00000002;
const uint32_t AlchemyPotionRecord::cFlagMedicine   = 0x00010000;
const uint32_t AlchemyPotionRecord::cFlagPoison     = 0x00020000;

AlchemyPotionRecord::AlchemyPotionRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  keywordArray(std::vector<uint32_t>()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownMODS(BinarySubRecord()),
  pickupSoundFormID(0),
  putdownSoundFormID(0),
  equipTypeFormID(0),
  weight(0.0f),
  //subrecord ENIT
  value(0),
  flags(0),
  unknownThirdENIT(0),
  addictionChance(0.0f),
  useSoundFormID(0),
  //end of ENIT
  effects(std::vector<EffectBlock>())
{
  memset(unknownOBND, 0, 12);
}

AlchemyPotionRecord::~AlchemyPotionRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool AlchemyPotionRecord::equals(const AlchemyPotionRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (name==other.name) and (keywordArray==other.keywordArray)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (unknownMODS==other.unknownMODS)
      and (pickupSoundFormID==other.pickupSoundFormID)
      and (putdownSoundFormID==other.putdownSoundFormID)
      and (equipTypeFormID==other.equipTypeFormID) and (weight==other.weight)
      and (value==other.value) and (flags==other.flags)
      and (unknownThirdENIT==other.unknownThirdENIT) and (addictionChance==other.addictionChance)
      and (useSoundFormID==other.useSoundFormID)
      and (effects==other.effects));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t AlchemyPotionRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of path +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +4 /* fixed length */
        +4 /* ENIT */ +2 /* 2 bytes for length */ +20 /* fixed length */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }//if has FULL
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed length */
                          +4 /* KWDA */ +2 /* 2 bytes for length */ +4*keywordArray.size() /*data length*/ ;
  }//if keywords
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */ + unknownMODT.size() /* length */;
  }//if MODT
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize + 4 /* MODS */ + 2 /* 2 bytes for length */ + unknownMODS.size() /* length */;
  }//if MODS
  if (pickupSoundFormID!=0)
  {
    writeSize = writeSize +4 /* YNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has YNAM
  if (putdownSoundFormID!=0)
  {
    writeSize = writeSize +4 /* ZNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has ZNAM
  if (equipTypeFormID!=0)
  {
    writeSize = writeSize +4 /* ETYP */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has ETYP
  uint32_t i;
  for (i=0; i<effects.size(); ++i)
  {
    writeSize = writeSize +effects.at(i).getWriteSize();
  }//for
  return writeSize;
}

bool AlchemyPotionRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cALCH, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

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

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  subLength = modelPath.length()+1;
  output.write((const char*) &subLength, 2);
  //write model path
  output.write(modelPath.c_str(), subLength);

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing MODT of ALCH!";
      return false;
    }
  }//if MODT

  if (unknownMODS.isPresent())
  {
    //write MODS
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cerr << "Error while writing MODS of ALCH!";
      return false;
    }
  }//if MODS

  if (pickupSoundFormID!=0)
  {
    //write YNAM
    output.write((const char*) &cYNAM, 4);
    //YNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write pickup sound form ID
    output.write((const char*) &pickupSoundFormID, 4);
  }//if has YNAM

  if (putdownSoundFormID!=0)
  {
    //write ZNAM
    output.write((const char*) &cZNAM, 4);
    //ZNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write putdown sound form ID
    output.write((const char*) &putdownSoundFormID, 4);
  }//if has ZNAM

  if (equipTypeFormID!=0)
  {
    //write ETYP
    output.write((const char*) &cETYP, 4);
    //ETYP's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write equip type form ID
    output.write((const char*) &equipTypeFormID, 4);
  }//if has ETYP

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) &weight, 4);

  //write ENIT
  output.write((const char*) &cENIT, 4);
  //ENIT's length
  subLength = 20; //fixed
  output.write((const char*) &subLength, 2);
  //write ENIT
  output.write((const char*) &value, 4);
  output.write((const char*) &flags, 4);
  output.write((const char*) &unknownThirdENIT, 4);
  output.write((const char*) &addictionChance, 4);
  output.write((const char*) &useSoundFormID, 4);

  for (i=0; i<effects.size(); ++i)
  {
    if (!effects[i].saveToStream(output))
    {
      std::cerr << "Error while writing effect blocks of ALCH!\n";
      return false;
    }
  }//for

  return  output.good();
}
#endif

bool AlchemyPotionRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cerr <<"Error: sub record EDID of ALCH is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of ALCH!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read OBND
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  //OBND's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=12)
  {
    std::cerr <<"Error: sub record OBND of ALCH has invalid length ("
              <<subLength<<" bytes. Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord OBND of ALCH!\n";
    return false;
  }

  name.reset();
  keywordArray.clear();
  uint32_t i, temp, k_Size;
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  pickupSoundFormID = 0;
  putdownSoundFormID = 0;
  equipTypeFormID = 0;
  bool hasReadDATA = false;
  bool hasReadENIT = false;
  effects.clear();
  EffectBlock tempEffect;
  CTDAData tempCTDA;
  bool hasNonPushedEffect = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: recurd ALCH seems to have more than one FULL subrecord!\n";
             return false;
           }
           //FULL's length
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cerr << "Error: recurd ALCH seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //KSIZ's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cerr <<"Error: subrecord KSIZ of ALCH has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read KSIZ's stuff
           k_Size = 0;
           in_File.read((char*) &k_Size, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord KSIZ of ALCH!\n";
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
             std::cerr <<"Error: sub record KWDA of ALCH has invalid length ("
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
               std::cerr << "Error while reading subrecord KWDA of ALCH!\n";
               return false;
             }//if
             keywordArray.push_back(temp);
           }//for
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: ALCH seems to have more than one MODL subrecord!\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cerr <<"Error: sub record MODL of ALCH is longer than 511 characters!\n";
             return false;
           }
           //read MODL's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord MODL of ALCH!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: ALCH seems to have more than one MODT subrecord!\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading subrecord MODT of ALCH!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODT.size();
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cerr << "Error: ALCH seems to have more than one MODS subrecord!\n";
             return false;
           }
           //read MODS
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
           {
             std::cerr << "Error while reading subrecord MODS of ALCH!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODS.size();
           break;
      case cYNAM:
           if (pickupSoundFormID!=0)
           {
             std::cerr << "Error: ALCH seems to have more than one YNAM subrecord!\n";
             return false;
           }
           //read YNAM
           if (!loadUint32SubRecordFromStream(in_File, cYNAM, pickupSoundFormID, false)) return false;
           bytesRead += 6;
           //check value
           if (pickupSoundFormID==0)
           {
             std::cerr << "Error: Subrecord YNAM of ALCH has value zero!\n";
             return false;
           }//if
           break;
      case cZNAM:
           if (putdownSoundFormID!=0)
           {
             std::cerr << "Error: ALCH seems to have more than one ZNAM subrecord!\n";
             return false;
           }
           //read ZNAM
           if (!loadUint32SubRecordFromStream(in_File, cZNAM, putdownSoundFormID, false)) return false;
           bytesRead += 6;
           //check value
           if (putdownSoundFormID==0)
           {
             std::cerr << "Error: Subrecord ZNAM of ALCH has value zero!\n";
             return false;
           }//if
           break;
      case cETYP:
           if (equipTypeFormID!=0)
           {
             std::cerr << "Error: ALCH seems to have more than one ETYP subrecord!\n";
             return false;
           }
           //read ETYP
           if (!loadUint32SubRecordFromStream(in_File, cETYP, equipTypeFormID, false)) return false;
           bytesRead += 6;
           //check value
           if (equipTypeFormID==0)
           {
             std::cerr << "Error: Subrecord ETYP of ALCH has value zero!\n";
             return false;
           }//if
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: ALCH seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cerr <<"Error: subrecord DATA of ALCH has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &weight, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of ALCH!\n";
             return false;
           }//if
           hasReadDATA = true;
           break;
      case cENIT:
           if (hasReadENIT)
           {
             std::cerr << "Error: ALCH seems to have more than one ENIT subrecord!\n";
             return false;
           }
           //ENIT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=20)
           {
             std::cerr <<"Error: subrecord ENIT of ALCH has invalid length ("
                       <<subLength<<" bytes). Should be 20 bytes!\n";
             return false;
           }
           //read ENIT's stuff
           in_File.read((char*) &value, 4);
           in_File.read((char*) &flags, 4);
           in_File.read((char*) &unknownThirdENIT, 4);
           in_File.read((char*) &addictionChance, 4);
           in_File.read((char*) &useSoundFormID, 4);
           bytesRead += 20;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord ENIT of ALCH!\n";
             return false;
           }//if
           hasReadENIT = true;
           break;
      case cEFID:
           //check for old effect block
           if (hasNonPushedEffect)
           {
             //need to push
             effects.push_back(tempEffect);
             hasNonPushedEffect = false;
           }
           //new effect block
           tempEffect.unknownCTDA_CIS2s.clear();
           //EFID's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cerr <<"Error: subrecord EFID of ALCH has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read EFID's stuff
           in_File.read((char*) &(tempEffect.effectFormID), 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord EFID of ALCH!\n";
             return false;
           }

           //read EFIT
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cEFIT)
           {
             UnexpectedRecord(cEFIT, subRecName);
             return false;
           }
           //EFIT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cerr <<"Error: subrecord EFIT of ALCH has invalid length ("
                       <<subLength<<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read EFIT's stuff
           in_File.read((char*) &(tempEffect.magnitude), 4);
           in_File.read((char*) &(tempEffect.areaOfEffect), 4);
           in_File.read((char*) &(tempEffect.duration), 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord EFIT of ALCH!\n";
             return false;
           }
           hasNonPushedEffect = true;
           break;
      //We don't need CTDA stuff yet, but it might be useful in the future.
      case cCTDA:
           if (!hasNonPushedEffect)
           {
             std::cerr << "Error while reading ALCH: CTDA found, but there was no EFID/EFIT block.\n";
             return false;
           }
           // read CTDA's stuff
           if (!tempCTDA.loadFromStream(in_File, bytesRead))
           {
             std::cerr << "Error while reading subrecord CTDA of ALCH!\n";
             return false;
           }
           tempEffect.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
           break;
      default:
           std::cerr << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only KSIZ, MODL, MODT, MODS, YNAM, ZNAM,"
                     << " ETYP, DATA, ENIT, EFID or CTDA are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //check possibly not yet pushed effect block
  if (hasNonPushedEffect)
  {
    effects.push_back(tempEffect);
  }

  //presence checks
  if (!(hasReadDATA and hasReadENIT))
  {
    std::cerr << "Error: DATA or ENIT subrecord of ALCH is missing!\n";
    return false;
  }
  if (effects.empty())
  {
    std::cerr << "Error: ALCH has no effects!\n";
    return false;
  }

  return in_File.good();
}

uint32_t AlchemyPotionRecord::getRecordType() const
{
  return cALCH;
}

bool AlchemyPotionRecord::doesAutoCalc() const
{
  return ((flags & cFlagNoAutoCalc)==0);
}

} //namespace
