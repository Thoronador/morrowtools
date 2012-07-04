/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012  Thoronador

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

#include "ESMReaderSpells.h"
#include <iostream>
#include "../base/MW_Constants.h"
#include "../base/HelperIO.h"
#include "../base/GameSettings.h"
#include "../base/MagicEffects.h"
#include "../base/Spells.h"

namespace MWTP
{

ESMReaderSpells::ESMReaderSpells()
{
  //empty
}

ESMReaderSpells::~ESMReaderSpells()
{
  //empty
}

int ESMReaderSpells::processNextRecord(std::ifstream& in_File)
{
  int32_t RecordName = 0; //normally should be 4 char, but char is not eligible for switch
  int lastResult = 0;

  //read record name
  in_File.read((char*) &RecordName, 4);
  switch(RecordName)
  {
    case cACTI:
    case cALCH:
    case cAPPA:
    case cARMO:
    case cBODY:
    case cBOOK:
    case cBSGN:
    case cCELL:
    case cCLAS:
    case cCLOT:
    case cCONT:
    case cCREA:
    case cDIAL:
    case cDOOR:
    case cENCH:
    case cFACT:
    case cGLOB:
         lastResult = ESMReader::skipRecord(in_File);
         break;
    case cGMST:
         lastResult = GameSettings::getSingleton().readRecordGMST(in_File);
         break;
    case cINFO:
    case cINGR:
    case cLAND:
    case cLEVC:
    case cLEVI:
    case cLIGH:
    case cLOCK:
    case cLTEX:
         lastResult = ESMReader::skipRecord(in_File);
         break;
    case cMGEF:
         lastResult = MagicEffects::getSingleton().readRecordMGEF(in_File);
         break;
    case cMISC:
    case cNPC_:
    case cPGRD:
    case cPROB:
    case cRACE:
    case cREGN:
    case cREPA:
    case cSCPT:
    case cSKIL:
    case cSNDG:
    case cSOUN:
         lastResult = ESMReader::skipRecord(in_File);
         break;
    case cSPEL:
         lastResult = Spells::getSingleton().readNextRecord(in_File);
         break;
    case cSSCR:
    case cSTAT:
    case cWEAP:
         lastResult = ESMReader::skipRecord(in_File);
         break;
    default:
         std::cout << "ProcessRecords: ERROR: unknown record type found: \""
                   <<IntTo4Char(RecordName)<<"\".\n"
                   << "Current file position: "<<in_File.tellg()<< " bytes.\n";
         lastResult = -1;
         break;
  }//swi
  return lastResult;
}//processNextRecord of ESMReaderSpells

} //namespace
