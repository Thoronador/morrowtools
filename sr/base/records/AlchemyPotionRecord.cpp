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

#include "AlchemyPotionRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

AlchemyPotionRecord::AlchemyPotionRecord()
: BasicRecord()
{
  editorID = "";
  memset(unknownOBND, 0, 12);
  hasFULL = false;
  nameStringID = 0;
  keywordArray.clear();
  modelPath = "";
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  hasYNAM = false;
  unknownYNAM = 0;
  hasZNAM = false;
  unknownZNAM = 0;
  unknownDATA = 0;
  memset(unknownENIT, 0, 20);
  effects.clear();
}

AlchemyPotionRecord::~AlchemyPotionRecord()
{
  //empty
}

bool AlchemyPotionRecord::equals(const AlchemyPotionRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0) and (hasFULL==other.hasFULL)
      and ((nameStringID==other.nameStringID) or (!hasFULL)) and (keywordArray==other.keywordArray)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (unknownMODS==other.unknownMODS)
      and (hasYNAM==other.hasYNAM) and ((unknownYNAM==other.unknownYNAM) or (!hasYNAM))
      and (hasZNAM==other.hasZNAM) and ((unknownZNAM==other.unknownZNAM) or (!hasZNAM))
      and (unknownDATA==other.unknownDATA)
      and (memcmp(unknownENIT, other.unknownENIT, 20)==0)
      and (effects==other.effects));
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t AlchemyPotionRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of path +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +4 /* fixed length */
        +4 /* ENIT */ +2 /* 2 bytes for length */ +20 /* fixed length */;
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has FULL
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed length */
                          +4 /* KWDA */ +2 /* 2 bytes for length */ +4*keywordArray.size() /*data length*/ ;
  }//if keywords
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* length */;
  }//if MODT
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize +4 /* MODS */ +2 /* 2 bytes for length */ +unknownMODS.getSize() /* length */;
  }//if MODS
  if (hasYNAM)
  {
    writeSize = writeSize +4 /* YNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has YNAM
  if (hasZNAM)
  {
    writeSize = writeSize +4 /* ZNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has ZNAM
  uint32_t i;
  for (i=0; i<effects.size(); ++i)
  {
    writeSize = writeSize +effects.at(i).getWriteSize();
  }//for
  return writeSize;
}

bool AlchemyPotionRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cALCH, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

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

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write FULL's data
    output.write((const char*) &nameStringID, 4);
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
      std::cout << "Error while writing MODT of ALCH!";
      return false;
    }
  }//if MODT

  if (unknownMODS.isPresent())
  {
    //write MODS
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cout << "Error while writing MODS of ALCH!";
      return false;
    }
  }//if MODS

  if (hasYNAM)
  {
    //write YNAM
    output.write((const char*) &cYNAM, 4);
    //YNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write YNAM's data
    output.write((const char*) &unknownYNAM, 4);
  }//if has YNAM

  if (hasZNAM)
  {
    //write ZNAM
    output.write((const char*) &cZNAM, 4);
    //ZNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write ZNAM's data
    output.write((const char*) &unknownZNAM, 4);
  }//if has ZNAM

  //write ENIT
  output.write((const char*) &cENIT, 4);
  //ENIT's length
  subLength = 20; //fixed
  output.write((const char*) &subLength, 2);
  //write ENIT
  output.write((const char*) unknownENIT, 20);

  for (i=0; i<effects.size(); ++i)
  {
    if (!effects[i].saveToStream(output))
    {
      std::cout << "Error while writing effect blocks of ALCH!\n";
      return false;
    }
  }//for

  return  output.good();
}
#endif

bool AlchemyPotionRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of ALCH is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of ALCH!\n";
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
    std::cout <<"Error: sub record OBND of ALCH has invalid length ("
              <<subLength<<" bytes. Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of ALCH!\n";
    return false;
  }

  hasFULL = false;
  nameStringID = 0;
  keywordArray.clear();
  uint32_t i, temp, k_Size;
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  hasYNAM = false;
  hasZNAM = false;
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
           if (hasFULL)
           {
             std::cout << "Error: recurd ALCH seems to have more than one FULL subrecord!\n";
             return false;
           }
           //FULL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record FULL of ALCH has invalid length ("
                       <<subLength<<" bytes. Should be four bytes!\n";
             return false;
           }
           //read FULL's stuff
           in_File.read((char*) &nameStringID, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FULL of ALCH!\n";
             return false;
           }
           hasFULL = true;
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: recurd ALCH seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //KSIZ's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord KSIZ of ALCH has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read KSIZ's stuff
           k_Size = 0;
           in_File.read((char*) &k_Size, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord KSIZ of ALCH!\n";
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
             std::cout <<"Error: sub record KWDA of ALCH has invalid length ("
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
               std::cout << "Error while reading subrecord KWDA of ALCH!\n";
               return false;
             }//if
             keywordArray.push_back(temp);
           }//for
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: ALCH seems to have more than one MODL subrecord!\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of ALCH is longer than 511 characters!\n";
             return false;
           }
           //read MODL's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of ALCH!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: ALCH seems to have more than one MODT subrecord!\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of ALCH!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMODT.getSize();
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cout << "Error: ALCH seems to have more than one MODS subrecord!\n";
             return false;
           }
           //read MODS
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
           {
             std::cout << "Error while reading subrecord MODS of ALCH!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMODS.getSize();
           break;
      case cYNAM:
           if (hasYNAM)
           {
             std::cout << "Error: ALCH seems to have more than one YNAM subrecord!\n";
             return false;
           }
           //YNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord YNAM of ALCH has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read YNAM's stuff
           in_File.read((char*) &unknownYNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord YNAM of ALCH!\n";
             return false;
           }//if
           hasYNAM = true;
           break;
      case cZNAM:
           if (hasZNAM)
           {
             std::cout << "Error: ALCH seems to have more than one ZNAM subrecord!\n";
             return false;
           }
           //ZNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord ZNAM of ALCH has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read ZNAM's stuff
           in_File.read((char*) &unknownZNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ZNAM of ALCH!\n";
             return false;
           }//if
           hasZNAM = true;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: ALCH seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord DATA of ALCH has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &unknownDATA, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of ALCH!\n";
             return false;
           }//if
           hasReadDATA = true;
           break;
      case cENIT:
           if (hasReadENIT)
           {
             std::cout << "Error: recurd ALCH seems to have more than one ENIT subrecord!\n";
             return false;
           }
           //ENIT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=20)
           {
             std::cout <<"Error: subrecord ENIT of ALCH has invalid length ("
                       <<subLength<<" bytes). Should be 20 bytes!\n";
             return false;
           }
           //read ENIT's stuff
           in_File.read((char*) unknownENIT, 20);
           bytesRead += 20;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ENIT of ALCH!\n";
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
             std::cout <<"Error: subrecord EFID of ALCH has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read EFID's stuff
           in_File.read((char*) &(tempEffect.effectFormID), 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord EFID of ALCH!\n";
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
             std::cout <<"Error: subrecord EFIT of ALCH has invalid length ("
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
             std::cout << "Error while reading subrecord EFIT of ALCH!\n";
             return false;
           }
           hasNonPushedEffect = true;
           break;
      //We don't need CTDA stuff yet, but it might be useful in the future.
      case cCTDA:
           if (!hasNonPushedEffect)
           {
             std::cout << "Error while reading ALCH: CTDA found, but there was no EFID/EFIT block.\n";
             return false;
           }
           //CTDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=32)
           {
             std::cout <<"Error: subrecord CTDA of ALCH has invalid length ("
                       <<subLength<<" bytes). Should be 32 bytes!\n";
             return false;
           }
           //read CTDA's stuff
           in_File.read((char*) &(tempCTDA.content), 32);
           bytesRead += 32;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CTDA of ALCH!\n";
             return false;
           }
           tempEffect.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only KSIZ, MODL, MODT, MODS, YNAM, ZNAM,"
                     << " DATA, ENIT, EFID or CTDA are allowed here!\n";
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
    std::cout << "Error: DATA or ENIT subrecord of ALCH is missing!\n";
    return false;
  }
  if (effects.empty())
  {
    std::cout << "Error: ALCH has no effects!\n";
    return false;
  }

  return in_File.good();
}

int32_t AlchemyPotionRecord::getRecordType() const
{
  return cALCH;
}

} //namespace
