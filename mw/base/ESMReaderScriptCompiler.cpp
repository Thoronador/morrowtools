/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012  Thoronador

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

#include "ESMReaderScriptCompiler.hpp"
#include <iostream>
#include "../base/Activators.hpp"
#include "../base/Containers.hpp"
#include "../base/Creatures.hpp"
#include "../base/Globals.hpp"
#include "../base/MagicEffects.hpp"
#include "../base/NPCs.hpp"
#include "../base/Scripts.hpp"
#include "../base/Statics.hpp"
#include "../base/MW_Constants.hpp"
#include "../base/HelperIO.hpp"

namespace MWTP
{

ESMReaderScriptCompiler::ESMReaderScriptCompiler()
{
  //empty
}

ESMReaderScriptCompiler::~ESMReaderScriptCompiler()
{
  //empty
}

int ESMReaderScriptCompiler::processNextRecord(std::ifstream& in_File)
{
  int32_t RecordName = 0; //normally should be 4 char, but char is not eligible for switch
  int lastResult = 0;

  //read record name
  in_File.read((char*) &RecordName, 4);
  switch(RecordName)
  {
    case cACTI:
         lastResult = Activators::getSingleton().readNextRecord(in_File);
         break;
    case cALCH:
    case cAPPA:
    case cARMO:
    case cBODY:
    case cBOOK:
    case cBSGN:
    case cCELL:
    case cCLAS:
    case cCLOT:
         lastResult = ESMReader::skipRecord(in_File);
         break;
    case cCONT:
         lastResult = Containers::getSingleton().readNextRecord(in_File);
         break;
    case cCREA:
         lastResult = Creatures::getSingleton().readNextRecord(in_File);
         break;
    case cDIAL:
    case cDOOR:
    case cENCH:
    case cFACT:
         lastResult = ESMReader::skipRecord(in_File);
         break;
    case cGLOB:
         lastResult = Globals::getSingleton().readNextRecord(in_File);
         break;
    case cGMST:
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
         lastResult = ESMReader::skipRecord(in_File);
         break;
    case cNPC_:
         lastResult = NPCs::getSingleton().readNextRecord(in_File);
         break;
    case cPGRD:
    case cPROB:
    case cRACE:
    case cREGN:
    case cREPA:
         lastResult = ESMReader::skipRecord(in_File);
         break;
    case cSCPT:
         lastResult = Scripts::getSingleton().readNextRecord(in_File);
         break;
    case cSKIL:
    case cSNDG:
    case cSOUN:
    case cSPEL:
    case cSSCR:
         lastResult = ESMReader::skipRecord(in_File);
         break;
    case cSTAT:
         lastResult = Statics::getSingleton().readNextRecord(in_File);
         break;
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
}//processNextRecord of ESMReaderScriptCompiler

} //namespace
