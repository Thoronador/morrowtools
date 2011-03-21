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
  //TODO: AddToLevCreature
  //TODO: AddToLevItem
  const uint16_t CodeAddTopic = 0x1022; //"2210"
  //TODO: AIActivate
  //TODO: AIEscort
  //TODO: AIEscortCell
  //TODO: AIFollow
  //TODO: AIFollowCell
  //TODO: AITravel
  //TODO: AIWander
  const uint16_t CodeBecomeWerewolf = 0x11B3; //"B311"
  const uint16_t CodeCast = 0x1123; //"2311"
  const uint16_t CodeCellChanged = 0x101F; //"1F10"
  const uint16_t CodeCellUpdate = 0x1013; //"1310"
  const uint16_t CodeChangeWeather = 0x1124; //"2411"
  //TODO: Choice
  const uint16_t CodeClearForceJump = 0x119E; //"9E11"
  const uint16_t CodeClearForceMoveJump = 0x11A1; //"A111"
  const uint16_t CodeClearForceRun = 0x119B; //"9B11"
  const uint16_t CodeClearForceSneak = 0x1164; //"6411"
  const uint16_t CodeClearInfoActor = 0x10CB; //"CB10"
  const uint16_t CodeDisable = 0x10DB; //"DB10"
  const uint16_t CodeDisableLevitation = 0x1194; //"9411"
  const uint16_t CodeDisablePlayerControls = 0x10DE; //"DE10"
  const uint16_t CodeDisablePlayerFighting = 0x115A; //"5A11"
  const uint16_t CodeDisablePlayerJumping = 0x1140; //"4011"
  const uint16_t CodeDisablePlayerLooking = 0x1143; //"4311"
  const uint16_t CodeDisablePlayerMagic = 0x115D; //"5D11"
  const uint16_t CodeDisablePlayerViewSwitch = 0x10E3; //"E310"
  const uint16_t CodeDisableTeleporting = 0x10EF; //"EF10"
  const uint16_t CodeDisableVanityMode = 0x114C; //"4C11"
  const uint16_t CodeDontSaveObject = 0x115F; //"5F11"
  const uint16_t CodeDrop = 0x110D; //"0D11"
  const uint16_t CodeDropSoulGem = 0x10ED; //"ED10"
  //TODO: Else
  //TODO: ElseIf
  const uint16_t CodeEnable = 0x10DA; //"DA10"
  const uint16_t CodeEnableBirthMenu = 0x1129; //"2911"
  const uint16_t CodeEnableClassMenu = 0x1128; //"2811"
  const uint16_t CodeEnableInventoryMenu = 0x1118; //"1811"
  const uint16_t CodeEnableLevelUpMenu = 0x1158; //"5811"
  const uint16_t CodeEnableLevitation = 0x1193; //"9311"
  const uint16_t CodeEnableMagicMenu = 0x111A; //"1A11"
  const uint16_t CodeEnableMapMenu = 0x1119; //"1911"
  const uint16_t CodeEnableNameMenu = 0x1126; //"2611"
  const uint16_t CodeEnablePlayerControls = 0x10DD; //"DD10"
  const uint16_t CodeEnablePlayerFighting = 0x1159; //"5911"
  const uint16_t CodeEnablePlayerJumping = 0x113F; //"3F11"
  const uint16_t CodeEnablePlayerLooking = 0x1142; //"4211"
  const uint16_t CodeEnablePlayerMagic = 0x115C; //"5C11"
  const uint16_t CodeEnablePlayerViewSwitch = 0x10E2; //"E210"
  const uint16_t CodeEnableRaceMenu = 0x1127; //"2711"
  const uint16_t CodeEnableRest = 0x013F; //"3F01"
  const uint16_t CodeEnableStatReviewMenu = 0x1160; //"6011"
  const uint16_t CodeEnableStatsMenu = 0x1117; //"1711"
  const uint16_t CodeEnableTeleporting = 0x10EE; //"EE10"
  const uint16_t CodeEnableVanityMode = 0x114B; //"4B11"
  const uint16_t CodeEnd = 0x0101; //"0101"
  //TODO: const uint16_t CodeEndif = 0x0109; //"0901"
  //TODO: const uint16_t CodeEndWhile = 0x010B; //"0B01"
  const uint16_t CodeEquip = 0x110E; //"0E11"
  const uint16_t CodeExplodeSpell = 0x11AD; //"AD11"
  //TODO: Face
  const uint16_t CodeFadeIn = 0x1131; //"3111"
  const uint16_t CodeFadeOut = 0x1130; //"3011"
  const uint16_t CodeFadeTo = 0x1145; //"4511"
  const uint16_t CodeFall = 0x1166; //"6611"
  const uint16_t CodeFillMap = 0x013E; //"3E01"
  const uint16_t CodeFixMe = 0x0131; //"3101"
  const uint16_t CodeForceGreeting = 0x10E8; //"E810"
  const uint16_t CodeForceJump = 0x119D; //"9D11"
  const uint16_t CodeForceMoveJump = 0x11A0; //"A011"
  const uint16_t CodeForceRun = 0x119A; //"9A11"
  const uint16_t CodeForceSneak = 0x1163; //"6311"
  const uint16_t CodeGetAcrobatics = 0x1077; //"7710"
  const uint16_t CodeGetAgility = 0x102C; //"2C10"
  const uint16_t CodeGetAIPackageDone = 0x10FB; //"FB10"
  const uint16_t CodeGetAlarm = 0x1105; //"0511"
  const uint16_t CodeGetAlchemy = 0x106B; //"6B10"
  const uint16_t CodeGetAlteration = 0x105C; //"5C10"
  const uint16_t CodeGetAngle = 0x100C; //"0C10"
  const uint16_t CodeGetArmorBonus = 0x1176; //"7611"
  const uint16_t CodeGetArmorer = 0x103E; //"3E10"
  const uint16_t CodeGetArmorType = 0x1198; //"9811"
  const uint16_t CodeGetAthletics = 0x1053; //"5310"
  const uint16_t CodeGetAttackBonus = 0x10A2; //"A210"
  const uint16_t CodeGetAttacked = 0x1148; //"4811"
  const uint16_t CodeGetAxe = 0x104D; //"4D10"
  const uint16_t CodeGetBlightDisease = 0x114A; //"4A11"
  const uint16_t CodeGetBlindness = 0x117F; //"7F11"
  const uint16_t CodeGetBlock = 0x103B; //"3B10"
  const uint16_t CodeGetBluntWeapon = 0x1047; //"4710"
  const uint16_t CodeGetButtonPressed = 0x101E; //"1E10"
  const uint16_t CodeGetCastPenalty = 0x1179; //"7911"
  const uint16_t CodeGetChameleon = 0x10C3; //"C310"
  const uint16_t CodeGetCollidingActor = 0x11A4; //"A411"
  const uint16_t CodeGetCollidingPC = 0x11A3; //"A311"
  const uint16_t CodeGetCommonDisease = 0x1149; //"4911"
  const uint16_t CodeGetConjuration = 0x1062; //"6210"
  const uint16_t CodeGetCurrentAIPackage = 0x10FA; //"FA10"
  const uint16_t CodeGetCurrentTime = 0x1011; //"1110"
  const uint16_t CodeGetCurrentWeather = 0x10A1; //"A110"
  const uint16_t CodeGetDeathCount = 0x10A5; //"FE10"
  const uint16_t CodeGetDefendBonus = 0x10A5; //"A510"
  const uint16_t CodeGetDestruction = 0x1059; //"5910"
  const uint16_t CodeGetDetected = 0x114E; //"4E11"
  const uint16_t CodeGetDisabled = 0x10DC; //"DC10"
  const uint16_t CodeGetDisposition = 0x1098; //"9810"
  const uint16_t CodeGetDistance = 0x1001; //"0110"
  const uint16_t CodeGetEffect = 0x1138; //"3811"
  const uint16_t CodeGetEnchant = 0x1056; //"5610"
  const uint16_t CodeGetEndurance = 0x1032; //"3210"
  //TODO: GetFactionReaction
  const uint16_t CodeGetFatigue = 0x1092; //"9210"
  const uint16_t CodeGetFight = 0x10FF; //"FF10"
  const uint16_t CodeGetFlee = 0x1002; //"0210"
  const uint16_t CodeGetFlying = 0x1173; //"7311"
  const uint16_t CodeGetForceJump = 0x119F; //"9F11"
  const uint16_t CodeGetForceMoveJump = 0x11A2; //"A211"
  const uint16_t CodeGetForceRun = 0x119C; //"9C11"
  const uint16_t CodeGetForceSneak = 0x1165; //"6511"
  const uint16_t CodeGetHandToHand = 0x1089; //"8910"
  const uint16_t CodeGetHealth = 0x108C; //"8C10"
  const uint16_t CodeGetHealthGetRatio = 0x1154; //"5411"
  const uint16_t CodeGetHeavyArmor = 0x1044; //"4410"
  const uint16_t CodeGetHello = 0x1108; //"0811"
  const uint16_t CodeGetIllusion = 0x105F; //"5F10"
  const uint16_t CodeGetIntelligence = 0x1026; //"2610"
  const uint16_t CodeGetInterior = 0x110B; //"0B11"
  const uint16_t CodeGetInvisible = 0x1185; //"8511"
  const uint16_t CodeGetItemCount = 0x10FD; //"FD10"
  const uint16_t CodeGetJournalIndex = 0x10CD; //"CD10"
  const uint16_t CodeGetLevel = 0x1157; //"5711"
  const uint16_t CodeGetLightArmor = 0x107A; //"7A10"
  const uint16_t CodeGetLineOfSight = 0x10FC; //"FC10"
  const uint16_t CodeGetLocked = 0x1161; //"6111"
  const uint16_t CodeGetLongBlade = 0x104A; //"4A10"
  const uint16_t CodeGetLOS = CodeGetLineOfSight; //"FC10"
  const uint16_t CodeGetLuck = 0x1038; //"3810"
  const uint16_t CodeGetMagicka = 0x108F; //"8F10"
  const uint16_t CodeGetMarksman = 0x1080; //"8010"
  const uint16_t CodeGetMasserPhase = 0x1146; //"4611"
  const uint16_t CodeGetMediumArmor = 0x1041; //"4110"
  const uint16_t CodeGetMercantile = 0x1083; //"8310"
  const uint16_t CodeGetMysticism = 0x1065; //"6510"
  const uint16_t CodeGetParalysis = 0x1182; //"8211"
  const uint16_t CodeGetPCCell = 0x1112; //"1211"
  const uint16_t CodeGetPCCrimeLevel = 0x109B; //"9B10"
  //TODO: GetPCFacRep
  const uint16_t CodeGetPCInJail = 0x11BB; //"BB11"
  const uint16_t CodeGetPCJumping = 0x118C; //"8C11"
  //TODO: GetPCRank
  const uint16_t CodeGetPCRunning = 0x118B; //"8B11"
  const uint16_t CodeGetPCSleep = 0x10E0; //"E010"
  const uint16_t CodeGetPCSneaking = 0x118A; //"8A11"
  const uint16_t CodeGetPCTraveling = 0x11BC; //"BC11"
  const uint16_t CodeGetPCVisionBonus = 0x111B0; //"B011"
  const uint16_t CodeGetPersonality = 0x1035; //"3510"
  const uint16_t CodeGetPlayerControlsDisabled = 0x10DF; //"DF10"
  const uint16_t CodeGetPlayerFightingDisabled = 0x115B; //"5B11"
  const uint16_t CodeGetPlayerJumpingDisabled = 0x1141; //"4111"
  const uint16_t CodeGetPlayerLookingDisabled = 0x1144; //"4411"
  const uint16_t CodeGetPlayerMagicDisabled = 0x115E; //"5E11"
  const uint16_t CodeGetPos = 0x100A; //"0A10"
  const uint16_t CodeGetRace = 0x1139; //"3911"
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
  const uint16_t CodeGetSecondsPassed = 0x1012; //"1210"
  const uint16_t CodeGetSecundaPhase = 0x1147; //"4711"
  const uint16_t CodeGetSecurity = 0x1071; //"7110"
  const uint16_t CodeGetShortBlade = 0x107D; //"7D10"
  const uint16_t CodeGetSilence = 0x117C; //"7C11"
  const uint16_t CodeGetSneak = 0x1074; //"7410"
  const uint16_t CodeGetSoundPlaying = 0x1188; //"8811"
  const uint16_t CodeGetSpear = 0x1050; //"5010"
  const uint16_t CodeGetSpeechcraft = 0x1086; //"8610"
  const uint16_t CodeGetSpeed = 0x102F; //"2F10"
  const uint16_t CodeGetSpell = 0x111F; //"1F11"
  const uint16_t CodeGetSpellEffects = 0x1121; //"2111"
  const uint16_t CodeGetSpellReadied = 0x11AF; //"AF11"
  const uint16_t CodeGetSquareRoot = 0x11AC; //"AC11"
  const uint16_t CodeGetStandingActor = 0x1114; //"1411"
  const uint16_t CodeGetStandingPC = 0x1113; //"1311"
  const uint16_t CodeGetStartingAngle = 0x100F; //"0F10"
  const uint16_t CodeGetStartingPos = 0x100E; //"0E10"
  const uint16_t CodeGetStrength = 0x1023; //"2310"
  const uint16_t CodeGetSuperJump = 0x1170; //"7011"
  const uint16_t CodeGetSwimSpeed = 0x116D; //"6D11"
  const uint16_t CodeGetTarget = 0x1150; //"5011"
  const uint16_t CodeGetUnarmored = 0x106E; //"6E10"
  const uint16_t CodeGetVanityModeDisabled = 0x114D; //"4D11"
  const uint16_t CodeGetWaterBreathing = 0x1167; //"6711"
  const uint16_t CodeGetWaterLevel = 0x1190; //"9011"
  const uint16_t CodeGetWaterWalking = 0x116A; //"6A11"
  const uint16_t CodeGetWeaponDrawn = 0x11AE; //"AE11"
  const uint16_t CodeGetWeaponType = 0x1197; //"9711"
  const uint16_t CodeGetWerewolfKills = 0x11B6; //"B611"
  const uint16_t CodeGetWillpower = 0x1029; //"2910"
  const uint16_t CodeGetWindSpeed = 0x10A0; //"A010"
  const uint16_t CodeGoodbye = 0x10CA; //"CA10"
  const uint16_t CodeGotoJail = 0x10E9; //"E910"
  const uint16_t CodeHasItemEquipped = 0x1199; //"9911"
  const uint16_t CodeHasSoulgem = 0x10EA; //"EA10"
  const uint16_t CodeHitAttemptOnMe = 0x1116; //"1611"
  const uint16_t CodeHitOnMe = 0x1115; //"1511"
  const uint16_t CodeHurtCollidingActor = 0x11A5; //"A511"
  const uint16_t CodeHurtStandingActor = 0x1135; //"3511"
  //TODO: If
  const uint16_t CodeIsWerewolf = 0x11B5; //"B511"
  const uint16_t CodeJournal = 0x10CC; //"CC10"
  const uint16_t CodeLock = 0x1136; //"3611"
  //TODO: LoopGroup
  //TODO: LowerRank
  const uint16_t CodeMenuMode = 0x1020; //"2010"
  //TODO: MenuTest
  //TODO: MessageBox
  const uint16_t CodeModAcrobatics = 0x1079; //"7910"
  const uint16_t CodeModAgility = 0x102E; //"2E10"
  const uint16_t CodeModAlarm = 0x1107; //"0711"
  const uint16_t CodeModAlchemy = 0x106D; //"6D10"
  const uint16_t CodeModAlteration = 0x105E; //"5E10"
  const uint16_t CodeModArmorBonus = 0x1178; //"7811"
  const uint16_t CodeModArmorer = 0x1040; //"4010"
  const uint16_t CodeModAthletics = 0x1055; //"5510"
  const uint16_t CodeModAttackBonus = 0x10A4; //"A410"
  const uint16_t CodeModAxe = 0x104F; //"4F10"
  const uint16_t CodeModBlindness = 0x1181; //"8111"
  const uint16_t CodeModBlock = 0x103D; //"3D10"
  const uint16_t CodeModBluntWeapon = 0x1049; //"4910"
  const uint16_t CodeModCastPenalty = 0x117B; //"7B11"
  const uint16_t CodeModChameleon = 0x10C5; //"C510"
  const uint16_t CodeModConjuration = 0x1064; //"6410"
  const uint16_t CodeModCurrentFatigue = 0x1134; //"3411"
  const uint16_t CodeModCurrentHealth = 0x1132; //"3211"
  const uint16_t CodeModCurrentMagicka = 0x1133; //"3311"
  const uint16_t CodeModDefendBonus = 0x10A7; //"A710"
  const uint16_t CodeModDestruction = 0x105B; //"5B10"
  const uint16_t CodeModDisposition = 0x109A; //"9A10"
  const uint16_t CodeModEnchant = 0x1058; //"5810"
  const uint16_t CodeModEndurance = 0x1034; //"3410"
  const uint16_t CodeModFactionReaction = 0x1111; //"1111"
  const uint16_t CodeModFatigue = 0x1094; //"9410"
  const uint16_t CodeModFight = 0x1101; //"0111"
  const uint16_t CodeModFlee = 0x1104; //"0411"
  const uint16_t CodeModFlying = 0x1175; //"7511"
  const uint16_t CodeModHandToHand = 0x108B; //"8B10"
  const uint16_t CodeModHealth = 0x108E; //"8E10"
  const uint16_t CodeModHeavyArmor = 0x1046; //"4610"
  const uint16_t CodeModHello = 0x110A; //"0A11"
  const uint16_t CodeModIllusion = 0x1061; //"6110"
  const uint16_t CodeModIntelligence = 0x1028; //"2810"
  const uint16_t CodeModInvisible = 0x1187; //"8711"
  const uint16_t CodeModLightArmor = 0x107C; //"7C10"
  const uint16_t CodeModLongBlade = 0x104C; //"4C10"
  const uint16_t CodeModLuck = 0x103A; //"3A10"
  const uint16_t CodeModMagicka = 0x1091; //"9110"
  const uint16_t CodeModMarksman = 0x1082; //"8210"
  const uint16_t CodeModMediumArmor = 0x1043; //"4310"
  const uint16_t CodeModMercantile = 0x1085; //"8510"
  const uint16_t CodeModMysticism = 0x1067; //"6710"
  const uint16_t CodeModParalysis = 0x1184; //"8411"
  const uint16_t CodeModPCCrimeLevel = 0x109D; //"9D10"
  const uint16_t CodeModPCFacRep = 0x10D9; //"D910"
  const uint16_t CodeModPCVisionBonus = 0x11B2; //"B211"
  const uint16_t CodeModPersonality = 0x1037; //"3710"
  const uint16_t CodeModRegion = 0x1125; //"2511"
  const uint16_t CodeModReputation = 0x1097; //"9710"
  const uint16_t CodeModResistBlight = 0x10B9; //"B910"
  const uint16_t CodeModResistCorprus = 0x10BC; //"BC10"
  const uint16_t CodeModResistDisease = 0x10B6; //"B610"
  const uint16_t CodeModResistFire = 0x10AD; //"AD10"
  const uint16_t CodeModResistFrost = 0x10B0; //"B010"
  const uint16_t CodeModResistMagicka = 0x10AA; //"AA10"
  const uint16_t CodeModResistNormalWeapons = 0x10C8; //"C810"
  const uint16_t CodeModResistParalysis = 0x10C2; //"C210"
  const uint16_t CodeModResistPoison = 0x10BF; //"BF10"
  const uint16_t CodeModResistShock = 0x10B3; //"B310"
  const uint16_t CodeModRestoration = 0x106A; //"6A10"
  const uint16_t CodeModScale = 0x118F; //"8F11"
  const uint16_t CodeModSecurity = 0x1073; //"7310"
  const uint16_t CodeModShortBlade = 0x107F; //"7F10"
  const uint16_t CodeModSilence = 0x117E; //"7E11"
  const uint16_t CodeModSneak = 0x1076; //"7610"
  const uint16_t CodeModSpear = 0x1052; //"5210"
  const uint16_t CodeModSpeechcraft = 0x1088; //"8810"
  const uint16_t CodeModSpeed = 0x1031; //"3110"
  const uint16_t CodeModStrength = 0x1025; //"2510"
  const uint16_t CodeModSuperJump = 0x1172; //"7211"
  const uint16_t CodeModSwimSpeed = 0x116F; //"6F11"
  const uint16_t CodeModUnarmored = 0x1070; //"7010"
  const uint16_t CodeModWaterBreathing = 0x1169; //"6911"
  const uint16_t CodeModWaterLevel = 0x9211; //"9211"
  const uint16_t CodeModWaterWalking = 0x116C; //"6C11"
  const uint16_t CodeModWillpower = 0x102B; //"2B10" "It's now or never..."
  const uint16_t CodeMove = 0x1006; //"0610"
  const uint16_t CodeMoveWorld = 0x1008; //"0810"
  const uint16_t CodeOnActivate = 0x1018; //"1810"
  const uint16_t CodeOnDeath = 0x10F0; //"F010"
  const uint16_t CodeOnKnockout = 0x10F1; //"F110"
  const uint16_t CodeOnMurder = 0x10F2; //"F210"
  const uint16_t CodePayFine = 0x114F; //"4F11"
  const uint16_t CodePayFineThief = 0x1189; //"8911"
  const uint16_t CodePCClearExpelled = 0x10D3; //"D310"
  const uint16_t CodePCExpell = 0x10D2; //"D210"
  const uint16_t CodePCExpelled = 0x109F; //"9F10"
  const uint16_t CodePCForce1stPerson = 0x113D; //"3D11"
  const uint16_t CodePCForce3rdPerson = 0x113C; //"3C11"
  const uint16_t CodePCGet3rdPerson = 0x113E; //"3E11"
  const uint16_t CodePCJoinFaction = 0x113B; //"3B11"
  const uint16_t CodePCLowerRank = 0x10D1; //"D110"
  const uint16_t CodePCRaiseRank = 0x10D0; //"D010"
  const uint16_t CodePlaceAtMe = 0x11BA; //"BA11"
  const uint16_t CodePlaceAtPC = 0x10E6; //"E610"
  const uint16_t CodePlaceItem = 0x1195; //"9511"
  const uint16_t CodePlaceItemCell = 0x1196; //"9611"
  const uint16_t CodePlayBink = 0x1155; //"5511"
  const uint16_t CodePlayGroup = 0x1014; //"1410"
  const uint16_t CodePlayLoopSound3D = 0x112E; //"2E11"
  const uint16_t CodePlayLoopSound3DVP = 0x112F; //"2F11"
  const uint16_t CodePlaySound = 0x1002; //"0210"
  const uint16_t CodePlaySoundVP = 0x112B; //"2B11"
  const uint16_t CodePlaySound3D = 0x112C; //"2C11"
  const uint16_t CodePlaySound3DVP = 0x112D; //"2D11"
  const uint16_t CodePosition = 0x1004; //"0410"
  const uint16_t CodePositionCell = 0x1005; //"0510"
  const uint16_t CodeQualifier = 0x010C; //"0C01"
  const uint16_t CodeRaiseRank = 0x10CE; //"CE10"
  const uint16_t CodeRandom = 0x1021; //"2110"
  const uint16_t CodeRemoveEffects = 0x1122; //"2211"
  const uint16_t CodeRemoveFromLevCreature = 0x11A7; //"A711"
  const uint16_t CodeRemoveFromLevItem = 0x11A9; //"A911"
  const uint16_t CodeRemoveItem = 0x10D5; //"D510"
  const uint16_t CodeRemoveSoulGem = 0x10EC; //"EC10"
  const uint16_t CodeRemoveSpell = 0x111E; //"1E11"
  const uint16_t CodeRemoveSpellEffects = 0x1120; //"2011"
  const uint16_t CodeRepairedOnMe = 0x110F; //"0F11"
  const uint16_t CodeResurrect = 0x10E7; //"E710"
  const uint16_t CodeReturn = 0x0124; //"2401"
  const uint16_t CodeRotate = 0x1007; //"0710"
  const uint16_t CodeRotateWorld = 0x1009; //"0910"
  const uint16_t CodeSameFaction = 0x109E; //"9E10"
  const uint16_t CodeSay = 0x111B; //"1B11"
  const uint16_t CodeSayDone = 0x111C; //"1C11"
  const uint16_t CodeScriptRunning = 0x101D; //"1D10"
  const uint16_t CodeSet = 0x0105; //"0501"
  const uint16_t CodeSetAcrobatics = 0x1078; //"7810"
  const uint16_t CodeSetAgility = 0x102D; //"2D10"
  const uint16_t CodeSetAlarm = 0x1106; //"0611"
  const uint16_t CodeSetAlchemy = 0x106C; //"6C10"
  const uint16_t CodeSetAngle = 0x100D; //"0D10"
  const uint16_t CodeSetArmorBonus = 0x1177; //"7711"
  const uint16_t CodeSetArmorer = 0x103F; //"3F10"
  const uint16_t CodeSetAthletics = 0x1054; //"5410"
  const uint16_t CodeSetAtStart = 0x1010; //"1010"
  const uint16_t CodeSetAttackBonus = 0x10A3; //"A310"
  const uint16_t CodeSetAxe = 0x104E; //"4E10"
  const uint16_t CodeSetBlindness = 0x1180; //"8011"
  const uint16_t CodeSetBlock = 0x103C; //"3C10"
  const uint16_t CodeSetBluntWeapon = 0x1048; //"4810"
  const uint16_t CodeSetCastPenalty = 0x117A; //"7A11"
  const uint16_t CodeSetChameleon = 0x10C4; //"C410"
  const uint16_t CodeSetConjuration = 0x1063; //"6310"
  const uint16_t CodeSetDefendBonus = 0x10A6; //"A610"
  const uint16_t CodeSetDelete = 0x11AB; //"AB11"
  const uint16_t CodeSetDestruction = 0x105A; //"5A10"
  const uint16_t CodeSetDisposition = 0x1099; //"9910"
  const uint16_t CodeSetEnchant = 0x1057; //"5710"
  const uint16_t CodeSetEndurance = 0x1033; //"3310"
  const uint16_t CodeSetFactionReaction = 0x1110; //"1011"
  const uint16_t CodeSetFatigue = 0x1093; //"9310"
  const uint16_t CodeSetFight = 0x1100; //"0011"
  const uint16_t CodeSetFlee = 0x1103; //"0311"
  const uint16_t CodeSetFlying = 0x1174; //"7411"
  const uint16_t CodeSetHandToHand = 0x108A; //"8A10"
  const uint16_t CodeSetHealth = 0x108D; //"8D10"
  const uint16_t CodeSetHeavyArmor = 0x1045; //"4510"
  const uint16_t CodeSetHello = 0x1109; //"0911"
  const uint16_t CodeSetIllusion = 0x1060; //"6010"
  const uint16_t CodeSetIntelligence = 0x1027; //"2710"
  const uint16_t CodeSetInvisible = 0x1186; //"8611"
  //TODO: SetJournalIndex
  const uint16_t CodeSetLevel = 0x1156; //"5611"
  const uint16_t CodeSetLightArmor = 0x107B; //"7B10"
  const uint16_t CodeSetLongBlade = 0x104B; //"4B10"
  const uint16_t CodeSetLuck = 0x1039; //"3910"
  const uint16_t CodeSetMagicka = 0x1090; //"9010"
  const uint16_t CodeSetMarksman = 0x1081; //"8110"
  const uint16_t CodeSetMediumArmor = 0x1042; //"4210"
  const uint16_t CodeSetMercantile = 0x1084; //"8410"
  const uint16_t CodeSetMysticism = 0x1066; //"6610"
  const uint16_t CodeSetParalysis = 0x1183; //"8311"
  const uint16_t CodeSetPCCrimeLevel = 0x109C; //"9C10"
  const uint16_t CodeSetPCFacRep = 0x10D8; //"D810"
  const uint16_t CodeSetPCVisionBonus = 0x11B1; //"B111"
  const uint16_t CodeSetPersonality = 0x1036; //"3610"
  const uint16_t CodeSetPos = 0x100B; //"0B10"
  const uint16_t CodeSetReputation = 0x1096; //"9610"
  const uint16_t CodeSetResistBlight = 0x10B8; //"B810"
  const uint16_t CodeSetResistCorprus = 0x10BB; //"BB10"
  const uint16_t CodeSetResistDisease = 0x10B5; //"B510"
  const uint16_t CodeSetResistFire = 0x10AC; //"AC10"
  const uint16_t CodeSetResistFrost = 0x10AF; //"AF10"
  const uint16_t CodeSetResistMagicka = 0x10A9; //"A910"
  const uint16_t CodeSetResistNormalWeapons = 0x10C7; //"C710"
  const uint16_t CodeSetResistParalysis = 0x10C1; //"C110"
  const uint16_t CodeSetResistPoison = 0x10BE; //"BE10"
  const uint16_t CodeSetResistShock = 0x10B2; //"B210"
  const uint16_t CodeSetRestoration = 0x1069; //"6910"
  const uint16_t CodeSetScale = 0x118E; //"8E11"
  const uint16_t CodeSetSecurity = 0x1072; //"7210"
  const uint16_t CodeSetShortBlade = 0x107E; //"7E10"
  const uint16_t CodeSetSilence = 0x117D; //"7D11"
  const uint16_t CodeSetSneak = 0x1075; //"7510"
  const uint16_t CodeSetSpear = 0x1051; //"5110"
  const uint16_t CodeSetSpeechcraft = 0x1087; //"8710"
  const uint16_t CodeSetSpeed = 0x1030; //"3010"
  const uint16_t CodeSetStrength = 0x1024; //"2410"
  const uint16_t CodeSetSuperJump = 0x1171; //"7111"
  const uint16_t CodeSetSwimSpeed = 0x116E; //"6E11"
  const uint16_t CodeSetUnarmored = 0x106F; //"6F10"
  const uint16_t CodeSetWaterBreathing = 0x1168; //"6811"
  const uint16_t CodeSetWaterLevel = 0x1191; //"9111"
  const uint16_t CodeSetWaterWalking = 0x116B; //"6B11"
  const uint16_t CodeSetWerewolfAcrobatics = 0x11B9; //"B911"
  const uint16_t CodeSetWillpower = 0x102A; //"2A10"
  const uint16_t CodeShowMap = 0x1152; //"5211"
  const uint16_t CodeShowRestMenu = 0x10E5; //"E510"
  const uint16_t CodeSkipAnim = 0x1016; //"1610"
  const uint16_t CodeStartCombat = 0x1019; //"1910"
  const uint16_t CodeStartScript = 0x101B; //"1B10"
  const uint16_t CodeStopCombat = 0x101A; //"1A10"
  const uint16_t CodeStopScript = 0x101C; //"1C10"
  const uint16_t CodeStopSound = 0x1151; //"5111"
  const uint16_t CodeStreamMusic = 0x1003; //"0310"
  const uint16_t CodeTurnMoonRed = 0x11B7; //"B711"
  const uint16_t CodeTurnMoonWhite = 0x11B8; //"B811"
  const uint16_t CodeUndoWerewolf = 0x11B4; //"B411"
  const uint16_t CodeUnlock = 0x1137; //"3711"
  const uint16_t CodeWakeUpPC = 0x10E1; //"E110"
  //TODO: While
  const uint16_t CodeXBox = 0x113A; //"3A11"
} //namespace

#endif // COMPILERCODES_H
