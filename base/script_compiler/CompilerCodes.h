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
  const uint16_t CodeEnd   = 0x0101; //"0101"
  //const uint16_t CodeEndif = 0x0109; //"0901"
  const uint16_t CodeForceSneak = 0x1163; //"6311"
  const uint16_t CodeGetAcrobatics = 0x1077; //"7710"
  const uint16_t CodeGetAlchemy = 0x106B; //"6B10"
  const uint16_t CodeGetAlteration = 0x105C; //"5C10"
  const uint16_t CodeGetArmorer = 0x103E; //"3E10"
  const uint16_t CodeGetAthletics = 0x1053; //"5310"
  const uint16_t CodeGetAxe = 0x104D; //"4D10"
  const uint16_t CodeGetBlock = 0x103B; //"3B10"
  const uint16_t CodeGetBluntWeapon = 0x1047; //"4710"
  const uint16_t CodeGetChameleon = 0x10C3; //"C310"
  const uint16_t CodeGetConjuration = 0x1062; //"6210"
  const uint16_t CodeGetDestruction = 0x1059; //"5910"
  const uint16_t CodeGetDisposition = 0x1098; //"9810"
  const uint16_t CodeGetEnchant = 0x1056; //"5610"
  const uint16_t CodeGetIllusion = 0x105F; //"5F10"
  const uint16_t CodeGetMysticism = 0x1065; //"6510"
  const uint16_t CodeGetRestoration = 0x1068; //"6810"
  const uint16_t CodeJournal = 0x10CC; //"CC10"
  const uint16_t CodeReturn = 0x0124; //"2401"
  const uint16_t CodeSet = 0x0105; //"0501"
  const uint16_t CodeStartScript = 0x101B; //"1B10"
  const uint16_t CodeStopScript = 0x101C; //"1C10"
} //namespace

#endif // COMPILERCODES_H
