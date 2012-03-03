/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2012 Thoronador

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

#include "ESMReaderSkillRebalance.h"
#include <iostream>
#include "../base/MW_Constants.h"
#include "../base/HelperIO.h"
#include "../base/Skills.h"

namespace MWTP
{

ESMReaderSkillRebalance::ESMReaderSkillRebalance()
{
  //empty
}

ESMReaderSkillRebalance::~ESMReaderSkillRebalance()
{
  //empty
}

int ESMReaderSkillRebalance::processNextRecord(std::ifstream& in_File)
{
  int32_t RecordName = 0; //normally should be 4 char, but char is not eligible for switch
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
    case cGMST:
    case cINFO:
    case cINGR:
    case cLAND:
    case cLEVC:
    case cLEVI:
    case cLIGH:
    case cLOCK:
    case cLTEX:
    case cMGEF:
    case cMISC:
    case cNPC_:
    case cPGRD:
    case cPROB:
    case cRACE:
    case cREGN:
    case cREPA:
    case cSCPT:
    case cSNDG:
    case cSOUN:
    case cSPEL:
    case cSSCR:
    case cSTAT:
    case cWEAP:
         return skipRecord(in_File);
         break;
    case cSKIL:
         return Skills::getSingleton().readRecordSKIL(in_File);
         break;
    default:
         std::cout << "processNextRecord: ERROR: unknown record type found: \""
                   <<IntTo4Char(RecordName)<<"\".\n"
                   << "Current file position: "<<in_File.tellg()<< " bytes.\n";
         return -1;
         break;
  }//swi
}

} //namespace
