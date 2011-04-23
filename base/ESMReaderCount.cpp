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

#include "ESMReaderCount.h"
#include <iostream>
#include "MW_Constants.h"
#include "HelperIO.h"

ESMReaderCount::ESMReaderCount()
{
  //empty
}

ESMReaderCount::~ESMReaderCount()
{
  RecordCounter.clear();
}

int ESMReaderCount::processNextRecord(std::ifstream& in_File)
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
    case cSKIL:
    case cSNDG:
    case cSOUN:
    case cSPEL:
    case cSSCR:
    case cSTAT:
    case cWEAP:
         lastResult = ESMReader::skipRecord(in_File);
         ++RecordCounter[RecordName];
         break;
    default:
         std::cout << "processNextRecord: ERROR: unknown record type found: \""
                   <<IntTo4Char(RecordName)<<"\".\n"
                   << "Current file position: "<<in_File.tellg()<< " bytes.\n";
         lastResult = -1;
         break;
  }//swi
  return lastResult;
}//processNextRecord of ESMReaderCount class

void ESMReaderCount::showStats(const bool withPercentage) const
{
  std::map<int32_t, unsigned int>::const_iterator iter;
  unsigned int sum = 1;
  if (withPercentage)
  {
    sum = 0;
    iter = RecordCounter.begin();
    while (iter!=RecordCounter.end())
    {
      sum += iter->second;
      ++iter;
    }//while
    if (sum==0) sum = 1;//avoid div. by zero
  }//if
  iter = RecordCounter.begin();
  while (iter!=RecordCounter.end())
  {
    std::cout << IntTo4Char(iter->first) << ": "<<iter->second;
    if (withPercentage)
    {
      std::cout << " ("<<(iter->second*100.0f/static_cast<float>(sum))<<"%)";
    }
    std::cout <<"\n";
    ++iter;
  }//while
  std::cout << "There are "<<RecordCounter.size()<<" distinct record types.\n";
}
