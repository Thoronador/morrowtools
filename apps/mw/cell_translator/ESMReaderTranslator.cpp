/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2014  Dirk Stolle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "ESMReaderTranslator.hpp"

#include <iostream>
#include <stdexcept>
#include "../../../lib/mw/HelperIO.hpp"
#include "../../../lib/mw/MW_Constants.hpp"
#include "../../../lib/mw/records/GenericRecord.hpp"
#include "../../../lib/mw/records/CellRecord.hpp"
#include "../../../lib/mw/records/CreatureRecord.hpp"
#include "../../../lib/mw/records/DialogueInfoRecord.hpp"
#include "../../../lib/mw/records/NPCRecord.hpp"
#include "../../../lib/mw/records/PathGridRecord.hpp"
#include "../../../lib/mw/records/ScriptRecord.hpp"

namespace MWTP
{

ESMReaderTranslator::ESMReaderTranslator(VectorType* vec)
: ESMReaderGeneric(vec)
{
  if (NULL==vec)
  {
    std::cout << "ESMReaderTranslator: Error: supplied pointer is NULL!\n";
    throw std::runtime_error("ESMReaderTranslator: Error: supplied pointer is NULL!");
  }
  m_VectorPointer = vec;
}

ESMReaderTranslator::~ESMReaderTranslator()
{
  //empty
}

int ESMReaderTranslator::processNextRecord(std::istream& in_File)
{
  uint32_t RecordName = 0; //normally should be 4 char, but char is not eligible for switch
  int lastResult = 0;

  GenericRecord* genRec = NULL;
  BasicRecord* baseRec = NULL;
  //read record name
  in_File.read((char*) &RecordName, 4);
  switch(RecordName)
  {
    case cCELL: //has cell names
         baseRec = new CellRecord;
         if (baseRec->loadFromStream(in_File))
         {
           m_VectorPointer->push_back(baseRec);
           return 1;
         }
         else
         {
           delete baseRec;
           std::cout << "Error: failed to load cell record!\n!";
           return -1;
         }
         break;
    case cCREA: //has cell info (AI and dest.)
         baseRec = new CreatureRecord;
         if (baseRec->loadFromStream(in_File))
         {
           m_VectorPointer->push_back(baseRec);
           return 1;
         }
         else
         {
           delete baseRec;
           std::cout << "Error: failed to load creature record!\n!";
           return -1;
         }
         break;
    case cINFO: //has cell info, but isn't processed yet
         baseRec = new DialogueInfoRecord;
         if (baseRec->loadFromStream(in_File))
         {
           m_VectorPointer->push_back(baseRec);
           return 1;
         }
         else
         {
           delete baseRec;
           std::cout << "Error: failed to load dialogue info record!\n!";
           return -1;
         }
         break;
    case cNPC_: //has cell info (AI and dest)
         baseRec = new NPCRecord;
         if (baseRec->loadFromStream(in_File))
         {
           m_VectorPointer->push_back(baseRec);
           return 1;
         }
         else
         {
           delete baseRec;
           std::cout << "Error: failed to load NPC record!\n!";
           return -1;
         }
         break;
    case cPGRD: //has cell info (cell name)
         baseRec = new PathGridRecord;
         if (baseRec->loadFromStream(in_File))
         {
           m_VectorPointer->push_back(baseRec);
           return 1;
         }
         else
         {
           delete baseRec;
           std::cout << "Error: failed to load path grid record!\n!";
           return -1;
         }
         break;
    case cSCPT: //might have cell names in script text
         baseRec = new ScriptRecord;
         if (baseRec->loadFromStream(in_File))
         {
           m_VectorPointer->push_back(baseRec);
           return 1;
         }
         else
         {
           delete baseRec;
           std::cout << "Error: failed to load script record!\n!";
           return -1;
         }
         break;
    case cACTI: //no cell names
    case cALCH: //no cell names
    case cAPPA: //no cell names
    case cARMO: //no cell names
    case cBODY: //no cell names
    case cBOOK: //no cell names
    case cBSGN: //no cell names
    case cCLAS: //no cell names
    case cCLOT: //no cell names
    case cCONT: //no cell names
    case cDIAL: //no cell names
    case cDOOR: //no cell names
    case cENCH: //no cell names
    case cFACT: //no cell names
    case cGLOB: //no cell names
    case cGMST: //no cell names(?)
    case cINGR: //no cell names
    case cLAND: //no cell names (but cell offset)
    case cLEVC: //no cell names
    case cLEVI: //no cell names
    case cLIGH: //no cell names
    case cLOCK: //no cell names
    case cLTEX: //no cell names
    case cMGEF: //no cell names
    case cMISC: //no cell names
    case cPROB: //no cell names
    case cRACE: //no cell names
    case cREGN: //no cell names, but region name that could be translated later
    case cREPA: //no cell names
    case cSKIL: //no cell names
    case cSNDG: //no cell names
    case cSOUN: //no cell names
    case cSPEL: //no cell names
    case cSSCR: //no cell names
    case cSTAT: //no cell names
    case cWEAP: //no cell names
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
         break;
    default:
         std::cout << "processNextRecord: ERROR: unknown record type found: \""
                   << IntTo4Char(RecordName)<<"\".\n"
                   << "Current file position: "<<in_File.tellg()<< " bytes.\n";
         lastResult = -1;
         break;
  }
  return lastResult;
}

} //namespace
