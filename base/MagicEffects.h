/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as published
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

#ifndef MAGICEFFECTS_H
#define MAGICEFFECTS_H

#include <string>
#include <map>

//spell schools
const int32_t cAlteration = 0;
const int32_t cConjuration = 1;
const int32_t cDestruction = 2;
const int32_t cIllusion = 3;
const int32_t cMysticism = 4;
const int32_t cRestoration = 5;

//flags
const int FlagSpellmaking = 0x0200;
const int FlagEnchanting = 0x0400;
const int FlagNegative = 0x0800;

struct MGEF_Data
{
  int32_t SpellSchool;
  float BaseCost;
  int Flags;
  int Red;
  int Green;
  int Blue;
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
}; //struct

//spell effect indices (hard-coded in MW)
const int EffectWaterBreathing = 0;
const int EffectSwiftSwim = 1;
const int EffectWaterWalking = 2;
const int EffectShield = 3;
const int EffectFireShield = 4;
const int EffectLightningShield = 5;
const int EffectFrostShield = 6;
const int EffectBurden = 7;
const int EffectFeather = 8;
const int EffectJump = 9;
const int EffectLevitate = 10;
const int EffectSlowFall = 11;
const int EffectLock = 12;
const int EffectOpen = 13;
const int EffectFireDamage = 14;
const int EffectShockDamage = 15;
const int EffectFrostDamage = 16;
const int EffectDrainAttribute = 17;
const int EffectDrainHealth = 18;
const int EffectDrainSpellpoints = 19;
const int EffectDrainFatigue = 20;
const int EffectDrainSkill = 21;
const int EffectDamageAttribute = 22;
const int EffectDamageHealth = 23;
const int EffectDamageMagicka = 24;
const int EffectDamageFatigue = 25;
const int EffectDamageSkill = 26;
const int EffectPoison = 27;
const int EffectWeaknessToFire = 28;
const int EffectWeaknessToFrost = 29;
const int EffectWeaknessToShock = 30;

const int EffectChameleon = 40;
const int EffectLight = 41;
const int EffectSanctuary = 42;
const int EffectNightEye = 43;
const int EffectCharm = 44;
const int EffectParalyze = 45;
const int EffectSilence = 46;
const int EffectBlind = 47;
const int EffectSound = 48;
const int EffectCalmHumanoid = 49;
const int EffectCalmCreature = 50;

const int EffectTelekinesis = 59;

typedef std::map<int, MGEF_Data>::const_iterator EffectListIterator;

class MagicEffects
{
  public:
    ~MagicEffects();
    static MagicEffects& getSingleton();
    void addEffect(const int Index, const MGEF_Data& Data);
    bool hasEffect(const int Index) const;
    unsigned int getNumberOfEffects() const;
    const MGEF_Data& getEffect(const int Index) const;
    void clearAll();
    EffectListIterator getBegin() const;
    EffectListIterator getEnd() const;
  private:
    MagicEffects();
    MagicEffects(const MGEF_Data& op) {}
    std::map<int, MGEF_Data> m_Effects;
}; //class

#endif // MAGICEFFECTS_H
