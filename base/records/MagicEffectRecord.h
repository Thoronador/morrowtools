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

#ifndef MAGICEFFECTRECORD_H
#define MAGICEFFECTRECORD_H

#include <string>
#include <fstream>
#include "BasicRecord.h"

//spell schools
const int32_t cAlteration = 0;
const int32_t cConjuration = 1;
const int32_t cDestruction = 2;
const int32_t cIllusion = 3;
const int32_t cMysticism = 4;
const int32_t cRestoration = 5;

//flags
const int32_t FlagSpellmaking = 0x0200;
const int32_t FlagEnchanting = 0x0400;
const int32_t FlagNegative = 0x0800;

struct MGEF_Data: public BasicRecord
{
  int32_t Index;
  int32_t SpellSchool;
  float BaseCost;
  int32_t Flags;
  int32_t Red;
  int32_t Green;
  int32_t Blue;
  float SpeedX;
  float SizeX;
  float SizeCap;
  std::string EffectIcon;
  std::string ParticleTexture;
  std::string CastingVisual;
  std::string BoltVisual;
  std::string HitVisual;
  std::string AreaVisual;
  std::string Description;
  std::string CastSound; //optional
  std::string BoltSound; //optional
  std::string HitSound; //optional
  std::string AreaSound; //optional

  /* returns true, of content of other record is equal to this one */
  bool equals(const MGEF_Data& other) const;

  /* writes the record to the given output stream and returns true on success

    parameters:
        output - the output file stream
  */
  bool saveToStream(std::ofstream& output) const;

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input file stream
  */
  bool loadFromStream(std::ifstream& in_File);
}; //struct

//spell effect indices (hard-coded in MW)
const int32_t EffectWaterBreathing = 0;
const int32_t EffectSwiftSwim = 1;
const int32_t EffectWaterWalking = 2;
const int32_t EffectShield = 3;
const int32_t EffectFireShield = 4;
const int32_t EffectLightningShield = 5;
const int32_t EffectFrostShield = 6;
const int32_t EffectBurden = 7;
const int32_t EffectFeather = 8;
const int32_t EffectJump = 9;
const int32_t EffectLevitate = 10;
const int32_t EffectSlowFall = 11;
const int32_t EffectLock = 12;
const int32_t EffectOpen = 13;
const int32_t EffectFireDamage = 14;
const int32_t EffectShockDamage = 15;
const int32_t EffectFrostDamage = 16;
const int32_t EffectDrainAttribute = 17;
const int32_t EffectDrainHealth = 18;
const int32_t EffectDrainSpellpoints = 19;
const int32_t EffectDrainFatigue = 20;
const int32_t EffectDrainSkill = 21;
const int32_t EffectDamageAttribute = 22;
const int32_t EffectDamageHealth = 23;
const int32_t EffectDamageMagicka = 24;
const int32_t EffectDamageFatigue = 25;
const int32_t EffectDamageSkill = 26;
const int32_t EffectPoison = 27;
const int32_t EffectWeaknessToFire = 28;
const int32_t EffectWeaknessToFrost = 29;
const int32_t EffectWeaknessToShock = 30;
const int32_t EffectWeaknessToMagicka = 31;
const int32_t EffectWeaknessToCommonDisease = 32;
const int32_t EffectWeaknessToBlightDisease = 33;
const int32_t EffectWeaknessToCorprusDisease = 34;
const int32_t EffectWeaknessToPoison = 35;
const int32_t EffectWeaknessToNormalWeapons = 36;
const int32_t EffectDisintegrateWeapon = 37;
const int32_t EffectDisintegrateArmor = 38;
const int32_t EffectInvisibility = 39;
const int32_t EffectChameleon = 40;
const int32_t EffectLight = 41;
const int32_t EffectSanctuary = 42;
const int32_t EffectNightEye = 43;
const int32_t EffectCharm = 44;
const int32_t EffectParalyze = 45;
const int32_t EffectSilence = 46;
const int32_t EffectBlind = 47;
const int32_t EffectSound = 48;
const int32_t EffectCalmHumanoid = 49;
const int32_t EffectCalmCreature = 50;
const int32_t EffectFrenzyHumanoid = 51;
const int32_t EffectFrenzyCreature = 52;
const int32_t EffectDemoralizeHumanoid = 53;
const int32_t EffectDemoralizeCreature = 54;
const int32_t EffectRallyHumanoid = 55;
const int32_t EffectRallyCreature = 56;
const int32_t EffectDispel = 57;
const int32_t EffectSoultrap = 58;
const int32_t EffectTelekinesis = 59;
const int32_t EffectMark = 60;
const int32_t EffectRecall = 61;
const int32_t EffectDivineIntervention = 62;
const int32_t EffectAlmsiviIntervention = 63;
const int32_t EffectDetectAnimal = 64;
const int32_t EffectDetectEnchantment = 65;
const int32_t EffectDetectKey = 66;
const int32_t EffectSpellAbsorption = 67;
const int32_t EffectReflect = 68;
const int32_t EffectCureCommonDisease = 69;
const int32_t EffectCureBlightDisease = 70;
const int32_t EffectCureCorprusDisease = 71;
const int32_t EffectCurePoison = 72;
const int32_t EffectCureParalyzation = 73;
const int32_t EffectRestoreAttribute = 74;
const int32_t EffectRestoreHealth = 75;
const int32_t EffectRestoreSpellPoints = 76;
const int32_t EffectRestoreFatigue = 77;
const int32_t EffectRestoreSkill = 78;
const int32_t EffectFortifyAttribute = 79;
const int32_t EffectFortifyHealth = 80;
const int32_t EffectFortifySpellpoints = 81;
const int32_t EffectFortifyFatigue = 82;
const int32_t EffectFortifySkill = 83;
const int32_t EffectFortifyMagickaMultiplier = 84;
const int32_t EffectAbsorbAttribute = 85;
const int32_t EffectAbsorbHealth = 86;
const int32_t EffectAbsorbSpellPoints = 87;
const int32_t EffectAbsorbFatigue = 88;
const int32_t EffectAbsorbSkill = 89;
const int32_t EffectResistFire = 90;
const int32_t EffectResistFrost = 91;
const int32_t EffectResistShock = 92;
const int32_t EffectResistMagicka = 93;
const int32_t EffectResistCommonDisease = 94;
const int32_t EffectResistBlightDisease = 95;
const int32_t EffectResistCorprusDisease = 96;
const int32_t EffectResistPoison = 97;
const int32_t EffectResistNormalWeapons = 98;
const int32_t EffectResistParalysis = 99;
const int32_t EffectRemoveCurse = 100;
const int32_t EffectTurnUndead = 101;
const int32_t EffectSummonScamp = 102;
const int32_t EffectSummonClannfear = 103;
const int32_t EffectSummonDaedroth = 104;
const int32_t EffectSummonDremora = 105;
const int32_t EffectSummonAncestralGhost = 106;
const int32_t EffectSummonSkeletalMinion = 107;
const int32_t EffectSummonLeastBonewalker = 108;
const int32_t EffectSummonGreaterBonewalker = 109;
const int32_t EffectSummonBonelord = 110;
const int32_t EffectSummonWingedTwilight = 111;
const int32_t EffectSummonHunger = 112;
const int32_t EffectSummonGoldenSaint = 113;
const int32_t EffectSummonFlameAtronach = 114;
const int32_t EffectSummonFrostAtronach = 115;
const int32_t EffectSummonStormAtronach = 116;
const int32_t EffectFortifyAttackBonus = 117;
const int32_t EffectCommandCreatures = 118;
const int32_t EffectCommandHumanoids = 119;
const int32_t EffectBoundDagger = 120;
const int32_t EffectBoundLongsword = 121;
const int32_t EffectBoundMace = 122;
const int32_t EffectBoundBattleAxe = 123;
const int32_t EffectBoundSpear = 124;
const int32_t EffectBoundLongbow = 125;
const int32_t EffectExtraSpell = 126;
const int32_t EffectBoundCuirass = 127;
const int32_t EffectBoundHelm = 128;
const int32_t EffectBoundBoots = 129;
const int32_t EffectBoundShield = 130;
const int32_t EffectBoundGloves = 131;
const int32_t EffectCorprus = 132;
const int32_t EffectVampirism = 133;
const int32_t EffectSummonCenturionSphere = 134;
const int32_t EffectSunDamage = 135;
const int32_t EffectStuntedMagicka = 136;

#endif // MAGICEFFECTRECORD_H
