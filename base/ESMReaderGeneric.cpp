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

#include "ESMReaderGeneric.h"
#include <iostream>
#include "HelperIO.h"
#include "MW_Constants.h"
#include "records/GenericRecord.h"

ESMReaderGeneric::ESMReaderGeneric(VectorType* vec)
{
  if (NULL==vec)
  {
    std::cout << "ESMReaderGeneric: Error: supplied pointer is NULL!\n";
    throw 42;
  }
  m_VectorPointer = vec;
}

ESMReaderGeneric::~ESMReaderGeneric()
{
  //empty
}

int ESMReaderGeneric::processNextRecord(std::ifstream& in_File)
{
  int32_t RecordName = 0; //normally should be 4 char, but char is not eligible for switch
  int lastResult = 0;

  GenericRecord* genRec = NULL;
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
         genRec = new GenericRecord;
         if (genRec->loadFromStream(in_File))
         {
           genRec->Header = RecordName;
           m_VectorPointer->push_back(genRec);
           return 1;
         }
         else
         {
           delete genRec;
           std::cout << "Error: failed to load generic record!\n!";
           return -1;
         }
    default:
         std::cout << "processNextRecord: ERROR: unknown record type found: \""
                   <<IntTo4Char(RecordName)<<"\".\n"
                   << "Current file position: "<<in_File.tellg()<< " bytes.\n";
         lastResult = -1;
         break;
  }//swi
  return lastResult;
}//processNextRecord of ESMReaderGeneric class

void ESMReaderGeneric::deallocateRecordsInVector()
{
  while (!(m_VectorPointer->empty()))
  {
    BasicRecord* ptr = m_VectorPointer->back();
    if (ptr!=NULL)
    {
      delete ptr;
    }
    m_VectorPointer->pop_back();
  }//while
}
