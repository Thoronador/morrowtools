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

#include "ScrollRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

ScrollRecord::ScrollRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  keywordArray(std::vector<uint32_t>()),
  menuDisplayObjectFormID(0),
  equipTypeFormID(0),
  description(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  value(0),
  weight(0.0f),
  effects(std::vector<EffectBlock>())
{
  memset(unknownOBND, 0, 12);
  memset(unknownSPIT, 0, 36);
}

ScrollRecord::~ScrollRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ScrollRecord::equals(const ScrollRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (name==other.name)
      and (menuDisplayObjectFormID==other.menuDisplayObjectFormID)
      and (keywordArray==other.keywordArray)
      and (equipTypeFormID==other.equipTypeFormID) and (description==other.description)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (value==other.value) and (weight==other.weight)
      and (memcmp(unknownSPIT, other.unknownSPIT, 36)==0)
      and (effects==other.effects));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ScrollRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */
        +4 /* MDOB */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */
        +4 /* ETYP */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */
        +description.getWriteSize() /* DESC */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +8 /* fixed length of 8 bytes */
        +4 /* SPIT */ +2 /* 2 bytes for length */ +36 /* fixed length of 36 bytes */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */
               +4 /* KWDA */ +2 /* 2 bytes for length */ +4*keywordArray.size(); /* fixed length of 4 bytes per elem.*/
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /*MODT*/ + 2 /* 2 bytes for length */
               + unknownMODT.size() /* size */;
  }
  if (!effects.empty())
  {
    unsigned int i;
    for (i=0; i<effects.size(); ++i)
    {
      writeSize = writeSize +effects[i].getWriteSize();
    }//for
  }//if effects
  return writeSize;
}

bool ScrollRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cSCRL, 4);
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
  //write OBND's stuff
  output.write((const char*) unknownOBND, 12);

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }//if FULL

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

  //write MDOB
  output.write((const char*) &cMDOB, 4);
  //MDOB's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write menu display object's form ID
  output.write((const char*) &menuDisplayObjectFormID, 4);

  //write ETYP
  output.write((const char*) &cETYP, 4);
  //ETYP's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write equip type's form ID
  output.write((const char*) &equipTypeFormID, 4);

  //write DESC
  if (!description.saveToStream(output, cDESC))
    return false;

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  subLength = modelPath.length()+1;
  output.write((const char*) &subLength, 2);
  //write model path
  output.write(modelPath.c_str(), subLength);

  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of SCRL!\n";
      return false;
    }
  }//if MODT

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 8; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) &value, 4);
  output.write((const char*) &weight, 4);

  //write SPIT
  output.write((const char*) &cSPIT, 4);
  //SPIT's length
  subLength = 36; //fixed
  output.write((const char*) &subLength, 2);
  //write SPIT's stuff
  output.write((const char*) unknownSPIT, 36);

  if (!effects.empty())
  {
    unsigned int i;
    for (i=0; i<effects.size(); ++i)
    {
      if (!effects[i].saveToStream(output))
      {
        std::cout << "Error while writing effect block of SCRL!\n";
        return false;
      }
    }//for i
  }//if effects

  return output.good();
}
#endif

bool ScrollRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cout <<"Error: sub record EDID of SCRL is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of SCRL!\n";
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
    std::cout <<"Error: subrecord OBND of SCRL has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of SCRL!\n";
    return false;
  }

  name.reset();
  keywordArray.clear();
  uint32_t tempKeyword, kwdaLength, i;
  bool hasReadMDOB = false;
  bool hasReadETYP = false;
  description.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  bool hasReadDATA = false;
  bool hasReadSPIT = false;
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
           if (name.isPresent())
           {
             std::cout << "Error: SCRL seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: SCRL seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //read KSIZ
           kwdaLength = 0;
           if (!loadUint32SubRecordFromStream(in_File, cKSIZ, kwdaLength, false)) return false;
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
             std::cout <<"Error: subrecord KWDA of SCRL has invalid length ("<<subLength
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
               std::cout << "Error while reading subrecord KWDA of SCRL!\n";
               return false;
             }
             keywordArray.push_back(tempKeyword);
           }//for
           break;
      case cMDOB:
           if (hasReadMDOB)
           {
             std::cout << "Error: SCRL seems to have more than one MDOB subrecord!\n";
             return false;
           }
           //read MDOB
           if (!loadUint32SubRecordFromStream(in_File, cMDOB, menuDisplayObjectFormID, false)) return false;
           bytesRead += 6;
           hasReadMDOB = true;
           break;
      case cETYP:
           if (hasReadETYP)
           {
             std::cout << "Error: SCRL seems to have more than one ETYP subrecord!\n";
             return false;
           }
           //read ETYP
           if (!loadUint32SubRecordFromStream(in_File, cETYP, equipTypeFormID, false)) return false;
           bytesRead += 6;
           hasReadETYP = true;
           break;
      case cDESC:
           if (description.isPresent())
           {
             std::cout << "Error: SCRL seems to have more than one DESC subrecord!\n";
             return false;
           }
           //read DESC
           if (!description.loadFromStream(in_File, cDESC, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: SCRL seems to have more than one MODL subrecord!\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of SCRL is longer than 511 characters!\n";
             return false;
           }
           //read MODL's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of SCRL!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: SCRL seems to have more than one MODT subrecord!\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading subrecord MODT of SCRL!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODT.size();
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: SCRL seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: subrecord DATA of SCRL has invalid length ("
                       <<subLength<<" bytes). Should be 8 bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &value, 4);
           in_File.read((char*) &weight, 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of SCRL!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cSPIT:
           if (hasReadSPIT)
           {
             std::cout << "Error: SCRL seems to have more than one SPIT subrecord!\n";
             return false;
           }
           //SPIT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=36)
           {
             std::cout <<"Error: subrecord SPIT of SCRL has invalid length ("
                       <<subLength<<" bytes). Should be 36 bytes!\n";
             return false;
           }
           //read SPIT's stuff
           in_File.read((char*) unknownSPIT, 36);
           bytesRead += 36;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SPIT of SCRL!\n";
             return false;
           }
           hasReadSPIT = true;
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
             std::cout <<"Error: subrecord EFID of SCRL has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read EFID's stuff
           in_File.read((char*) &(tempEffect.effectFormID), 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord EFID of SCRL!\n";
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
             std::cout <<"Error: subrecord EFIT of SCRL has invalid length ("
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
             std::cout << "Error while reading subrecord EFIT of SCRL!\n";
             return false;
           }
           hasNonPushedEffect = true;
           break;
      case cCTDA:
           if (!hasNonPushedEffect)
           {
             std::cout << "Error while reading SCRL: CTDA found, but there was no EFID/EFIT block.\n";
             return false;
           }
           //CTDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=32)
           {
             std::cout <<"Error: subrecord CTDA of SCRL has invalid length ("
                       <<subLength<<" bytes). Should be 32 bytes!\n";
             return false;
           }
           //read CTDA's stuff
           in_File.read((char*) &(tempCTDA.content), 32);
           bytesRead += 32;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CTDA of SCRL!\n";
             return false;
           }
           tempEffect.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
           break;
      case cCIS2:
           if (!hasNonPushedEffect)
           {
             std::cout << "Error while reading SCRL: CIS2 found, but there was no EFID/EFIT block!\n";
             return false;
           }
           if (tempEffect.unknownCTDA_CIS2s.empty())
           {
             std::cout << "Error while reading SCRL: CIS2 found, but there was no CTDA before it!\n";
             return false;
           }
           if (!tempEffect.unknownCTDA_CIS2s.back().unknownCISx.empty())
           {
             std::cout << "Error: SCRL seems to have more than one CIS2 subrecord per CTDA!\n";
             return false;
           }
           //read CIS2
           if (!loadString512FromStream(in_File, tempEffect.unknownCTDA_CIS2s.back().unknownCISx, buffer, cCIS2, false, bytesRead))
             return false;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FULL, MDOB, ETYP, DESC, SPIT, EFID,"
                     << " CTDA or CIS2 are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //check possibly not yet pushed effect block
  if (hasNonPushedEffect)
  {
    effects.push_back(tempEffect);
  }

  //check presence
  if (!(hasReadMDOB and hasReadETYP and description.isPresent() and (!modelPath.empty()) and hasReadDATA and hasReadSPIT))
  {
    std::cout << "Error while reading record SCRL: at least one of the "
              << "subrecords MDOB, ETYP, DESC, MODL, DATA or SPIT is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t ScrollRecord::getRecordType() const
{
  return cSCRL;
}

} //namespace
