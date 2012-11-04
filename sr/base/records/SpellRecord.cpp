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

#include "SpellRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

//flag constants
const uint32_t SpellRecord::cFlagNoAutoCalc              = 0x00000001;
const uint32_t SpellRecord::cFlagPCStartSpell            = 0x00020000;
const uint32_t SpellRecord::cFlagAreaEffectIgnoresLOS    = 0x00080000;
const uint32_t SpellRecord::cFlagIgnoreResistance        = 0x00100000;
const uint32_t SpellRecord::cFlagDisallowAbsorbReflect   = 0x00200000;
const uint32_t SpellRecord::cFlagNoDualCastModifications = 0x00800000;

//type constants
const uint32_t SpellRecord::cSpell       = 0x00000000;
const uint32_t SpellRecord::cDisease     = 0x00000001;
const uint32_t SpellRecord::cPower       = 0x00000002;
const uint32_t SpellRecord::cLesserPower = 0x00000003;
const uint32_t SpellRecord::cAbility     = 0x00000004;
const uint32_t SpellRecord::cPoison      = 0x00000005;
const uint32_t SpellRecord::cAddiction   = 0x0000000A;
const uint32_t SpellRecord::cVoicePower  = 0x0000000B;

//casting type constants
const uint32_t SpellRecord::cConstantEffect = 0x00000000;
const uint32_t SpellRecord::cFireAndForget  = 0x00000001;
const uint32_t SpellRecord::cConcentration  = 0x00000002;

// delivery constants
const uint32_t SpellRecord::cSelf           = 0x00000000;
const uint32_t SpellRecord::cContact        = 0x00000001;
const uint32_t SpellRecord::cAimed          = 0x00000002;
const uint32_t SpellRecord::cTargetActor    = 0x00000003;
const uint32_t SpellRecord::cTargetLocation = 0x00000004;


SpellRecord::SpellRecord()
: BasicRecord()
{
  editorID = "";
  memset(unknownOBND, 0, 12);
  hasFULL = false;
  nameStringID = 0;
  menuDisplayObjectFormID = 0;
  equipTypeFormID = 0;
  descriptionStringID = 0;
  //subrecord SPIT
  castingCost = 0;
  flags = 0;
  type = 0;
  chargeTime = 0.0f;
  castingType = 0;
  delivery = 0;
  castDuration = 0.0f;
  range = 0.0f;
  castingPerkFormID = 0;
  //end of SPIT
  effects.clear();
}

SpellRecord::~SpellRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool SpellRecord::equals(const SpellRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0) and (hasFULL==other.hasFULL)
      and ((nameStringID==other.nameStringID) or (!hasFULL))
      and (menuDisplayObjectFormID==other.menuDisplayObjectFormID)
      and (equipTypeFormID==other.equipTypeFormID) and (descriptionStringID==other.descriptionStringID)
      and (castingCost==other.castingCost) and (flags==other.flags)
      and (type==other.type) and (chargeTime==other.chargeTime)
      and (castingType==other.castingType) and (delivery==other.delivery)
      and (castDuration==other.castDuration) and (range==other.range)
      and (castingPerkFormID==other.castingPerkFormID)
      and (effects==other.effects));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t SpellRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */
        +4 /* ETYP */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */
        +4 /* DESC */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */
        +4 /* SPIT */ +2 /* 2 bytes for length */ +36 /* fixed length of 36 bytes */;
  if (hasFULL)
  {
    writeSize = writeSize +4 /*FULL*/ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }
  if (menuDisplayObjectFormID!=0)
  {
    writeSize = writeSize +4 /*MDOB*/ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }
  unsigned int i;
  if (!effects.empty())
  {
    for (i=0; i<effects.size(); ++i)
    {
      writeSize = writeSize +effects[i].getWriteSize();
    }//for
  }//if effects
  return writeSize;
}

bool SpellRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cSPEL, 4);
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

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write FULL's stuff
    output.write((const char*) &nameStringID, 4);
  }//if has FULL

  if (menuDisplayObjectFormID!=0)
  {
    //write MDOB
    output.write((const char*) &cMDOB, 4);
    //MDOB's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write menu display object's form ID
    output.write((const char*) &menuDisplayObjectFormID, 4);
  }//if has MDOB

  //write ETYP
  output.write((const char*) &cETYP, 4);
  //ETYP's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write equip type's form ID
  output.write((const char*) &equipTypeFormID, 4);

  //write DESC
  output.write((const char*) &cDESC, 4);
  //DESC's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write DESC's stuff
  output.write((const char*) &descriptionStringID, 4);

  //write SPIT
  output.write((const char*) &cSPIT, 4);
  //SPIT's length
  subLength = 36; //fixed
  output.write((const char*) &subLength, 2);
  //write SPIT's stuff
  output.write((const char*) &castingCost, 4);
  output.write((const char*) &flags, 4);
  output.write((const char*) &type, 4);
  output.write((const char*) &chargeTime, 4);
  output.write((const char*) &castingType, 4);
  output.write((const char*) &delivery, 4);
  output.write((const char*) &castDuration, 4);
  output.write((const char*) &range, 4);
  output.write((const char*) &castingPerkFormID, 4);

  if (!effects.empty())
  {
    unsigned int i;
    for (i=0; i<effects.size(); ++i)
    {
      if (!effects[i].saveToStream(output))
      {
        std::cout << "Error while writing effect block of SPEL!\n";
        return false;
      }
    }//for i
  }//if effects

  return output.good();
}
#endif

bool SpellRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of SPEL is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of SPEL!\n";
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
    std::cout <<"Error: subrecord OBND of SPEL has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of SPEL!\n";
    return false;
  }

  hasFULL = false; nameStringID = 0;
  menuDisplayObjectFormID = 0;
  bool hasReadETYP = false;
  bool hasReadDESC = false;
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
           if (hasFULL)
           {
             std::cout << "Error: SPEL seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, nameStringID, false)) return false;
           bytesRead += 6;
           hasFULL = true;
           break;
      case cMDOB:
           if (menuDisplayObjectFormID!=0)
           {
             std::cout << "Error: SPEL seems to have more than one MDOB subrecord!\n";
             return false;
           }
           //read MDOB
           if (!loadUint32SubRecordFromStream(in_File, cMDOB, menuDisplayObjectFormID, false)) return false;
           bytesRead += 6;
           if (menuDisplayObjectFormID==0)
           {
             std::cout << "Error: subrecord MDOB of SPEL has value zero!\n";
             return false;
           }
           break;
      case cETYP:
           if (hasReadETYP)
           {
             std::cout << "Error: SPEL seems to have more than one ETYP subrecord!\n";
             return false;
           }
           //read ETYP
           if (!loadUint32SubRecordFromStream(in_File, cETYP, equipTypeFormID, false)) return false;
           bytesRead += 6;
           hasReadETYP = true;
           break;
      case cDESC:
           if (hasReadDESC)
           {
             std::cout << "Error: SPEL seems to have more than one DESC subrecord!\n";
             return false;
           }
           //read DESC
           if (!loadUint32SubRecordFromStream(in_File, cDESC, descriptionStringID, false)) return false;
           bytesRead += 6;
           hasReadDESC = true;
           break;
      case cSPIT:
           if (hasReadSPIT)
           {
             std::cout << "Error: SPEL seems to have more than one SPIT subrecord!\n";
             return false;
           }
           //SPIT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=36)
           {
             std::cout <<"Error: subrecord SPIT of SPEL has invalid length ("
                       <<subLength<<" bytes). Should be 36 bytes!\n";
             return false;
           }
           //read SPIT's stuff
           in_File.read((char*) &castingCost, 4);
           in_File.read((char*) &flags, 4);
           in_File.read((char*) &type, 4);
           in_File.read((char*) &chargeTime, 4);
           in_File.read((char*) &castingType, 4);
           in_File.read((char*) &delivery, 4);
           in_File.read((char*) &castDuration, 4);
           in_File.read((char*) &range, 4);
           in_File.read((char*) &castingPerkFormID, 4);
           bytesRead += 36;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SPIT of SPEL!\n";
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
             std::cout <<"Error: subrecord EFID of SPEL has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read EFID's stuff
           in_File.read((char*) &(tempEffect.effectFormID), 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord EFID of SPEL!\n";
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
             std::cout <<"Error: subrecord EFIT of SPEL has invalid length ("
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
             std::cout << "Error while reading subrecord EFIT of SPEL!\n";
             return false;
           }
           hasNonPushedEffect = true;
           break;
      case cCTDA:
           if (!hasNonPushedEffect)
           {
             std::cout << "Error while reading SPEL: CTDA found, but there was no EFID/EFIT block.\n";
             return false;
           }
           //CTDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=32)
           {
             std::cout <<"Error: subrecord CTDA of SPEL has invalid length ("
                       <<subLength<<" bytes). Should be 32 bytes!\n";
             return false;
           }
           //read CTDA's stuff
           in_File.read((char*) &(tempCTDA.content), 32);
           bytesRead += 32;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CTDA of SPEL!\n";
             return false;
           }
           tempEffect.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
           break;
      case cCIS2:
           if (!hasNonPushedEffect)
           {
             std::cout << "Error while reading SPEL: CIS2 found, but there was no EFID/EFIT block!\n";
             return false;
           }
           if (tempEffect.unknownCTDA_CIS2s.empty())
           {
             std::cout << "Error while reading SPEL: CIS2 found, but there was no CTDA before it!\n";
             return false;
           }
           if (!tempEffect.unknownCTDA_CIS2s.back().unknownCIS2.empty())
           {
             std::cout << "Error: SPEL seems to have more than one CIS2 subrecord per CTDA!\n";
             return false;
           }
           //CIS2's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: subrecord CIS2 of SPEL is longer than 511 characters!\n";
             return false;
           }
           //read CIS2's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CIS2 of SPEL!\n";
             return false;
           }
           tempEffect.unknownCTDA_CIS2s.back().unknownCIS2 = std::string(buffer);
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
  if (!(hasReadETYP and hasReadDESC and hasReadSPIT))
  {
    std::cout << "Error while reading record SPEL: at least one of the "
              << "subrecords ETYP, DESC or SPIT is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t SpellRecord::getRecordType() const
{
  return cSPEL;
}

//flag evaluation functions
bool SpellRecord::doesAutoCalc() const
{
  return ((flags & cFlagNoAutoCalc)==0);
}

bool SpellRecord::isPCStartSpell() const
{
  return ((flags & cFlagPCStartSpell)!=0);
}

bool SpellRecord::areaEffectIgnoresLOS() const
{
  return ((flags & cFlagAreaEffectIgnoresLOS)!=0);
}

bool SpellRecord::ignoresResistance() const
{
  return ((flags & cFlagIgnoreResistance)!=0);
}

bool SpellRecord::disallowsAbsorbAndReflect() const
{
  return ((flags & cFlagDisallowAbsorbReflect)!=0);
}

bool SpellRecord::noDualCastModifications() const
{
  return ((flags & cFlagNoDualCastModifications)!=0);
}

} //namespace
