/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2021  Dirk Stolle

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
#include "Activators.hpp"
#include "Containers.hpp"
#include "Creatures.hpp"
#include "Globals.hpp"
#include "MagicEffects.hpp"
#include "NPCs.hpp"
#include "Scripts.hpp"
#include "Statics.hpp"
#include "MW_Constants.hpp"
#include "HelperIO.hpp"

namespace MWTP
{

int ESMReaderScriptCompiler::processNextRecord(std::istream& in_File)
{
  // normally should be 4 chars, but char array is not eligible for switch
  uint32_t RecordName = 0;
  // read record name
  in_File.read(reinterpret_cast<char*>(&RecordName), 4);
  switch(RecordName)
  {
    case cACTI:
         return Activators::get().readNextRecord(in_File);
    case cCONT:
         return Containers::get().readNextRecord(in_File);
    case cCREA:
         return Creatures::get().readNextRecord(in_File);
    case cGLOB:
         return Globals::get().readNextRecord(in_File);
    case cMGEF:
         return MagicEffects::get().readNextRecord(in_File);
    case cNPC_:
         return NPCs::get().readNextRecord(in_File);
    case cSCPT:
         return Scripts::get().readNextRecord(in_File);
    case cSTAT:
         return Statics::get().readNextRecord(in_File);
    case cALCH:
    case cAPPA:
    case cARMO:
    case cBODY:
    case cBOOK:
    case cBSGN:
    case cCELL:
    case cCLAS:
    case cCLOT:
    case cDIAL:
    case cDOOR:
    case cENCH:
    case cFACT:
    case cGMST:
    case cINFO:
    case cINGR:
    case cLAND:
    case cLEVC:
    case cLEVI:
    case cLIGH:
    case cLOCK:
    case cLTEX:
    case cMISC:
    case cPGRD:
    case cPROB:
    case cRACE:
    case cREGN:
    case cREPA:
    case cSKIL:
    case cSNDG:
    case cSOUN:
    case cSPEL:
    case cSSCR:
    case cWEAP:
         return ESMReader::skipRecord(in_File);
    default:
         std::cerr << "ProcessRecords: ERROR: Unknown record type found: \""
                   << IntTo4Char(RecordName) << "\".\n"
                   << "Current file position: " << in_File.tellg() << " bytes.\n";
         return -1;
  };
}

} // namespace
