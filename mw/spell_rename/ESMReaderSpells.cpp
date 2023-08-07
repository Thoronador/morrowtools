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

#include "ESMReaderSpells.hpp"
#include <iostream>
#include "../../lib/mw/MW_Constants.hpp"
#include "../../lib/mw/HelperIO.hpp"
#include "../../lib/mw/GameSettings.hpp"
#include "../../lib/mw/MagicEffects.hpp"
#include "../../lib/mw/Spells.hpp"

namespace MWTP
{

int ESMReaderSpells::processNextRecord(std::istream& input)
{
  // Normally should be 4 chars, but char array is not eligible for switch.
  uint32_t RecordName = 0;
  // read record name
  input.read(reinterpret_cast<char*>(&RecordName), 4);
  switch(RecordName)
  {
    case cGMST:
         return GameSettings::get().readNextRecord(input);
    case cMGEF:
         return MagicEffects::get().readNextRecord(input);
    case cSPEL:
         return Spells::get().readNextRecord(input);
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
    case cINFO:
    case cINGR:
    case cLAND:
    case cLEVC:
    case cLEVI:
    case cLIGH:
    case cLOCK:
    case cLTEX:
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
    case cSSCR:
    case cSTAT:
    case cWEAP:
         return ESMReader::skipRecord(input);
    default:
         std::cout << "ProcessRecords: ERROR: Unknown record type found: \""
                   << IntTo4Char(RecordName) << "\".\n"
                   << "Current file position: " << input.tellg() << " bytes.\n";
         return -1;
  }
}

} // namespace
