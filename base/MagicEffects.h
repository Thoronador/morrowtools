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

#ifndef MAGICEFFECTS_H
#define MAGICEFFECTS_H

#include <string>
#include <map>
#include <fstream>

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

  /* returns true, of content of other record is equal to this one */
  bool equals(const MGEF_Data& other) const;
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
    /* destructor */
    ~MagicEffects();

    /* singleton access */
    static MagicEffects& getSingleton();

    /* adds an effect to the list */
    void addEffect(const int Index, const MGEF_Data& Data);

    /* returns true, if an effect with the given index exists */
    bool hasEffect(const int Index) const;

    /* returns the number of different effects */
    unsigned int getNumberOfEffects() const;

    /* retrieves data of a specific magic effect

       parameters:
           Index - the index of the effect

       remarks:
           If there is no effect with the given index, the function throws an
           error. Use hasEffect() to determine if an effect exists.
    */
    const MGEF_Data& getEffect(const int Index) const;

    /* tries to read a magic effect record from the given input file and returns
       true on succes. If an error occured, false is returned.
    */
    bool readMGEF(std::ifstream& in_File);

    /* tries to read a magic effect record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File - the input file stream that is used to read the record
    */
    int readRecordMGEF(std::ifstream& in_File);

    EffectListIterator getBegin() const;
    EffectListIterator getEnd() const;

    /* deletes all effects */
    void clearAll();
  private:
    /* constructor */
    MagicEffects();

    /* copy constructor - empty */
    MagicEffects(const MGEF_Data& op) {}

    /* internal list of effects */
    std::map<int, MGEF_Data> m_Effects;
}; //class

#endif // MAGICEFFECTS_H
