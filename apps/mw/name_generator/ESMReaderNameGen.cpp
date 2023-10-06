/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2023  Dirk Stolle

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

#include "ESMReaderNameGen.hpp"
#include "../../../lib/mw/MW_Constants.hpp"
#include "../../../lib/mw/NPCs.hpp"
#include "../../../lib/mw/Races.hpp"

namespace MWTP
{

int ESMReaderNameGen::processNextRecord(std::istream& input)
{
  // Normally should be 4 chars, but char array is not eligible for switch.
  uint32_t RecordName = 0;
  // read record name
  input.read(reinterpret_cast<char*>(&RecordName), 4);
  switch(RecordName)
  {
    case cNPC_:
         return NPCs::get().readNextRecord(input);
    case cRACE:
         return Races::get().readNextRecord(input);
    default:
         return ESMReader::skipRecord(input);
  }
}

} // namespace
