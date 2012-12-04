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

#include "MagicEffectRecord.h"
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"
#include <iostream>
#include <cstring>

namespace SRTP
{

/* flag constants */
const uint32_t MagicEffectRecord::cFlagHostile               = 0x00000001;
const uint32_t MagicEffectRecord::cFlagRecover               = 0x00000002;
const uint32_t MagicEffectRecord::cFlagDetrimetral           = 0x00000004;
const uint32_t MagicEffectRecord::cFlagNoHitEvent            = 0x00000010;
const uint32_t MagicEffectRecord::cFlagDispelEffects         = 0x00000100;
const uint32_t MagicEffectRecord::cFlagNoDuration            = 0x00000200;
const uint32_t MagicEffectRecord::cFlagNoMagnitude           = 0x00000400;
const uint32_t MagicEffectRecord::cFlagNoArea                = 0x00000800;
const uint32_t MagicEffectRecord::cFlagFXPersist             = 0x00001000;
const uint32_t MagicEffectRecord::cFlagGoryVisuals           = 0x00004000;
const uint32_t MagicEffectRecord::cFlagHideInUI              = 0x00008000;
const uint32_t MagicEffectRecord::cFlagNoRecast              = 0x00020000;
const uint32_t MagicEffectRecord::cFlagPowerAffectsMagnitude = 0x00200000;
const uint32_t MagicEffectRecord::cFlagPowerAffectsDuration  = 0x00400000;
const uint32_t MagicEffectRecord::cFlagPainless              = 0x04000000;
const uint32_t MagicEffectRecord::cFlagNoHitEffect           = 0x08000000;
const uint32_t MagicEffectRecord::cFlagNoDeathDispel         = 0x10000000;

/* magic skill constants */
const uint32_t MagicEffectRecord::cSkillAlteration  = 0x00000012;
const uint32_t MagicEffectRecord::cSkillConjuration = 0x00000013;
const uint32_t MagicEffectRecord::cSkillDestruction = 0x00000014;
const uint32_t MagicEffectRecord::cSkillIllusion    = 0x00000015;
const uint32_t MagicEffectRecord::cSkillRestoration = 0x00000016;
const uint32_t MagicEffectRecord::cSkillNone        = 0xFFFFFFFF;

/* casting type constants */
const uint32_t MagicEffectRecord::cConstantEffect = 0x00000000;
const uint32_t MagicEffectRecord::cFireAndForget  = 0x00000001;
const uint32_t MagicEffectRecord::cConcentration  = 0x00000002;

/* delivery constants */
const uint32_t MagicEffectRecord::cSelf           = 0x00000000;
const uint32_t MagicEffectRecord::cContact        = 0x00000001;
const uint32_t MagicEffectRecord::cAimed          = 0x00000002;
const uint32_t MagicEffectRecord::cTargetActor    = 0x00000003;
const uint32_t MagicEffectRecord::cTargetLocation = 0x00000004;

/* sound level constants */
const uint32_t MagicEffectRecord::cSoundLevelLoud     = 0x00000000;
const uint32_t MagicEffectRecord::cSoundLevelNormal   = 0x00000001;
const uint32_t MagicEffectRecord::cSoundLevelSilent   = 0x00000002;
const uint32_t MagicEffectRecord::cSoundLevelVeryLoud = 0x00000003;

/* sound type constants */
const uint32_t MagicEffectRecord::cSoundTypeDrawSheathe = 0x00000000;
const uint32_t MagicEffectRecord::cSoundTypeCharge      = 0x00000001;
const uint32_t MagicEffectRecord::cSoundTypeReady       = 0x00000002;
const uint32_t MagicEffectRecord::cSoundTypeRelease     = 0x00000003;
const uint32_t MagicEffectRecord::cSoundTypeCastLoop    = 0x00000004;
const uint32_t MagicEffectRecord::cSoundTypeOnHit       = 0x00000005;


MagicEffectRecord::MagicEffectRecord()
: BasicRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false);
  hasFULL = false;
  fullNameStringID = 0;
  menuDisplayObjectFormID = 0;
  keywordArray.clear();
  //DATA
  flags = 0;
  baseCost = 0.0f;
  unknownDATA03 = 0;
  magicSkill = cSkillNone;
  unknownDATA05 = 0;
  unknownDATA06 = 0;
  unknownDATA07 = 0;
  taperWeight = 0.0f;
  hitShaderFormID = 0;
  enchantShaderFormID = 0;
  skillLevel = 0;
  area = 0;
  castingTime = 0.0f;
  taperCurve = 0.0f;
  taperDuration = 0.0f;
  unknownDATA16 = 0;
  unknownDATA17 = 0;
  assocItem1 = 0;
  projectileFormID = 0;
  explosionFormID = 0;
  castingType = cFireAndForget;
  delivery = cSelf;
  unknownDATA23 = 0;
  castingArtFormID = 0;
  hitEffectArtFormID = 0;
  impactDataSetFormID = 0;
  skillUsageMult = 0.0f;
  dualCastingArt = 0;
  dualCastingScale = 0.0f;
  enchantArtFormID = 0;
  unknownDATA31 = 0;
  unknownDATA32 = 0;
  equipAbilityFormID = 0;
  imageSpaceModFormID = 0;
  perkFormID = 0;
  castingSoundLevel = 0;
  scriptEffectAIDataScore = 0.0f;
  scriptEffectAIDataDelayTime = 0.0f;
  //end of DATA
  sounds.clear();
  descriptionStringID = 0;
  unknownCTDAs.clear();
}

MagicEffectRecord::~MagicEffectRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool MagicEffectRecord::equals(const MagicEffectRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD) and (hasFULL==other.hasFULL)
      and ((fullNameStringID==other.fullNameStringID) or (!hasFULL))
      and (menuDisplayObjectFormID==other.menuDisplayObjectFormID)
      and (keywordArray==other.keywordArray)
      //subrecord DATA
      and (flags==other.flags) and (baseCost==other.baseCost)
      and (unknownDATA03==other.unknownDATA03) and (magicSkill==other.magicSkill)
      and (unknownDATA05==other.unknownDATA05) and (unknownDATA06==other.unknownDATA06)
      and (unknownDATA07==other.unknownDATA07) and (taperWeight==other.taperWeight)
      and (hitShaderFormID==other.hitShaderFormID) and (enchantShaderFormID==other.enchantShaderFormID)
      and (skillLevel==other.skillLevel) and (area==other.area)
      and (castingTime==other.castingTime) and (taperCurve==other.taperCurve)
      and (taperDuration==other.taperDuration) and (unknownDATA16==other.unknownDATA16)
      and (unknownDATA17==other.unknownDATA17) and (assocItem1==other.assocItem1)
      and (projectileFormID==other.projectileFormID) and (explosionFormID==other.explosionFormID)
      and (castingType==other.castingType) and (delivery==other.delivery)
      and (unknownDATA23==other.unknownDATA23) and (castingArtFormID==other.castingArtFormID)
      and (hitEffectArtFormID==other.hitEffectArtFormID) and (impactDataSetFormID==other.impactDataSetFormID)
      and (skillUsageMult==other.skillUsageMult) and (dualCastingArt==other.dualCastingArt)
      and (dualCastingScale==other.dualCastingScale) and (enchantArtFormID==other.enchantArtFormID)
      and (unknownDATA31==other.unknownDATA31) and (unknownDATA32==other.unknownDATA32)
      and (equipAbilityFormID==other.equipAbilityFormID) and (imageSpaceModFormID==other.imageSpaceModFormID)
      and (perkFormID==other.perkFormID) and (castingSoundLevel==other.castingSoundLevel)
      and (scriptEffectAIDataScore==other.scriptEffectAIDataScore) and (scriptEffectAIDataDelayTime==other.scriptEffectAIDataDelayTime)
      //end of subrecord DATA
      and (sounds==other.sounds)
      and (descriptionStringID==other.descriptionStringID) and (unknownCTDAs==other.unknownCTDAs));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t MagicEffectRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +152 /* fixed size */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +unknownCTDAs.size()*
        (4 /* CTDA */ +2 /* 2 bytes for length */ +32 /* fixed size */);
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /* VMAD */ +2 /* 2 bytes for length */ +unknownVMAD.getSize() /* size */;
  }
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (menuDisplayObjectFormID!=0)
  {
    writeSize = writeSize +4 /* MDOB */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +4 /* KWDA */ +2 /* 2 bytes for length */ +keywordArray.size()*4 /* n*fixed size */;
  }
  if (!sounds.empty())
  {
    writeSize = writeSize +4 /* SNDD */ +2 /* 2 bytes for length */ +8*sounds.size() /* size */;
  }
  return writeSize;
}

bool MagicEffectRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cMGEF, 4);
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
      std::cout << "Error while writing subrecord VMAD of MGEF!\n";
      return false;
    }
  }//if VMAD

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write FULL
    output.write((const char*) &fullNameStringID, 4);
  }//if has FULL subrecord

  if (menuDisplayObjectFormID!=0)
  {
    //write MDOB
    output.write((const char*) &cMDOB, 4);
    //MDOB's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write Menu Display Object
    output.write((const char*) &menuDisplayObjectFormID, 4);
  }//if MDOB present

  uint32_t i;
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
    for (i=0; i<k_Size; ++i)
    {
      output.write((const char*) &(keywordArray[i]), 4);
    }//for
  }//if keywords

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 152; //fixed size
  output.write((const char*) &subLength, 2);
  //write actual data
  output.write((const char*) &flags, 4);
  output.write((const char*) &baseCost, 4);
  output.write((const char*) &unknownDATA03, 4);
  output.write((const char*) &magicSkill, 4);
  output.write((const char*) &unknownDATA05, 4);
  output.write((const char*) &unknownDATA06, 4);
  output.write((const char*) &unknownDATA07, 4);
  output.write((const char*) &taperWeight, 4);
  output.write((const char*) &hitShaderFormID, 4);
  output.write((const char*) &enchantShaderFormID, 4);
  output.write((const char*) &skillLevel, 4);
  output.write((const char*) &area, 4);
  output.write((const char*) &castingTime, 4);
  output.write((const char*) &taperCurve, 4);
  output.write((const char*) &taperDuration, 4);
  output.write((const char*) &unknownDATA16, 4);
  output.write((const char*) &unknownDATA17, 4);
  output.write((const char*) &assocItem1, 4);
  output.write((const char*) &projectileFormID, 4);
  output.write((const char*) &explosionFormID, 4);
  output.write((const char*) &castingType, 4);
  output.write((const char*) &delivery, 4);
  output.write((const char*) &unknownDATA23, 4);
  output.write((const char*) &castingArtFormID, 4);
  output.write((const char*) &hitEffectArtFormID, 4);
  output.write((const char*) &impactDataSetFormID, 4);
  output.write((const char*) &skillUsageMult, 4);
  output.write((const char*) &dualCastingArt, 4);
  output.write((const char*) &dualCastingScale, 4);
  output.write((const char*) &enchantArtFormID, 4);
  output.write((const char*) &unknownDATA31, 4);
  output.write((const char*) &unknownDATA32, 4);
  output.write((const char*) &equipAbilityFormID, 4);
  output.write((const char*) &imageSpaceModFormID, 4);
  output.write((const char*) &perkFormID, 4);
  output.write((const char*) &castingSoundLevel, 4);
  output.write((const char*) &scriptEffectAIDataScore, 4);
  output.write((const char*) &scriptEffectAIDataDelayTime, 4);

  if (!sounds.empty())
  {
    //write SNDD
    output.write((const char*) &cSNDD, 4);
    //SNDD's length
    subLength = 8*sounds.size(); //eight bytes per entry
    output.write((const char*) &subLength, 2);
    //write sounds
    std::map<uint32_t, uint32_t>::const_iterator snddIter = sounds.begin();
    while (snddIter!=sounds.end())
    {
      output.write((const char*) &snddIter->first, 4);
      output.write((const char*) &snddIter->second, 4);
      ++snddIter;
    }//while
  }//if

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write DNAM
  output.write((const char*) &descriptionStringID, 4);

  //write CTDAs
  for (i=0; i<unknownCTDAs.size(); ++i)
  {
    //write CTDA
    output.write((const char*) &cCTDA, 4);
    //CTDA's length
    subLength = 32;
    output.write((const char*) &subLength, 2);
    //write CTDA
    output.write((const char*) (unknownCTDAs[i].content), 32);
  }//for

  return output.good();
}
#endif

bool MagicEffectRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of MGEF is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of MGEF!\n";
    return false;
  }
  editorID = std::string(buffer);

  unknownVMAD.setPresence(false);
  hasFULL = false;
  menuDisplayObjectFormID = 0;
  bool hasReadDATA = false;
  keywordArray.clear();
  uint32_t k_Size, i, helper;
  sounds.clear();
  uint32_t tempUint32;
  bool hasReadDNAM = false;
  unknownCTDAs.clear();
  CTDAData tempCTDA;

  while (bytesRead<readSize)
  {
    //read next subrecord header
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cout << "Error: MGEF seems to have more than one VMAD subrecord!\n";
             return false;
           }
           //read VMAD's stuff
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cout << "Error while reading subrecord VMAD of MGEF!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownVMAD.getSize();
           break;
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: MGEF seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, fullNameStringID, false)) return false;
           bytesRead += 6;
           hasFULL = true;
           break;
      case cMDOB:
           if (menuDisplayObjectFormID!=0)
           {
             std::cout << "Error: MGEF seems to have more than one MDOB subrecord!\n";
             return false;
           }
           //read MDOB
           if (!loadUint32SubRecordFromStream(in_File, cMDOB, menuDisplayObjectFormID, false)) return false;
           bytesRead += 6;
           if (menuDisplayObjectFormID==0)
           {
             std::cout << "Error: subrecord MDOB of MGEF has value zero!\n";
             return false;
           }
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: MGEF seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //KSIZ's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record KSIZ of MGEF has invalid length("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read KSIZ's stuff
           k_Size = 0;
           in_File.read((char*) &k_Size, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord KSIZ of MGEF!\n";
             return false;
           }

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
             std::cout <<"Error: sub record KWDA of MGEF has invalid length("
                       <<subLength<<" bytes). Should be "<<4*k_Size<<" bytes!\n";
             return false;
           }
           //read KWDA's stuff
           for (i=0; i<k_Size; ++i)
           {
             in_File.read((char*) &helper, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord KWDA of MGEF!\n";
               return false;
             }
             keywordArray.push_back(helper);
           }//for
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: MGEF seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=152)
           {
             std::cout << "Error: sub record DATA of MGEF has invalid length("
                       << subLength << " bytes). Should be 152 bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &flags, 4);
           in_File.read((char*) &baseCost, 4);
           in_File.read((char*) &unknownDATA03, 4);
           in_File.read((char*) &magicSkill, 4);
           in_File.read((char*) &unknownDATA05, 4);
           in_File.read((char*) &unknownDATA06, 4);
           in_File.read((char*) &unknownDATA07, 4);
           in_File.read((char*) &taperWeight, 4);
           in_File.read((char*) &hitShaderFormID, 4);
           in_File.read((char*) &enchantShaderFormID, 4);
           in_File.read((char*) &skillLevel, 4);
           in_File.read((char*) &area, 4);
           in_File.read((char*) &castingTime, 4);
           in_File.read((char*) &taperCurve, 4);
           in_File.read((char*) &taperDuration, 4);
           in_File.read((char*) &unknownDATA16, 4);
           in_File.read((char*) &unknownDATA17, 4);
           in_File.read((char*) &assocItem1, 4);
           in_File.read((char*) &projectileFormID, 4);
           in_File.read((char*) &explosionFormID, 4);
           in_File.read((char*) &castingType, 4);
           in_File.read((char*) &delivery, 4);
           in_File.read((char*) &unknownDATA23, 4);
           in_File.read((char*) &castingArtFormID, 4);
           in_File.read((char*) &hitEffectArtFormID, 4);
           in_File.read((char*) &impactDataSetFormID, 4);
           in_File.read((char*) &skillUsageMult, 4);
           in_File.read((char*) &dualCastingArt, 4);
           in_File.read((char*) &dualCastingScale, 4);
           in_File.read((char*) &enchantArtFormID, 4);
           in_File.read((char*) &unknownDATA31, 4);
           in_File.read((char*) &unknownDATA32, 4);
           in_File.read((char*) &equipAbilityFormID, 4);
           in_File.read((char*) &imageSpaceModFormID, 4);
           in_File.read((char*) &perkFormID, 4);
           in_File.read((char*) &castingSoundLevel, 4);
           in_File.read((char*) &scriptEffectAIDataScore, 4);
           in_File.read((char*) &scriptEffectAIDataDelayTime, 4);
           bytesRead += 152;
           //check length
           if (!in_File.good())
           {
             std::cout <<"Error while reading subrecord DATA of MGEF!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cSNDD:
           if (!sounds.empty())
           {
             std::cout << "Error: MGEF seems to have more than one SNDD subrecord!\n";
             return false;
           }
           //SNDD's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (((subLength%8)!=0) or (subLength==0))
           {
             std::cout <<"Error: sub record SNDD of MGEF has invalid length("
                       <<subLength<<" bytes). Should be an integral multiple of eight bytes!\n";
             return false;
           }
           //read SNDD's stuff
           k_Size = subLength / 8;
           for (i=0; i<k_Size; ++i)
           {
             in_File.read((char*) &helper, 4);
             in_File.read((char*) &tempUint32, 4);
             bytesRead += 8;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord SNDD of MGEF!\n";
               return false;
             }
             if (tempUint32==0)
             {
               std::cout << "Error while reading subrecord SNDD of MGEF: form ID is zero!\n";
               return false;
             }
             sounds[helper] = tempUint32;
           }//for
           //length check
           if (sounds.size()!=k_Size)
           {
             std::cout << "Error: subrecord SNDD of MGEF contains duplicate entries!\n";
             return false;
           }
           break;
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cout << "Error: MGEF seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record DNAM of MGEF has invalid length("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read DNAM's stuff
           in_File.read((char*) &descriptionStringID, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of MGEF!\n";
             return false;
           }
           hasReadDNAM = true;
           break;
      case cCTDA:
           //CTDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=32)
           {
             std::cout <<"Error: sub record CTDA of MGEF has invalid length("
                       <<subLength<<" bytes). Should be 32 bytes!\n";
             return false;
           }
           //read CTDA's stuff
           memset(tempCTDA.content, 0, 32);
           in_File.read((char*) tempCTDA.content, 32);
           bytesRead += 32;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CTDA of MGEF!\n";
             return false;
           }
           unknownCTDAs.push_back(tempCTDA);
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only VMAD, FULL, MDOB, KSIZ, SNDD, DATA or DNAM are allowed!\n";
           return false;
           break;
    }//swi
  }//while

  //check for required DNAM and DATA
  if (!(hasReadDNAM and hasReadDATA))
  {
    std::cout << "Error: DNAM or DATA subrecord of MGEF is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t MagicEffectRecord::getRecordType() const
{
  return cMGEF;
}

} //namespace
