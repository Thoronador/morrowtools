/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "MagicEffects.h"
#include <iostream>
#include "MW_Constants.h"
#include "HelperIO.h"


bool MGEF_Data::equals(const MGEF_Data& other) const
{
  return ((SpellSchool==other.SpellSchool) and (BaseCost==other.BaseCost)
      and (Flags==other.Flags) and (Red==other.Red) and (Green==other.Green)
      and (Blue==other.Blue) and (SpeedX==other.SpeedX) and (SizeX==other.SizeX)
      and (SizeCap==other.SizeCap) and (EffectIcon==other.EffectIcon)
      and(ParticleTexture==other.ParticleTexture) and (CastingVisual==other.CastingVisual)
      and (BoltVisual==other.BoltVisual) and (HitVisual==other.HitVisual)
      and (AreaVisual==other.AreaVisual) and (Description==other.Description)
      and (CastSound==other.CastSound) and (BoltSound==other.BoltSound)
      and (HitSound==other.HitSound) and (AreaSound==other.AreaSound));
}

MagicEffects::MagicEffects()
{
  //empty
}

MagicEffects::~MagicEffects()
{
  m_Effects.clear();
}

MagicEffects& MagicEffects::getSingleton()
{
  static MagicEffects Instance;
  return Instance;
}

void MagicEffects::addEffect(const int32_t Index, const MGEF_Data& Data)
{
  m_Effects[Index] = Data;
}

bool MagicEffects::hasEffect(const int32_t Index) const
{
  return m_Effects.find(Index)!=m_Effects.end();
}

unsigned int MagicEffects::getNumberOfEffects() const
{
  return m_Effects.size();
}

const MGEF_Data& MagicEffects::getEffect(const int32_t Index) const
{
  std::map<int, MGEF_Data>::const_iterator iter;
  iter = m_Effects.find(Index);
  if (iter!=m_Effects.end())
  {
    return iter->second;
  }
  //nothing found
  std::cout << "No magic effect with index "<<Index<<" found!\n";
  throw 1;
}

std::string MagicEffects::getSettingNameForEffect(const int32_t Index)
{
  if ((Index<0) or (Index>136))
  {
    std::cout << "Error: No setting defined for effect "<<Index<<"!\n";
    throw 42;
  }

  switch (Index)
  {
    case EffectAbsorbAttribute:
         return "sEffectAbsorbAttribute";
         break;
    case EffectAbsorbFatigue:
         return "sEffectAbsorbSkill";
         break;
    case EffectAbsorbHealth:
         return "sEffectAbsorbHealth";
         break;
    case EffectAbsorbSkill:
         return "sEffectAbsorbSkill";
         break;
    case EffectAbsorbSpellPoints:
         return "sEffectAbsorbSpellPoints";
         break;
    case EffectAlmsiviIntervention:
         return "sEffectAlmsiviIntervention";
         break;
    case EffectBlind:
         return "sEffectBlind";
         break;
    case EffectBoundBattleAxe:
         return "sEffectBoundBattleAxe";
         break;
    case EffectBoundBoots:
         return "sEffectBoundBoots";
         break;
    case EffectBoundCuirass:
         return "sEffectBoundCuirass";
         break;
    case EffectBoundDagger:
         return "sEffectBoundDagger";
         break;
    case EffectBoundGloves:
         return "sEffectBoundGloves";
         break;
    case EffectBoundHelm:
         return "sEffectBoundHelm";
         break;
    case EffectBoundLongbow:
         return "sEffectBoundLongbow";
         break;
    case EffectBoundLongsword:
         return "sEffectBoundLongsword";
         break;
    case EffectBoundMace:
         return "sEffectBoundMace";
         break;
    case EffectBoundShield:
         return "sEffectBoundShield";
         break;
    case EffectBoundSpear:
         return "sEffectBoundSpear";
         break;
    case EffectBurden:
         return "sEffectBurden";
         break;
    case EffectCalmCreature:
         return "sEffectCalmCreature";
         break;
    case EffectCalmHumanoid:
         return "sEffectCalmHumanoid";
         break;
    case EffectChameleon:
         return "sEffectChameleon";
         break;
    case EffectCharm:
         return "sEffectCharm";
         break;
    case EffectCommandCreatures:
         return "sEffectCommandCreatures";
         break;
    case EffectCommandHumanoids:
         return "sEffectCommandHumanoids";
         break;
    case EffectCorprus:
         return "sEffectCorpus";
         break;
    case EffectCureBlightDisease:
         return "sEffectCureBlightDisease";
         break;
    case EffectCureCommonDisease:
         return "sEffectCureCommonDisease";
         break;
    case EffectCureCorprusDisease:
         return "sEffectCureCorprusDisease";
         break;
    case EffectCureParalyzation:
         return "sEffectCureParalyzation";
         break;
    case EffectCurePoison:
         return "sEffectCurePoison";
         break;
    case EffectDamageAttribute:
         return "sEffectDamageAttribute";
         break;
    case EffectDamageFatigue:
         return "sEffectDamageFatigue";
         break;
    case EffectDamageHealth:
         return "sEffectDamageHealth";
         break;
    case EffectDamageMagicka:
         return "sEffectDamageMagicka";
         break;
    case EffectDamageSkill:
         return "sEffectDamageSkill";
         break;
    case EffectDemoralizeCreature:
         return "sEffectDemoralizeCreature";
         break;
    case EffectDemoralizeHumanoid:
         return "sEffectDemoralizeHumanoid";
         break;
    case EffectDetectAnimal:
         return "sEffectDetectAnimal";
         break;
    case EffectDetectEnchantment:
         return "sEffectDetectEnchantment";
         break;
    case EffectDetectKey:
         return "sEffectDetectKey";
         break;
    case EffectDisintegrateWeapon:
         return "sEffectDisintegrateWeapon";
         break;
    case EffectDisintegrateArmor:
         return "sEffectDisintegrateArmor";
         break;
    case EffectDispel:
         return "sEffectDispel";
         break;
    case EffectDivineIntervention:
         return "sEffectDivineIntervention";
         break;
    case EffectDrainAttribute:
         return "sEffectDrainAttribute";
         break;
    case EffectDrainFatigue:
         return "sEffectDrainFatigue";
         break;
    case EffectDrainHealth:
         return "sEffectDrainHealth";
         break;
    case EffectDrainSkill:
         return "sEffectDrainSkill";
         break;
    case EffectDrainSpellpoints:
         return "sEffectDrainSpellpoints";
         break;
    case EffectExtraSpell:
         return "sEffectExtraSpell";
         break;
    case EffectFeather:
         return "sEffectFeather";
         break;
    case EffectFireDamage:
         return "sEffectFireDamage";
         break;
    case EffectFireShield:
         return "sEffectFireShield";
         break;
    case EffectFortifyAttackBonus:
         return "sEffectFortifyAttackBonus";
         break;
    case EffectFortifyAttribute:
         return "sEffectFortifyAttribute";
         break;
    case EffectFortifyFatigue:
         return "sEffectFortifyFatigue";
         break;
    case EffectFortifyHealth:
         return "sEffectFortifyHealth";
         break;
    case EffectFortifyMagickaMultiplier:
         return "sEffectFortifyMagickaMultiplier";
         break;
    case EffectFortifySkill:
         return "sEffectFortifySkill";
         break;
    case EffectFortifySpellpoints:
         return "sEffectFortifySpellpoints";
         break;
    case EffectFrenzyCreature:
         return "sEffectFrenzyCreature";
         break;
    case EffectFrenzyHumanoid:
         return "sEffectFrenzyHumanoid";
         break;
    case EffectFrostDamage:
         return "sEffectFrostDamage";
         break;
    case EffectFrostShield:
         return "sEffectFrostShield";
         break;
    case EffectInvisibility:
         return "sEffectInvisibility";
         break;
    case EffectJump:
         return "sEffectJump";
         break;
    case EffectLevitate:
         return "sEffectLevitate";
         break;
    case EffectLight:
         return "sEffectLight";
         break;
    case EffectLightningShield:
         return "sEffectLightningShield";
         break;
    case EffectLock:
         return "sEffectLock";
         break;
    case EffectMark:
         return "sEffectMark";
         break;
    case EffectNightEye:
         return "sEffectNightEye";
         break;
    case EffectOpen:
         return "sEffectOpen";
         break;
    case EffectParalyze:
         return "sEffectParalyze";
         break;
    case EffectPoison:
         return "sEffectPoison";
         break;
    case EffectRallyCreature:
         return "sEffectRallyCreature";
         break;
    case EffectRallyHumanoid:
         return "sEffectRallyHumanoid";
         break;
    case EffectRecall:
         return "sEffectRecall";
         break;
    case EffectReflect:
         return "sEffectReflect";
         break;
    case EffectRemoveCurse:
         return "sEffectRemoveCurse";
         break;
    case EffectResistBlightDisease:
         return "sEffectResistBlightDisease";
         break;
    case EffectResistCommonDisease:
         return "sEffectResistCommonDisease";
         break;
    case EffectResistCorprusDisease:
         return "sEffectResistCorprusDisease";
         break;
    case EffectResistFire:
         return "sEffectResistFire";
         break;
    case EffectResistFrost:
         return "sEffectResistFrost";
         break;
    case EffectResistMagicka:
         return "sEffectResistMagicka";
         break;
    case EffectResistNormalWeapons:
         return "sEffectResistNormalWeapons";
         break;
    case EffectResistParalysis:
         return "sEffectResistParalysis";
         break;
    case EffectResistPoison:
         return "sEffectResistPoison";
         break;
    case EffectResistShock:
         return "sEffectResistShock";
         break;
    case EffectRestoreAttribute:
         return "sEffectRestoreAttribute";
         break;
    case EffectRestoreFatigue:
         return "sEffectRestoreFatigue";
         break;
    case EffectRestoreHealth:
         return "sEffectRestoreHealth";
         break;
    case EffectRestoreSkill:
         return "sEffectRestoreSkill";
         break;
    case EffectRestoreSpellPoints:
         return "sEffectRestoreSpellPoints";
         break;
    case EffectSanctuary:
         return "sEffectSanctuary";
         break;
    case EffectShield:
         return "sEffectShield";
         break;
    case EffectShockDamage:
         return "sEffectShockDamage";
         break;
    case EffectSilence:
         return "sEffectSilence";
         break;
    case EffectSlowFall:
         return "sEffectSlowFall";
         break;
    case EffectSoultrap:
         return "sEffectSoultrap";
         break;
    case EffectSound:
         return "sEffectSound";
         break;
    case EffectSpellAbsorption:
         return "sEffectSpellAbsorption";
         break;
    case EffectStuntedMagicka:
         return "sEffectStuntedMagicka";
         break;
    case EffectSummonAncestralGhost:
         return "sEffectSummonAncestralGhost";
         break;
    case EffectSummonBonelord:
         return "sEffectSummonBonelord";
         break;
    case EffectSummonCenturionSphere:
         return "sEffectSummonCenturionSphere";
         break;
    case EffectSummonClannfear:
         return "sEffectSummonClannfear";
         break;
    case EffectSummonDaedroth:
         return "sEffectSummonDaedroth";
         break;
    case EffectSummonDremora:
         return "sEffectSummonDremora";
         break;
    case EffectSummonFlameAtronach:
         return "sEffectSummonFlameAtronach";
         break;
    case EffectSummonFrostAtronach:
         return "sEffectSummonFrostAtronach";
         break;
    case EffectSummonGoldenSaint:
         return "sEffectSummonGoldensaint";
         break;
    case EffectSummonGreaterBonewalker:
         return "sEffectSummonGreaterBonewalker";
         break;
    case EffectSummonHunger:
         return "sEffectSummonHunger";
         break;
    case EffectSummonLeastBonewalker:
         return "sEffectSummonLeastBonewalker";
         break;
    case EffectSummonScamp:
         return "sEffectSummonScamp";
         break;
    case EffectSummonSkeletalMinion:
         return "sEffectSummonSkeletalMinion";
         break;
    case EffectSummonStormAtronach:
         return "sEffectSummonStormAtronach";
         break;
    case EffectSummonWingedTwilight:
         return "sEffectSummonWingedTwilight";
         break;
    case EffectSunDamage:
         return "sEffectSunDamage";
         break;
    case EffectSwiftSwim:
         return "sEffectSwiftSwim";
         break;
    case EffectTelekinesis:
         return "sEffectTelekinesis";
         break;
    case EffectTurnUndead:
         return "sEffectTurnUndead";
         break;
    case EffectVampirism:
         return "sEffectVampirism";
         break;
    case EffectWaterBreathing:
         return "sEffectWaterBreathing";
         break;
    case EffectWaterWalking:
         return "sEffectWaterWalking";
         break;
    case EffectWeaknessToBlightDisease:
         return "sEffectWeaknessToBlightDisease";
         break;
    case EffectWeaknessToCommonDisease:
         return "sEffectWeaknessToCommonDisease";
         break;
    case EffectWeaknessToCorprusDisease:
         return "sEffectWeaknessToCorprusDisease";
         break;
    case EffectWeaknessToFire:
         return "sEffectWeaknessToFire";
         break;
    case EffectWeaknessToFrost:
         return "sEffectWeaknessToFrost";
         break;
    case EffectWeaknessToMagicka:
         return "sEffectWeaknessToMagicka";
         break;
    case EffectWeaknessToNormalWeapons:
         return "sEffectWeaknessToNormalWeapons";
         break;
    case EffectWeaknessToPoison:
         return "sEffectWeaknessToPoison";
         break;
    case EffectWeaknessToShock:
         return "sEffectWeaknessToFire";
         break;
  }//swi
  std::cout << "Error: No setting definded for effect "<<Index<<"!\n";
  throw 42;
}

bool MagicEffects::isSkillRelatedEffect(const int32_t Index)
{
  return ((Index==EffectDrainSkill) or (Index==EffectDamageSkill)
     or (Index==EffectRestoreSkill) or (Index==EffectFortifySkill)
     or (Index==EffectAbsorbSkill));
}

bool MagicEffects::isAttributeRelatedEffect(const int32_t Index)
{
  return ((Index==EffectDrainAttribute) or (Index==EffectDamageAttribute)
       or (Index==EffectRestoreAttribute) or (Index==EffectFortifyAttribute)
       or (Index==EffectAbsorbAttribute));
}

bool MagicEffects::readMGEF(std::ifstream& in_File)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Magic effects:
    INDX = The Effect ID (4 bytes, long)
    MEDT = Effect Data (36 bytes)
        long  SpellSchool (0 = Alteration,1 = Conjuration,2 = Destruction,
                           3 = Illusion,4 = Mysticism,5 = Restoration)
        float BaseCost
        long  Flags (0x0200 = Spellmaking,0x0400 = Enchanting,0x0800 = Negative)
        long  Red
        long  Blue
        long  Green
        float SpeedX
        float SizeX
        float SizeCap
    ITEX = Effect Icon string
    PTEX = Particle texture string
    CVFX = Casting visual string (optional?)
    BVFX = Bolt visual string (optional?)
    HVFX = Hit visual string (optional?)
    AVFX = Area visual string (optional?)
    DESC = Description text (optional?
    CSND = Cast sound (optional)
    BSND = Bolt sound (optional)
    HSND = Hit sound (optional)
    ASND = Area sound (optional)
      Note (Thoronador):
        sequence of the (at least) last four optional sub records can vary.*/

  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read INDX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cINDX)
  {
    UnexpectedRecord(cINDX, SubRecName);
    return false;
  }
  //INDX's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout << "Error: sub record INDX of MGEF has invalid length ("
              <<SubLength<< " bytes). Should be 4 bytes.\n";
    return false;
  }

  int32_t temp_Index = -1;
  in_File.read((char*) &temp_Index, 4); //read the index

  //read MEDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMEDT)
  {
    UnexpectedRecord(cMEDT, SubRecName);
    return false;
  }
  //MEDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=36)
  {
    std::cout << "Error: sub record MEDT of MGEF has invalid length ("
              <<SubLength<< " bytes). Should be 36 bytes.\n";
    return false;
  }

  MGEF_Data tempData;

  //SpellSchool
  in_File.read((char*) &(tempData.SpellSchool), 4);
  //Cost
  in_File.read((char*) &(tempData.BaseCost), 4);
  //Flags
  in_File.read((char*) &(tempData.Flags), 4);
  //Colour
  in_File.read((char*) &(tempData.Red), 4);
  in_File.read((char*) &(tempData.Blue), 4);
  in_File.read((char*) &(tempData.Green), 4);
  //speed
  in_File.read((char*) &(tempData.SpeedX), 4);
  //size
  in_File.read((char*) &(tempData.SizeX), 4);
  //size cap
  in_File.read((char*) &(tempData.SizeCap), 4);


  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  char Buffer [1024];
  memset(Buffer, '\0', 1024);
  //read tex name
  in_File.read(Buffer, SubLength);
  tempData.EffectIcon = std::string(Buffer);

  //read PTEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cPTEX)
  {
    UnexpectedRecord(cPTEX, SubRecName);
    return false;
  }
  //PTEX's length
  in_File.read((char*) &SubLength, 4);
  //read particle tex name
  memset(Buffer, '\0', 1024);
  in_File.read(Buffer, SubLength);
  tempData.ParticleTexture = std::string(Buffer);

  //read next subrecord
  in_File.read((char*) &SubRecName, 4);

  //visual strings, partially optional
  tempData.CastingVisual = "";
  tempData.BoltVisual = "";
  tempData.HitVisual = "";
  tempData.AreaVisual = "";
  //description, optional
  tempData.Description = "";
  //effect sounds, all optional
  tempData.CastSound = "";
  tempData.BoltSound = "";
  tempData.HitSound = "";
  tempData.AreaSound = "";
  bool go_on_with_subs = true;
  do
  {
    switch(SubRecName)
    {
      case cCVFX: //read optional CVFX
           //CVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.CastingVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cBVFX: //read optional BVFX
           //BVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.BoltVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cHVFX: //read optional HVFX
           //HVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.HitVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cAVFX: //read optional AVFX
           //AVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.AreaVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cDESC: //read optional DESC
           //DESC's length
           in_File.read((char*) &SubLength, 4);
           //read effect description
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.Description = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cCSND:
           //CSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.CastSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cBSND:
           //BSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.BoltSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cHSND:
           //HSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.HitSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cASND:
           //ASND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.AreaSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      default:
           //unknown sub record, thus stop here, it's beginning of new record
           go_on_with_subs = false;
    }//switch
  } while (go_on_with_subs);

  //seek four bytes into the direction if the beginning to land before the
  // name of the next record
  in_File.seekg(-4, std::ios::cur);
  if (in_File.good())
  {
    MagicEffects::getSingleton().addEffect(temp_Index, tempData);
    return true;
  }
  return false;
}//readMGEF

int MagicEffects::readRecordMGEF(std::ifstream& in_File)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Magic effects:
    INDX = The Effect ID (4 bytes, long)
    MEDT = Effect Data (36 bytes)
        long  SpellSchool (0 = Alteration,1 = Conjuration,2 = Destruction,
                           3 = Illusion,4 = Mysticism,5 = Restoration)
        float BaseCost
        long  Flags (0x0200 = Spellmaking,0x0400 = Enchanting,0x0800 = Negative)
        long  Red
        long  Blue
        long  Green
        float SpeedX
        float SizeX
        float SizeCap
    ITEX = Effect Icon string
    PTEX = Particle texture string
    CVFX = Casting visual string (optional?)
    BVFX = Bolt visual string (optional?)
    HVFX = Hit visual string (optional?)
    AVFX = Area visual string (optional?)
    DESC = Description text (optional?
    CSND = Cast sound (optional)
    BSND = Bolt sound (optional)
    HSND = Hit sound (optional)
    ASND = Area sound (optional)
      Note (Thoronador):
        sequence of the (at least) last four optional sub records can vary.*/

  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read INDX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cINDX)
  {
    UnexpectedRecord(cINDX, SubRecName);
    return -1;
  }
  //INDX's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout << "Error: sub record INDX of MGEF has invalid length ("
              <<SubLength<< " bytes). Should be 4 bytes.\n";
    return -1;
  }

  int32_t temp_Index = -1;
  in_File.read((char*) &temp_Index, 4); //read the index

  //read MEDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMEDT)
  {
    UnexpectedRecord(cMEDT, SubRecName);
    return -1;
  }
  //MEDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=36)
  {
    std::cout << "Error: sub record MEDT of MGEF has invalid length ("
              <<SubLength<< " bytes). Should be 36 bytes.\n";
    return -1;
  }

  MGEF_Data tempData;

  //SpellSchool
  in_File.read((char*) &(tempData.SpellSchool), 4);
  //Cost
  in_File.read((char*) &(tempData.BaseCost), 4);
  //Flags
  in_File.read((char*) &(tempData.Flags), 4);
  //Colour
  in_File.read((char*) &(tempData.Red), 4);
  in_File.read((char*) &(tempData.Blue), 4);
  in_File.read((char*) &(tempData.Green), 4);
  //speed
  in_File.read((char*) &(tempData.SpeedX), 4);
  //size
  in_File.read((char*) &(tempData.SizeX), 4);
  //size cap
  in_File.read((char*) &(tempData.SizeCap), 4);

  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MEDT of MGEF.\n";
    return -1;
  }

  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return -1;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  char Buffer [1024];
  memset(Buffer, '\0', 1024);
  //read tex name
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord ITEX of MGEF.\n";
    return -1;
  }
  tempData.EffectIcon = std::string(Buffer);

  //read PTEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cPTEX)
  {
    UnexpectedRecord(cPTEX, SubRecName);
    return -1;
  }
  //PTEX's length
  in_File.read((char*) &SubLength, 4);
  //read particle tex name
  memset(Buffer, '\0', 1024);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord PTEX of MGEF.\n";
    return -1;
  }
  tempData.ParticleTexture = std::string(Buffer);

  //read next subrecord
  in_File.read((char*) &SubRecName, 4);

  //visual strings, partially optional
  tempData.CastingVisual = "";
  tempData.BoltVisual = "";
  tempData.HitVisual = "";
  tempData.AreaVisual = "";
  //description, optional
  tempData.Description = "";
  //effect sounds, all optional
  tempData.CastSound = "";
  tempData.BoltSound = "";
  tempData.HitSound = "";
  tempData.AreaSound = "";
  bool go_on_with_subs = true;
  do
  {
    switch(SubRecName)
    {
      case cCVFX: //read optional CVFX
           //CVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.CastingVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cBVFX: //read optional BVFX
           //BVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.BoltVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cHVFX: //read optional HVFX
           //HVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.HitVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cAVFX: //read optional AVFX
           //AVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.AreaVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cDESC: //read optional DESC
           //DESC's length
           in_File.read((char*) &SubLength, 4);
           //read effect description
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.Description = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cCSND:
           //CSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.CastSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cBSND:
           //BSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.BoltSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cHSND:
           //HSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.HitSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cASND:
           //ASND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.AreaSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      default:
           //unknown sub record, thus stop here, it's beginning of new record
           go_on_with_subs = false;
    }//switch
  } while (go_on_with_subs);

  //seek four bytes into the direction if the beginning to land before the
  // name of the next record
  in_File.seekg(-4, std::ios::cur);
  if (in_File.good())
  {
    if (hasEffect(temp_Index))
    {
      //Are the contents the same?
      if (getEffect(temp_Index).equals(tempData))
      {
        return 0; //same content, nothing changed here
      }
    }//if
    //no effect with that index present or different content, so just add it
    addEffect(temp_Index, tempData);
    return 1;
  }//read operation good
  return -1;
}//readRecordMGEF (integer version of readMGEF)

EffectListIterator MagicEffects::getBegin() const
{
  return m_Effects.begin();
}

EffectListIterator MagicEffects::getEnd() const
{
  return m_Effects.end();
}

void MagicEffects::clearAll()
{
  m_Effects.clear();
}
