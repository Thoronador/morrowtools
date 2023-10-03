/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2021, 2023  Dirk Stolle

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

#ifndef MW_SKILLCONSTANTS_HPP
#define MW_SKILLCONSTANTS_HPP

namespace MWTP
{

// constants for skill indices
const int32_t SkillBlock = 0;
const int32_t SkillArmorer = 1;
const int32_t SkillMediumArmor = 2;
const int32_t SkillHeavyArmor = 3;
const int32_t SkillBluntWeapon = 4;
const int32_t SkillLongBlade = 5;
const int32_t SkillAxe = 6;
const int32_t SkillSpear = 7;
const int32_t SkillAthletics = 8;
const int32_t SkillEnchant = 9;
const int32_t SkillDestruction = 10;
const int32_t SkillAlteration = 11;
const int32_t SkillIllusion = 12;
const int32_t SkillConjuration = 13;
const int32_t SkillMysticism = 14;
const int32_t SkillRestoration = 15;
const int32_t SkillAlchemy = 16;
const int32_t SkillUnarmored = 17;
const int32_t SkillSecurity = 18;
const int32_t SkillSneak = 19;
const int32_t SkillAcrobatics = 20;
const int32_t SkillLightArmor = 21;
const int32_t SkillShortBlade = 22;
const int32_t SkillMarksman = 23;
const int32_t SkillMercantile = 24;
const int32_t SkillSpeechcraft = 25;
const int32_t SkillHandToHand = 26;


// enumeration for specialization of skills
enum SkillSpecialization: uint32_t
{
  Combat = 0,
  Magic = 1,
  Stealth = 2
};

} // namespace

#endif // MW_SKILLCONSTANTS_HPP
