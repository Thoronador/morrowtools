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

#ifndef MW_CONSTANTS_H
#define MW_CONSTANTS_H

#include <stdint.h>

const int32_t cAADT = 0x54444141; //"AADT" = 41 41 44 54
const int32_t cACTI = 0x49544341; //"ACTI" = 41 43 54 49
const int32_t cAIDT = 0x54444941; //"AIDT" = 41 49 44 54
const int32_t cAI_A = 0x415F4941; //"AI_A" = 41 49 5F 41
const int32_t cAI_E = 0x455F4941; //"AI_E" = 41 49 5F 45
const int32_t cAI_F = 0x465F4941; //"AI_F" = 41 49 5F 46
const int32_t cAI_T = 0x545F4941; //"AI_T" = 41 49 5F 54
const int32_t cAI_W = 0x575F4941; //"AI_W" = 41 49 5F 57
const int32_t cALCH = 0x48434C41; //"ALCH" = 41 4C 43 48
const int32_t cALDT = 0x54444C41; //"ALDT" = 41 4C 44 54
const int32_t cAMBI = 0x49424D41; //"AMBI" = 41 4D 42 49
const int32_t cANAM = 0x4D414E41; //"ANAM" = 41 4E 41 4D
const int32_t cAODT = 0x54444F41; //"AODT" = 41 4F 44 54
const int32_t cAPPA = 0x41505041; //"APPA" = 41 50 50 41
const int32_t cARMO = 0x4F4D5241; //"ARMO" = 41 52 4D 4F
const int32_t cASND = 0x444E5341; //"ASND" = 41 53 4E 44
const int32_t cAVFX = 0x58465641; //"AVFX" = 41 56 46 58
const int32_t cBKDT = 0x54444B42; //"BKDT" = 42 4B 44 54
const int32_t cBNAM = 0x4D414E42; //"BNAM" = 42 4E 41 4D
const int32_t cBODY = 0x59444F42; //"BODY" = 42 4F 44 59
const int32_t cBOOK = 0x4B4F4F42; //"BOOK" = 42 4F 4F 4B
const int32_t cBSGN = 0x4E475342; //"BSGN" = 42 53 47 4E
const int32_t cBSND = 0x444E5342; //"BSND" = 42 53 4E 44
const int32_t cBYDT = 0x54445942; //"BYDT" = 42 59 44 54
const int32_t cBVFX = 0x58465642; //"BVFX" = 42 56 46 58
const int32_t cCELL = 0x4C4C4543; //"CELL" = 43 45 4C 4C
const int32_t cCLAS = 0x53414C43; //"CLAS" = 43 4C 41 53
const int32_t cCLDT = 0x54444C43; //"CLDT" = 43 4C 44 54
const int32_t cCLOT = 0x544F4C43; //"CLOT" = 43 4C 4F 54
const int32_t cCNAM = 0x4D414E43; //"CNAM" = 43 4E 41 4D
const int32_t cCNDT = 0x54444E43; //"CNDT" = 43 4E 44 54
const int32_t cCONT = 0x544E4F43; //"CONT" = 43 4F 4E 54
const int32_t cCREA = 0x41455243; //"CREA" = 43 52 45 41
const int32_t cCSND = 0x444E5343; //"CSND" = 43 53 4E 44
const int32_t cCTDT = 0x54445443; //"CTDT" = 43 54 44 54
const int32_t cCVFX = 0x58465643; //"CVFX" = 43 56 46 58
const int32_t cDATA = 0x41544144; //"DATA" = 44 41 54 41
const int32_t cDESC = 0x43534544; //"DESC" = 44 45 53 43
const int32_t cDIAL = 0x4C414944; //"DIAL" = 44 49 41 4C
const int32_t cDNAM = 0x4D414E44; //"DNAM" = 44 4E 41 4D
const int32_t cDODT = 0x54444F44; //"DODT" = 44 4F 44 54
const int32_t cDOOR = 0x524F4F44; //"DOOR" = 44 4F 4F 52
const int32_t cENAM = 0x4D414E45; //"ENAM" = 45 4E 41 4D
const int32_t cENCH = 0x48434E45; //"ENCH" = 45 4E 43 48
const int32_t cENDT = 0x54444E45; //"ENDT" = 45 4E 44 54
const int32_t cFACT = 0x54434146; //"FACT" = 46 41 43 54
const int32_t cFADT = 0x54444146; //"FADT" = 46 41 44 54
const int32_t cFLAG = 0x47414C46; //"FLAG" = 46 4C 41 47
const int32_t cFLTV = 0x56544C46; //"FLTV" = 46 4C 54 56
const int32_t cFNAM = 0x4D414E46; //"FNAM" = 46 4E 41 4D
const int32_t cFRMR = 0x524D5246; //"FRMR" = 46 52 4D 52
const int32_t cGLOB = 0x424F4C47; //"GLOB" = 47 4C 4F 42
const int32_t cGMST = 0x54534D47; //"GMST" = 47 4D 53 54
const int32_t cHEDR = 0x52444548; //"HEDR" = 48 45 44 52
const int32_t cHSND = 0x444E5348; //"HSND" = 48 53 4E 44
const int32_t cHVFX = 0x58465648; //"HVFX" = 48 56 46 58
const int32_t cINAM = 0x4D414E49; //"INAM" = 49 4E 41 4D
const int32_t cINDX = 0x58444E49; //"INDX" = 49 4E 44 58
const int32_t cINFO = 0x4F464E49; //"INFO" = 49 4E 46 4F
const int32_t cINGR = 0x52474E49; //"INGR" = 49 4E 47 52
const int32_t cINTV = 0x56544E49; //"INTV" = 49 4E 54 56
const int32_t cIRDT = 0x54445249; //"IRDT" = 49 52 44 54
const int32_t cITEX = 0x58455449; //"ITEX" = 49 54 45 58
const int32_t cKNAM = 0x4D414E4B; //"KNAM" = 4B 4E 41 4D
const int32_t cLAND = 0x444E414C; //"LAND" = 4C 41 4E 44
const int32_t cLEVC = 0x4356454C; //"LEVC" = 4C 45 56 43
const int32_t cLEVI = 0x4956454C; //"LEVI" = 4C 45 56 49
const int32_t cLHDT = 0x5444484C; //"LHDT" = 4C 48 44 54
const int32_t cLIGH = 0x4847494C; //"LIGH" = 4C 49 47 48
const int32_t cLKDT = 0x54444B4C; //"LKDT" = 4C 4B 44 54
const int32_t cLOCK = 0x4B434F4C; //"LOCK" = 4C 4F 43 4B
const int32_t cLTEX = 0x5845544C; //"LTEX" = 4C 54 45 58
const int32_t cMAST = 0x5453414D; //"MAST" = 4D 41 53 54
const int32_t cMCDT = 0x5444434D; //"MCDT" = 4D 43 44 54
const int32_t cMEDT = 0x5444454D; //"MEDT" = 4D 45 44 54
const int32_t cMGEF = 0x4645474D; //"MGEF" = 4D 47 45 46
const int32_t cMISC = 0x4353494D; //"MISC" = 4D 49 53 43
const int32_t cMODL = 0x4C444F4D; //"MODL" = 4D 4F 44 4C
const int32_t cNAM0 = 0x304D414E; //"NAM0" = 4E 41 4D 30
const int32_t cNAM5 = 0x354D414E; //"NAM5" = 4E 41 4D 35
const int32_t cNAME = 0x454D414E; //"NAME" = 4E 41 4D 45
const int32_t cNNAM = 0x4D414E4E; //"NNAM" = 4E 4E 41 4D
const int32_t cNPCO = 0x4F43504E; //"NPCO" = 4E 50 43 4F
const int32_t cNPCS = 0x5343504E; //"NPCS" = 4E 50 43 53
const int32_t cNPC_ = 0x5F43504E; //"NPC_" = 4E 50 43 5F
const int32_t cNPDT = 0x5444504E; //"NPDT" = 4E 50 44 54
const int32_t cONAM = 0x4D414E4F; //"ONAM" = 4F 4E 41 4D
const int32_t cPBDT = 0x54444250; //"PBDT" = 50 42 44 54
const int32_t cPGRC = 0x43524750; //"PGRC" = 50 47 52 43
const int32_t cPGRD = 0x44524750; //"PGRD" = 50 47 52 44
const int32_t cPGRP = 0x50524750; //"PGRP" = 50 47 52 50
const int32_t cPROB = 0x424F5250; //"PROB" = 50 52 4F 42
const int32_t cPNAM = 0x4D414E50; //"PNAM" = 50 4E 41 4D
const int32_t cPTEX = 0x58455450; //"PTEX" = 50 54 45 58
const int32_t cQSTF = 0x46545351; //"QSTF" = 51 53 54 46
const int32_t cQSTN = 0x4E545351; //"QSTN" = 51 53 54 4E
const int32_t cQSTR = 0x52545351; //"QSTR" = 51 53 54 52
const int32_t cRACE = 0x45434152; //"RACE" = 52 41 43 45
const int32_t cRADT = 0x54444152; //"RADT" = 52 41 44 54
const int32_t cREGN = 0x4E474552; //"REGN" = 52 45 47 4E
const int32_t cREPA = 0x41504552; //"REPA" = 52 45 50 41
const int32_t cRGNN = 0x4E4E4752; //"RGNN" = 52 47 4E 4E
const int32_t cRIDT = 0x54444952; //"RIDT" = 52 49 44 54
const int32_t cRNAM = 0x4D414E52; //"RNAM" = 52 4E 41 4D
const int32_t cSCDT = 0x54444353; //"SCDT" = 53 43 44 54
const int32_t cSCHD = 0x44484353; //"SCHD" = 53 43 48 44
const int32_t cSCPT = 0x54504353; //"SCPT" = 53 43 50 54
const int32_t cSCRI = 0x49524353; //"SCRI" = 53 43 52 49
const int32_t cSCTX = 0x58544353; //"SCTX" = 53 43 54 58
const int32_t cSCVR = 0x52564353; //"SCVR" = 53 43 56 52
const int32_t cSKDT = 0x54444B53; //"SKDT" = 53 4B 44 54
const int32_t cSKIL = 0x4C494B53; //"SKIL" = 53 4B 49 4C
const int32_t cSNAM = 0x4D414E53; //"SNAM" = 53 4E 41 4D
const int32_t cSNDG = 0x47444E53; //"SNDG" = 53 4E 44 47
const int32_t cSOUN = 0x4E554F53; //"SOUN" = 53 4F 55 4E
const int32_t cSPDT = 0x54445053; //"SPDT" = 53 50 44 54
const int32_t cSPEL = 0x4C455053; //"SPEL" = 53 50 45 4C
const int32_t cSSCR = 0x52435353; //"SSCR" = 53 53 43 52
const int32_t cSTAT = 0x54415453; //"STAT" = 53 54 41 54
const int32_t cSTRV = 0x56525453; //"STRV" = 53 54 52 56
const int32_t cTES3 = 0x33534554; //"TES3" = 54 45 53 33
const int32_t cTNAM = 0x4D414E54; //"TNAM" = 54 4E 41 4D
const int32_t cTEXT = 0x54584554; //"TEXT" = 54 45 58 54
const int32_t cVCLR = 0x524C4356; //"VCLR" = 56 43 4C 52
const int32_t cVHGT = 0x54474856; //"VHGT" = 56 48 47 54
const int32_t cVNML = 0x4C4D4E56; //"VNML" = 56 4E 4D 4C
const int32_t cVTEX = 0x58455456; //"VTEX" = 56 54 45 58
const int32_t cWEAP = 0x50414557; //"WEAP" = 57 45 41 50
const int32_t cWEAT = 0x54414557; //"WEAT" = 57 45 41 54
const int32_t cWHGT = 0x54474857; //"WHGT" = 57 48 47 54
const int32_t cWPDT = 0x54445057; //"WPDT" = 57 50 44 54
const int32_t cWNAM = 0x4D414E57; //"WNAM" = 57 4E 41 4D
const int32_t cXSCL = 0x4C435358; //"XSCL" = 58 53 43 4C

#endif // MW_CONSTANTS_H
