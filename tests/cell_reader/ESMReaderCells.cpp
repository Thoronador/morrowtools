/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for the Morrowind Tools Project.
    Copyright (C) 2022  Dirk Stolle

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

#include "ESMReaderCells.hpp"
#include <iostream>
#include "../../mw/base/Cells.hpp"
#include "../../mw/base/HelperIO.hpp"
#include "../../mw/base/MW_Constants.hpp"

namespace MWTP
{

int ESMReaderCells::processNextRecord(std::istream& input)
{
  // Normally should be 4 chars, but char array is not eligible for switch.
  uint32_t RecordName = 0;
  // read record name
  input.read(reinterpret_cast<char*>(&RecordName), 4);
  switch(RecordName)
  {
    case cCELL:
         return Cells::get().readRecordCELL(input);
    case cACTI:
    case cALCH:
    case cAPPA:
    case cARMO:
    case cBODY:
    case cBOOK:
    case cBSGN:
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
         return ESMReader::skipRecord(input);
    default:
         std::cerr << "ESMReader::processNextRecord: ERROR: Unknown record type found: \""
                   << IntTo4Char(RecordName) << "\".\n"
                   << "Current file position: " << input.tellg() << " bytes.\n";
         return -1;
  }
}

} // namespace
