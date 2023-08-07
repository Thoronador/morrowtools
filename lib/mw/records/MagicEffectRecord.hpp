/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012, 2021  Dirk Stolle

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

#ifndef MW_MAGICEFFECTRECORD_HPP
#define MW_MAGICEFFECTRECORD_HPP

#include <string>
#include "BasicRecord.hpp"

namespace MWTP
{

// spell schools
enum class MagicSchool: uint32_t
{
  Alteration = 0,
  Conjuration = 1,
  Destruction = 2,
  Illusion = 3,
  Mysticism = 4,
  Restoration = 5
};

/// spell effect indices (hard-coded in MW executable)
enum class EffectIndex: uint32_t
{
  WaterBreathing = 0,
  SwiftSwim = 1,
  WaterWalking = 2,
  Shield = 3,
  FireShield = 4,
  LightningShield = 5,
  FrostShield = 6,
  Burden = 7,
  Feather = 8,
  Jump = 9,
  Levitate = 10,
  SlowFall = 11,
  Lock = 12,
  Open = 13,
  FireDamage = 14,
  ShockDamage = 15,
  FrostDamage = 16,
  DrainAttribute = 17,
  DrainHealth = 18,
  DrainSpellpoints = 19,
  DrainFatigue = 20,
  DrainSkill = 21,
  DamageAttribute = 22,
  DamageHealth = 23,
  DamageMagicka = 24,
  DamageFatigue = 25,
  DamageSkill = 26,
  Poison = 27,
  WeaknessToFire = 28,
  WeaknessToFrost = 29,
  WeaknessToShock = 30,
  WeaknessToMagicka = 31,
  WeaknessToCommonDisease = 32,
  WeaknessToBlightDisease = 33,
  WeaknessToCorprusDisease = 34,
  WeaknessToPoison = 35,
  WeaknessToNormalWeapons = 36,
  DisintegrateWeapon = 37,
  DisintegrateArmor = 38,
  Invisibility = 39,
  Chameleon = 40,
  Light = 41,
  Sanctuary = 42,
  NightEye = 43,
  Charm = 44,
  Paralyze = 45,
  Silence = 46,
  Blind = 47,
  Sound = 48,
  CalmHumanoid = 49,
  CalmCreature = 50,
  FrenzyHumanoid = 51,
  FrenzyCreature = 52,
  DemoralizeHumanoid = 53,
  DemoralizeCreature = 54,
  RallyHumanoid = 55,
  RallyCreature = 56,
  Dispel = 57,
  Soultrap = 58,
  Telekinesis = 59,
  Mark = 60,
  Recall = 61,
  DivineIntervention = 62,
  AlmsiviIntervention = 63,
  DetectAnimal = 64,
  DetectEnchantment = 65,
  DetectKey = 66,
  SpellAbsorption = 67,
  Reflect = 68,
  CureCommonDisease = 69,
  CureBlightDisease = 70,
  CureCorprusDisease = 71,
  CurePoison = 72,
  CureParalyzation = 73,
  RestoreAttribute = 74,
  RestoreHealth = 75,
  RestoreSpellPoints = 76,
  RestoreFatigue = 77,
  RestoreSkill = 78,
  FortifyAttribute = 79,
  FortifyHealth = 80,
  FortifySpellpoints = 81,
  FortifyFatigue = 82,
  FortifySkill = 83,
  FortifyMagickaMultiplier = 84,
  AbsorbAttribute = 85,
  AbsorbHealth = 86,
  AbsorbSpellPoints = 87,
  AbsorbFatigue = 88,
  AbsorbSkill = 89,
  ResistFire = 90,
  ResistFrost = 91,
  ResistShock = 92,
  ResistMagicka = 93,
  ResistCommonDisease = 94,
  ResistBlightDisease = 95,
  ResistCorprusDisease = 96,
  ResistPoison = 97,
  ResistNormalWeapons = 98,
  ResistParalysis = 99,
  RemoveCurse = 100,
  TurnUndead = 101,
  SummonScamp = 102,
  SummonClannfear = 103,
  SummonDaedroth = 104,
  SummonDremora = 105,
  SummonAncestralGhost = 106,
  SummonSkeletalMinion = 107,
  SummonLeastBonewalker = 108,
  SummonGreaterBonewalker = 109,
  SummonBonelord = 110,
  SummonWingedTwilight = 111,
  SummonHunger = 112,
  SummonGoldenSaint = 113,
  SummonFlameAtronach = 114,
  SummonFrostAtronach = 115,
  SummonStormAtronach = 116,
  FortifyAttackBonus = 117,
  CommandCreatures = 118,
  CommandHumanoids = 119,
  BoundDagger = 120,
  BoundLongsword = 121,
  BoundMace = 122,
  BoundBattleAxe = 123,
  BoundSpear = 124,
  BoundLongbow = 125,
  ExtraSpell = 126,
  BoundCuirass = 127,
  BoundHelm = 128,
  BoundBoots = 129,
  BoundShield = 130,
  BoundGloves = 131,
  Corprus = 132,
  Vampirism = 133,
  SummonCenturionSphere = 134,
  SunDamage = 135,
  StuntedMagicka = 136
};

// flags
const uint32_t FlagSpellmaking = 0x0200;
const uint32_t FlagEnchanting = 0x0400;
const uint32_t FlagNegative = 0x0800;

struct MagicEffectRecord: public BasicRecord
{
  EffectIndex Index;
  MagicSchool SpellSchool;
  float BaseCost;
  uint32_t Flags;
  uint32_t Red;
  uint32_t Green;
  uint32_t Blue;
  float SpeedX;
  float SizeX;
  float SizeCap;
  std::string EffectIcon;
  std::string ParticleTexture;
  std::string BoltSound; // optional
  std::string CastSound; // optional
  std::string HitSound; // optional
  std::string AreaSound; // optional
  std::string CastingVisual;
  std::string BoltVisual;
  std::string HitVisual;
  std::string AreaVisual;
  std::string Description;

  MagicEffectRecord();

  /** \brief Checks whether another instance contains the same data.
   *
   * \param other   the other record to compare with
   * \return Returns true, if @other contains the same data as this instance.
   *         Returns false otherwise.
   */
  bool equals(const MagicEffectRecord& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Writes the record to the given output stream.
   *
   * \param output  the output stream
   * \return Returns true on success (record was written to stream).
   *         Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const override;
  #endif

  /** \brief Loads the record from the given input stream.
   *
   * \param input    the input stream
   * \return Returns true on success (record was loaded from stream).
   *         Returns false, if an error occurred.
   */
  bool loadFromStream(std::istream& input) override;
}; // struct

} // namespace

#endif // MW_MAGICEFFECTRECORD_HPP
