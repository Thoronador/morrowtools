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

const int32_t cAACT = 0x54434141; //"AACT" = 41 41 43 54
const int32_t cACTI = 0x49544341; //"ACTI" = 41 43 54 49
const int32_t cAMMO = 0x4F4D4D41; //"AMMO" = 41 4D 4D 4F
const int32_t cASTP = 0x50545341; //"ASTP" = 41 53 54 50
const int32_t cBAMT = 0x544D4142; //"BAMT" = 42 41 4D 54
const int32_t cBIDS = 0x53444942; //"BIDS" = 42 49 44 53
const int32_t cBNAM = 0x4D414E42; //"BNAM" = 42 4E 41 4D
const int32_t cBOOK = 0x4B4F4F42; //"BOOK" = 42 4F 4F 4B
const int32_t cCIS2 = 0x32534943; //"CIS2" = 43 49 53 32
const int32_t cCNAM = 0x4D414E43; //"CNAM" = 43 4E 41 4D
const int32_t cCNTO = 0x4F544E43; //"CNTO" = 43 4E 54 4F
const int32_t cCOBJ = 0x4A424F43; //"COBJ" = 43 4F 42 4A
const int32_t cCOCT = 0x54434F43; //"COCT" = 43 4F 43 54
const int32_t cCRDT = 0x54445243; //"CRDT" = 43 52 44 54
const int32_t cCTDA = 0x41445443; //"CTDA" = 43 54 44 41
const int32_t cDATA = 0x41544144; //"DATA" = 44 41 54 41
const int32_t cDESC = 0x43534544; //"DESC" = 44 45 53 43
const int32_t cDEST = 0x54534544; //"DEST" = 44 45 53 54
const int32_t cDMDL = 0x4C444D44; //"DMDL" = 44 4D 44 4C
const int32_t cDMDS = 0x53444D44; //"DMDS" = 44 4D 44 53
const int32_t cDMDT = 0x54444D44; //"DMDT" = 44 4D 44 54
const int32_t cDNAM = 0x4D414E44; //"DNAM" = 44 4E 41 4D
const int32_t cDODT = 0x54444F44; //"DODT" = 44 4F 44 54
const int32_t cDSTD = 0x44545344; //"DSTD" = 44 53 54 44
const int32_t cDSTF = 0x46545344; //"DSTF" = 44 53 54 46
const int32_t cEAMT = 0x544D4145; //"EAMT" = 45 41 4D 54
const int32_t cEDID = 0x44494445; //"EDID" = 45 44 49 44
const int32_t cEFID = 0x44494645; //"EFID" = 45 46 49 44
const int32_t cEFIT = 0x54494645; //"EFIT" = 45 46 49 54
const int32_t cEITM = 0x4D544945; //"EITM" = 45 49 54 4D
const int32_t cENIT = 0x54494E45; //"ENIT" = 45 4E 49 54
const int32_t cEPFD = 0x44465045; //"EPFD" = 45 50 46 44
const int32_t cEPFT = 0x54465045; //"EPFT" = 45 50 46 54
const int32_t cEQUP = 0x50555145; //"EQUP" = 45 51 55 50
const int32_t cETYP = 0x50595445; //"ETYP" = 45 54 59 50
const int32_t cEYES = 0x53455945; //"EYES" = 45 59 45 53
const int32_t cFCHT = 0x54484346; //"FCHT" = 46 43 48 54
const int32_t cFLST = 0x54534C46; //"FLST" = 46 4C 53 54
const int32_t cFLTR = 0x52544C46; //"FLTR" = 46 4C 54 52
const int32_t cFLTV = 0x56544C46; //"FLTV" = 46 4C 54 56
const int32_t cFNAM = 0x4D414E46; //"FNAM" = 46 4E 41 4D
const int32_t cFPRT = 0x54525046; //"FPRT" = 46 50 52 54
const int32_t cFULL = 0x4C4C5546; //"FULL" = 46 55 4C 4C
const int32_t cGLOB = 0x424F4C47; //"GLOB" = 47 4C 4F 42
const int32_t cGMST = 0x54534D47; //"GMST" = 47 4D 53 54
const int32_t cGNAM = 0x4D414E47; //"GNAM" = 47 4E 41 4D
const int32_t cGRUP = 0x50555247; //"GRUP" = 47 52 55 50
const int32_t cHEDR = 0x52444548; //"HEDR" = 48 45 44 52
const int32_t cHNAM = 0x4D414E48; //"HNAM" = 48 4E 41 4D
const int32_t cICON = 0x4E4F4349; //"ICON" = 49 43 4F 4E
const int32_t cINAM = 0x4D414E49; //"INAM" = 49 4E 41 4D
const int32_t cINDX = 0x58444E49; //"INDX" = 49 4E 44 58
const int32_t cINGR = 0x52474E49; //"INGR" = 49 4E 47 52
const int32_t cINTV = 0x56544E49; //"INTV" = 49 4E 54 56
const int32_t cKNAM = 0x4D414E4B; //"KNAM" = 4B 4E 41 4D
const int32_t cKSIZ = 0x5A49534B; //"KSIZ" = 4B 53 49 5A
const int32_t cKYWD = 0x4457594B; //"KYWD" = 4B 59 57 44
const int32_t cKWDA = 0x4144574B; //"KWDA" = 4B 57 44 41
const int32_t cLNAM = 0x4D414E4C; //"LNAM" = 4C 4E 41 4D
const int32_t cLTEX = 0x5845544C; //"LTEX" = 4C 54 45 58
const int32_t cMAST = 0x5453414D; //"MAST" = 4D 41 53 54
const int32_t cMCHT = 0x5448434D; //"MCHT" = 4D 43 48 54
const int32_t cMDOB = 0x424F444D; //"MDOB" = 4D 44 4F 42
const int32_t cMGEF = 0x4645474D; //"MGEF" = 4D 47 45 46
const int32_t cMISC = 0x4353494D; //"MISC" = 4D 49 53 43
const int32_t cMNAM = 0x4D414E4D; //"MNAM" = 4D 4E 41 4D
const int32_t cMODL = 0x4C444F4D; //"MODL" = 4D 4F 44 4C
const int32_t cMODS = 0x53444F4D; //"MODS" = 4D 4F 44 53
const int32_t cMODT = 0x54444F4D; //"MODT" = 4D 4F 44 54
const int32_t cMPRT = 0x5452504D; //"MPRT" = 4D 50 52 54
const int32_t cNAM1 = 0x314D414E; //"NAM1" = 4E 41 4D 31
const int32_t cNAM8 = 0x384D414E; //"NAM9" = 4E 41 4D 38
const int32_t cNAM9 = 0x394D414E; //"NAM9" = 4E 41 4D 39
const int32_t cNEXT = 0x5458454E; //"NEXT" = 4E 45 58 54
const int32_t cNNAM = 0x4D414E4E; //"NNAM" = 4E 4E 41 4D
const int32_t cOBND = 0x444E424F; //"OBND" = 4F 42 4E 44
const int32_t cOTFT = 0x5446544F; //"OTFT" = 4F 54 46 54
const int32_t cPERK = 0x4B524550; //"PERK" = 50 45 52 4B
const int32_t cPNAM = 0x4D414E50; //"PNAM" = 50 4E 41 4D
const int32_t cPRKC = 0x434B5250; //"PRKC" = 50 52 4B 43
const int32_t cPRKE = 0x454B5250; //"PRKE" = 50 52 4B 45
const int32_t cPRKF = 0x464B5250; //"PRKF" = 50 52 4B 46
const int32_t cQSDT = 0x54445351; //"QSDT" = 51 53 44 54
const int32_t cQUST = 0x54535551; //"QUST" = 51 55 53 54
const int32_t cRELA = 0x414C4552; //"RELA" = 52 45 4C 41
const int32_t cRNAM = 0x4D414E52; //"RNAM" = 52 4E 41 4D
const int32_t cSCRL = 0x4C524353; //"SCRL" = 53 43 52 4C
const int32_t cSDSC = 0x43534453; //"SDSC" = 53 44 53 43
const int32_t cSHOU = 0x554F4853; //"SHOU" = 53 48 4F 55
const int32_t cSNAM = 0x4D414E53; //"SNAM" = 53 4E 41 4D
const int32_t cSNDD = 0x44444E53; //"SNDD" = 53 4E 44 44
const int32_t cSOUN = 0x4E554F53; //"SOUN" = 53 4F 55 4E
const int32_t cSPEL = 0x4C455053; //"SPEL" = 53 50 45 4C
const int32_t cSPIT = 0x54495053; //"SPIT" = 53 50 49 54
const int32_t cSTAT = 0x54415453; //"STAT" = 53 54 41 54
const int32_t cTES4 = 0x34534554; //"TES4" = 54 45 53 34
const int32_t cTNAM = 0x4D414E54; //"TNAM" = 54 4E 41 4D
const int32_t cTX00 = 0x30305854; //"TX00" = 54 58 30 30
const int32_t cTX01 = 0x31305854; //"TX01" = 54 58 30 31
const int32_t cTX02 = 0x32305854; //"TX02" = 54 58 30 32
const int32_t cTX03 = 0x33305854; //"TX03" = 54 58 30 33
const int32_t cTX04 = 0x34305854; //"TX04" = 54 58 30 34
const int32_t cTX05 = 0x35305854; //"TX05" = 54 58 30 35
const int32_t cTX07 = 0x37305854; //"TX07" = 54 58 30 37
const int32_t cTXST = 0x54535854; //"TXST" = 54 58 53 54
const int32_t cUNAM = 0x4D414E55; //"UNAM" = 55 4E 41 4D
const int32_t cVMAD = 0x44414D56; //"VMAD" = 56 4D 41 44
const int32_t cVNAM = 0x4D414E56; //"VNAM" = 56 4E 41 4D
const int32_t cWEAP = 0x50414557; //"WEAP" = 57 45 41 50
const int32_t cWNAM = 0x4D414E57; //"WNAM" = 57 4E 41 4D
const int32_t cWOOP = 0x504F4F57; //"WOOP" = 57 4F 4F 50
const int32_t cYNAM = 0x4D414E59; //"YNAM" = 59 4E 41 4D
const int32_t cZNAM = 0x4D414E5A; //"ZNAM" = 5A 4E 41 4D

} //namespace

#endif // SR_CONSTANTS_H
