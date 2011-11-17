/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef SR_CONSTANTS_H
#define SR_CONSTANTS_H

#include <stdint.h>

namespace SRTP
{

const int32_t cBAMT = 0x544D4142; //"BAMT" = 42 41 4D 54
const int32_t cBIDS = 0x53444942; //"BIDS" = 42 49 44 53
const int32_t cCNAM = 0x4D414E43; //"CNAM" = 43 4E 41 4D
const int32_t cCRDT = 0x54445243; //"CRDT" = 43 52 44 54
const int32_t cDATA = 0x41544144; //"DATA" = 44 41 54 41
const int32_t cDESC = 0x43534544; //"DESC" = 44 45 53 43
const int32_t cDNAM = 0x4D414E44; //"DNAM" = 44 4E 41 4D
const int32_t cEAMT = 0x544D4145; //"EAMT" = 45 41 4D 54
const int32_t cEDID = 0x44494445; //"EDID" = 45 44 49 44
const int32_t cEITM = 0x4D544945; //"EITM" = 45 49 54 4D
const int32_t cETYP = 0x50595445; //"ETYP" = 45 54 59 50
const int32_t cEYES = 0x53455945; //"EYES" = 45 59 45 53
const int32_t cFLTV = 0x56544C46; //"FLTV" = 46 4C 54 56
const int32_t cFNAM = 0x4D414E46; //"FNAM" = 46 4E 41 4D
const int32_t cFULL = 0x4C4C5546; //"FULL" = 46 55 4C 4C
const int32_t cGLOB = 0x424F4C47; //"GLOB" = 47 4C 4F 42
const int32_t cGMST = 0x54534D47; //"GMST" = 47 4D 53 54
const int32_t cGRUP = 0x50555247; //"GRUP" = 47 52 55 50
const int32_t cHEDR = 0x52444548; //"HEDR" = 48 45 44 52
const int32_t cICON = 0x4E4F4349; //"ICON" = 49 43 4F 4E
const int32_t cINAM = 0x4D414E49; //"INAM" = 49 4E 41 4D
const int32_t cINTV = 0x56544E49; //"INTV" = 49 4E 54 56
const int32_t cKSIZ = 0x5A49534B; //"KSIZ" = 4B 53 49 5A
const int32_t cKYWD = 0x4457594B; //"KYWD" = 4B 59 57 44
const int32_t cKWDA = 0x4144574B; //"KWDA" = 4B 57 44 41
const int32_t cMODL = 0x4C444F4D; //"MODL" = 4D 4F 44 4C
const int32_t cMODT = 0x54444F4D; //"MODT" = 4D 4F 44 54
const int32_t cNAM8 = 0x384D414E; //"NAM9" = 4E 41 4D 38
const int32_t cNAM9 = 0x394D414E; //"NAM9" = 4E 41 4D 39
const int32_t cOBND = 0x444E424F; //"OBND" = 4F 42 4E 44
const int32_t cTES4 = 0x34534554; //"TES4" = 54 45 53 34
const int32_t cTNAM = 0x4D414E54; //"TNAM" = 54 4E 41 4D
const int32_t cVNAM = 0x4D414E56; //"VNAM" = 56 4E 41 4D
const int32_t cWEAP = 0x50414557; //"WEAP" = 57 45 41 50
const int32_t cWNAM = 0x4D414E57; //"WNAM" = 57 4E 41 4D
const int32_t cWOOP = 0x504F4F57; //"WOOP" = 57 4F 4F 50

} //namespace

#endif // SR_CONSTANTS_H
