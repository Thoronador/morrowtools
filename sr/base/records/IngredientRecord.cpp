/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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

#include "IngredientRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

IngredientRecord::IngredientRecord()
: BasicRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false);
  memset(unknownOBND, 0, 12);
  nameStringID = 0;
  keywordArray.clear();
  modelPath = "";
  unknownMODT.setPresence(false);
  hasYNAM = false;
  unknownYNAM = 0;
  hasZNAM = false;
  unknownZNAM = 0;
  memset(unknownDATA, 0, 8);
  memset(unknownENIT, 0, 8);
  effects.clear();
}

IngredientRecord::~IngredientRecord()
{
  //empty
}

bool IngredientRecord::equals(const IngredientRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (nameStringID==other.nameStringID) and (keywordArray==other.keywordArray)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (hasYNAM==other.hasYNAM) and ((unknownYNAM==other.unknownYNAM) or (!hasYNAM))
      and (hasZNAM==other.hasZNAM) and ((unknownZNAM==other.unknownZNAM) or (!hasZNAM))
      and (memcmp(unknownDATA, other.unknownDATA, 8)==0)
      and (memcmp(unknownENIT, other.unknownENIT, 8)==0)
      and (effects==other.effects));
}

bool IngredientRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cINGR, 4);
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */
        +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +8 /* fixed length of 8 bytes */
        +4 /* ENIT */ +2 /* 2 bytes for length */ +8 /* fixed length of 8 bytes */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /*VMAD*/ +2 /* 2 bytes for length */
               +unknownVMAD.getSize() /* size */;
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */
               +4 /* KWDA */ +2 /* 2 bytes for length */ +4*keywordArray.size(); /* fixed length of 4 bytes per elem.*/
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /*MODT*/ +2 /* 2 bytes for length */
               +unknownMODT.getSize() /* size */;
  }
  if (hasYNAM)
  {
    writeSize = writeSize +4 /* YNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  }//if has YNAM
  if (hasZNAM)
  {
    writeSize = writeSize +4 /* ZNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  }//if has YNAM
  unsigned int i;
  if (!effects.empty())
  {
    for (i=0; i<effects.size(); ++i)
    {
      writeSize = writeSize +4 /*EFID*/ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */
                 +4 /*EFIT*/ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */
                 + effects[i].unknownCTDAs.size()*
                    (4 /*CTDA*/ +2 /* 2 bytes for length */ +32 /* fixed length of 32 bytes */);
    }//for
  }//if effects
  if (!saveSizeAndUnknownValues(output, writeSize)) return false;

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
      std::cout << "Error while writing subrecord VMAD of INGR!\n";
      return false;
    }
  }//if VMAD

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write OBND's stuff
  output.write((const char*) unknownOBND, 12);

  //write FULL
  output.write((const char*) &cFULL, 4);
  //FULL's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write FULL's stuff
  output.write((const char*) &nameStringID, 4);

  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write KSIZ's stuff
    uint32_t len = keywordArray.size();
    output.write((const char*) &len, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*len; //fixed
    output.write((const char*) &subLength, 2);
    //write keywords' form IDs
    unsigned int i;
    for (i=0; i<len; ++i)
    {
      output.write((const char*) &(keywordArray[i]), 4);
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
      std::cout << "Error while writing subrecord MODT of INGR!\n";
      return false;
    }
  }//if MODT

  if (hasYNAM)
  {
    //write YNAM
    output.write((const char*) &cYNAM, 4);
    //YNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write YNAM's stuff
    output.write((const char*) &unknownYNAM, 4);
  }//if has YNAM

  if (hasZNAM)
  {
    //write ZNAM
    output.write((const char*) &cZNAM, 4);
    //ZNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write ZNAM's stuff
    output.write((const char*) &unknownZNAM, 4);
  }//if ZNAM

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 8; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) unknownDATA, 8);

  //write ENIT
  output.write((const char*) &cENIT, 4);
  //ENIT's length
  subLength = 8; //fixed
  output.write((const char*) &subLength, 2);
  //write ENIT's stuff
  output.write((const char*) unknownENIT, 8);

  if (!effects.empty())
  {
    unsigned int jay;
    for (i=0; i<effects.size(); ++i)
    {
      //write EFID
      output.write((const char*) &cEFID, 4);
      //EFID's length
      subLength = 4; //fixed
      output.write((const char*) &subLength, 2);
      //write EFID's stuff
      output.write((const char*) &(effects[i].unknownEFID), 4);

      //write EFIT
      output.write((const char*) &cEFIT, 4);
      //EFIT's length
      subLength = 12; //fixed
      output.write((const char*) &subLength, 2);
      //write EFIT's stuff
      output.write((const char*) &(effects[i].unknownEFITs[0]), 4);
      output.write((const char*) &(effects[i].unknownEFITs[1]), 4);
      output.write((const char*) &(effects[i].unknownEFITs[2]), 4);

      for (jay=0; jay<effects[i].unknownCTDAs.size(); ++jay)
      {
        //write CTDA
        output.write((const char*) &cCTDA, 4);
        //CTDA's length
        subLength = 32; //fixed
        output.write((const char*) &subLength, 2);
        //write CTDA's stuff
        output.write((const char*) &(effects[i].unknownCTDAs[jay].content), 32);
      }//for jay
    }//for i
  }//if effects

  return output.good();
}

bool IngredientRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of INGR is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of INGR!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read (VMAD or) OBND
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName==cVMAD)
  {
    //has VMAD
    if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
    {
      std::cout << "Error while reading subrecord VMAD of INGR!\n";
      return false;
    }
    bytesRead = bytesRead +2 +unknownVMAD.getSize();

    //read next subrecord (OBND)
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
  }
  else
  {
    //no VMAD here!
    unknownVMAD.setPresence(false);
  }

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
    std::cout <<"Error: subrecord OBND of INGR has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of INGR!\n";
    return false;
  }

  bool hasReadFULL = false;
  keywordArray.clear();
  uint32_t tempKeyword, kwdaLength, i;
  modelPath.clear();
  unknownMODT.setPresence(false);
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
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cFULL:
           if (hasReadFULL)
           {
             std::cout << "Error: INGR seems to have more than one FULL subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, nameStringID)) return false;
           bytesRead += 6;
           hasReadFULL = true;
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: INGR seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read KSIZ
           kwdaLength = 0;
           if (!loadUint32SubRecordFromStream(in_File, cKSIZ, kwdaLength)) return false;
           bytesRead += 6;

           //read KWDA
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cKWDA)
           {
             UnexpectedRecord(cOBND, subRecName);
             return false;
           }
           //KWDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=kwdaLength*4)
           {
             std::cout <<"Error: subrecord KWDA of INGR has invalid length ("<<subLength
                       <<" bytes). Should be "<<kwdaLength*4<<" bytes!\n";
             return false;
           }
           //read KWDA's stuff
           for (i=0; i<kwdaLength; ++i)
           {
             in_File.read((char*) &tempKeyword, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord KWDA of INGR!\n";
               return false;
             }
             keywordArray.push_back(tempKeyword);
           }//for
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: INGR seems to have more than one MODL subrecord!\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of INGR is longer than 511 characters!\n";
             return false;
           }
           //read MODL's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of INGR!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: INGR seems to have more than one MODT subrecord!\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of INGR!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMODT.getSize();
           break;
      case cYNAM:
           if (hasYNAM)
           {
             std::cout << "Error: INGR seems to have more than one YNAM subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read YNAM
           if (!loadUint32SubRecordFromStream(in_File, cYNAM, unknownYNAM)) return false;
           bytesRead += 6;
           hasYNAM = true;
           break;
      case cZNAM:
           if (hasZNAM)
           {
             std::cout << "Error: INGR seems to have more than one ZNAM subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read ZNAM
           if (!loadUint32SubRecordFromStream(in_File, cZNAM, unknownZNAM)) return false;
           bytesRead += 6;
           hasZNAM = true;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: INGR seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: subrecord DATA of INGR has invalid length ("
                       <<subLength<<" bytes). Should be 8 bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) unknownDATA, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of INGR!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cENIT:
           if (hasReadENIT)
           {
             std::cout << "Error: INGR seems to have more than one ENIT subrecord!\n";
             return false;
           }
           //ENIT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: subrecord ENIT of INGR has invalid length ("
                       <<subLength<<" bytes). Should be 8 bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) unknownENIT, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ENIT of INGR!\n";
             return false;
           }
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
           tempEffect.unknownCTDAs.clear();
           //EFID's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord EFID of INGR has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read EFID's stuff
           in_File.read((char*) &(tempEffect.unknownEFID), 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord EFID of INGR!\n";
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
             std::cout <<"Error: subrecord EFIT of INGR has invalid length ("
                       <<subLength<<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read EFIT's stuff
           in_File.read((char*) &(tempEffect.unknownEFITs[0]), 4);
           in_File.read((char*) &(tempEffect.unknownEFITs[1]), 4);
           in_File.read((char*) &(tempEffect.unknownEFITs[2]), 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord EFIT of INGR!\n";
             return false;
           }
           hasNonPushedEffect = true;
           break;
      case cCTDA:
           if (!hasNonPushedEffect)
           {
             std::cout << "Error while reading INGR: CTDA found, but there was no EFID/EFIT block.\n";
             return false;
           }
           //CTDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=32)
           {
             std::cout <<"Error: subrecord CTDA of INGR has invalid length ("
                       <<subLength<<" bytes). Should be 32 bytes!\n";
             return false;
           }
           //read CTDA's stuff
           in_File.read((char*) &(tempCTDA.content), 32);
           bytesRead += 32;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CTDA of INGR!\n";
             return false;
           }
           tempEffect.unknownCTDAs.push_back(tempCTDA);
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FULL, KSIZ, MODL, MODT, ENIT, EFID,"
                     << " or CTDA are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //check for left effect block
  if (hasNonPushedEffect)
  {
    //need to push
    effects.push_back(tempEffect);
    hasNonPushedEffect = false;
  }

  //checks
  if (effects.size()!=4)
  {
    std::cout << "Error while reading INGR: there aren't four effects!\n";
    return false;
  }//if effects
  //more checks
  if (!(hasReadFULL and hasReadDATA and hasReadENIT and (!modelPath.empty())  ))
  {
    std::cout << "Error while reading record INGR: At least one of the "
              << "subrecords FULL, DATA, ENIT or MODL is missing!\n";
    return false;
  }

  return in_File.good();
}

int32_t IngredientRecord::getRecordType() const
{
  return cINGR;
}

} //namespace
