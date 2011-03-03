/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef COMPILERCODES_H
#define COMPILERCODES_H

namespace ScriptCompiler
{
  /* op-codes for scripts' compiled data */
  const uint16_t CodeActivate = 0x1017; //"1710"
  const uint16_t CodeAddItem = 0x10D4; //"D410"
  const uint16_t CodeAddSoulGem = 0x10EB; //"EB10"
  const uint16_t CodeAddSpell = 0x111D; //"1D11"
  const uint16_t CodeAddTopic = 0x1022; //"2210"
  const uint16_t CodeEnd = 0x0101; //"0101"
  //const uint16_t CodeEndif = 0x0109; //"0901"
  const uint16_t CodeForceSneak = 0x1163; //"6311"
  const uint16_t CodeGetAcrobatics = 0x1077; //"7710"
  const uint16_t CodeGetAgility = 0x102C; //"2C10"
  const uint16_t CodeGetAlarm = 0x1105; //"0511"
  const uint16_t CodeGetAlchemy = 0x106B; //"6B10"
  const uint16_t CodeGetAlteration = 0x105C; //"5C10"
  const uint16_t CodeGetArmorBonus = 0x1176; //"7611"
  const uint16_t CodeGetArmorer = 0x103E; //"3E10"
  const uint16_t CodeGetAthletics = 0x1053; //"5310"
  const uint16_t CodeGetAttackBonus = 0x10A2; //"A210"
  const uint16_t CodeGetAxe = 0x104D; //"4D10"
  const uint16_t CodeGetBlindness = 0x117F; //"7F11"
  const uint16_t CodeGetBlock = 0x103B; //"3B10"
  const uint16_t CodeGetBluntWeapon = 0x1047; //"4710"
  const uint16_t CodeGetCastPenalty = 0x1179; //"7911"
  const uint16_t CodeGetChameleon = 0x10C3; //"C310"
  const uint16_t CodeGetConjuration = 0x1062; //"6210"
  const uint16_t CodeGetDefendBonus = 0x10A5; //"A510"
  const uint16_t CodeGetDestruction = 0x1059; //"5910"
  const uint16_t CodeGetDisposition = 0x1098; //"9810"
  const uint16_t CodeGetEnchant = 0x1056; //"5610"
  const uint16_t CodeGetEndurance = 0x1032; //"3210"
  const uint16_t CodeGetFatigue = 0x1092; //"9210"
  const uint16_t CodeGetFight = 0x10FF; //"FF10"
  const uint16_t CodeGetFlee = 0x1002; //"0210"
  const uint16_t CodeGetFlying = 0x1173; //"7311"
  const uint16_t CodeGetHandToHand = 0x1089; //"8910"
  const uint16_t CodeGetHealth = 0x108C; //"8C10"
  const uint16_t CodeGetHeavyArmor = 0x1044; //"4410"
  const uint16_t CodeGetHello = 0x1108; //"0811"
  const uint16_t CodeGetIllusion = 0x105F; //"5F10"
  const uint16_t CodeGetIntelligence = 0x1026; //"2610"
  const uint16_t CodeGetInvisible = 0x1185; //"8511"
  const uint16_t CodeGetLevel = 0x1157; //"5711"
  const uint16_t CodeGetLightArmor = 0x107A; //"7A10"
  const uint16_t CodeGetLongBlade = 0x104A; //"4A10"
  const uint16_t CodeGetLuck = 0x1038; //"3810"
  const uint16_t CodeGetMagicka = 0x108F; //"8F10"
  const uint16_t CodeGetMarksman = 0x1080; //"8010"
  const uint16_t CodeGetMediumArmor = 0x1041; //"4110"
  const uint16_t CodeGetMercantile = 0x1083; //"8310"
  const uint16_t CodeGetMysticism = 0x1065; //"6510"
  const uint16_t CodeGetParalysis = 0x1182; //"8211"
  const uint16_t CodeGetPCCrimeLevel = 0x109B; //"9B10"
  const uint16_t CodeGetPCVisionBonus = 0x111B0; //"B011"
  const uint16_t CodeGetPersonality = 0x1035; //"3510"
  const uint16_t CodeGetReputation = 0x1095; //"9510"
  const uint16_t CodeGetResistBlight = 0x10B7; //"B710"
  const uint16_t CodeGetResistCorprus = 0x10BA; //"BA10"
  const uint16_t CodeGetResistDisease = 0x10B4; //"B410"
  const uint16_t CodeGetResistFire = 0x10AB; //"AB10"
  const uint16_t CodeGetResistFrost = 0x10AE; //"AE10"
  const uint16_t CodeGetResistMagicka = 0x10A8; //"A810"
  const uint16_t CodeGetResistNormalWeapons = 0x10C6; //"C610"
  const uint16_t CodeGetResistParalysis = 0x10C0; //"C010"
  const uint16_t CodeGetResistPoison = 0x10BD; //"BD10"
  const uint16_t CodeGetResistShock = 0x10B1; //"B110"
  const uint16_t CodeGetRestoration = 0x1068; //"6810"
  const uint16_t CodeGetScale = 0x118D; //"8D11"
  const uint16_t CodeGetSecurity = 0x1071; //"7110"
  const uint16_t CodeGetShortBlade = 0x107D; //"7D10"
  const uint16_t CodeGetSilence = 0x117C; //"7C11"
  const uint16_t CodeGetSneak = 0x1074; //"7410"
  const uint16_t CodeGetSpear = 0x1050; //"5010"
  const uint16_t CodeGetSpeechcraft = 0x1086; //"8610"
  const uint16_t CodeGetSpeed = 0x102F; //"2F10"
  const uint16_t CodeGetStrength = 0x1023; //"2310"
  const uint16_t CodeGetSuperJump = 0x1170; //"7011"
  const uint16_t CodeGetSwimSpeed = 0x116D; //"6D11"
  const uint16_t CodeGetUnarmored = 0x106E; //"6E10"
  const uint16_t CodeGetWaterBreathing = 0x1167; //"6711"
  const uint16_t CodeGetWaterWalking = 0x116A; //"6A11"
  const uint16_t CodeGetWillpower = 0x1029; //"2910"
  const uint16_t CodeJournal = 0x10CC; //"CC10"
  const uint16_t CodeReturn = 0x0124; //"2401"
  const uint16_t CodeSet = 0x0105; //"0501"
  const uint16_t CodeStartScript = 0x101B; //"1B10"
  const uint16_t CodeStopScript = 0x101C; //"1C10"
} //namespace

#endif // COMPILERCODES_H
