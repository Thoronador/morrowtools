/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013, 2021, 2022  Dirk Stolle

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

#include "TravelDestination.hpp"
#include <iostream>
#include "../MW_Constants.hpp"

namespace MWTP
{

TravelDestination::TravelDestination()
: XPos(0.0f), YPos(0.0f), ZPos(0.0f),
  XRot(0.0f), YRot(0.0f), ZRot(0.0f),
  CellName("")
{ }

bool TravelDestination::operator==(const TravelDestination& other) const
{
  return (XPos == other.XPos) && (YPos == other.YPos) && (ZPos == other.ZPos)
      && (XRot == other.XRot) && (YRot == other.YRot) && (ZRot == other.ZRot)
      && (CellName == other.CellName);
}

bool TravelDestination::partialLoadFromStream(std::istream& input, uint32_t& bytesRead)
{
  // DODT's length
  uint32_t subLength = 0;
  input.read(reinterpret_cast<char*>(&subLength), 4);
  bytesRead += 4;
  if (subLength != 24)
  {
    std::cerr << "Error: Sub record DODT of NPC_ or CREA has invalid length ("
              << subLength << " bytes). Should be 24 bytes.\n";
    return false;
  }
  // read destination data
  input.read(reinterpret_cast<char*>(&XPos), 4);
  input.read(reinterpret_cast<char*>(&YPos), 4);
  input.read(reinterpret_cast<char*>(&ZPos), 4);
  input.read(reinterpret_cast<char*>(&XRot), 4);
  input.read(reinterpret_cast<char*>(&YRot), 4);
  input.read(reinterpret_cast<char*>(&ZRot), 4);
  bytesRead += 24;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record DODT of NPC_ or CREA!\n";
    return false;
  }
  CellName.clear();

  return true;
}

#ifndef MW_UNSAVEABLE_RECORDS
uint32_t TravelDestination::getStreamSize() const
{
  uint32_t Size = 4 /* DODT */ + 4 /* 4 bytes for length */ + 24 /*fixed length of 24 bytes */;
  if (!CellName.empty())
  {
    Size += 4 /* DNAM */ + 4 /* 4 bytes for length */
          + CellName.length() + 1 /* length of cell name +1 byte for NUL */;
  }
  return Size;
}

bool TravelDestination::saveToStream(std::ostream& output) const
{
  // write DODT
  output.write(reinterpret_cast<const char*>(&cDODT), 4);
  uint32_t subLength = 24;
  output.write(reinterpret_cast<const char*>(&subLength), 4);
  // write destination data
  output.write(reinterpret_cast<const char*>(&XPos), 4);
  output.write(reinterpret_cast<const char*>(&YPos), 4);
  output.write(reinterpret_cast<const char*>(&ZPos), 4);
  output.write(reinterpret_cast<const char*>(&XRot), 4);
  output.write(reinterpret_cast<const char*>(&YRot), 4);
  output.write(reinterpret_cast<const char*>(&ZRot), 4);
  // see if there's a cell name, too
  if (!CellName.empty())
  {
    // write destination cell name (DNAM)
    output.write(reinterpret_cast<const char*>(&cDNAM), 4);
    subLength = CellName.length() + 1; // length of cell name +1 byte for NUL
    output.write(reinterpret_cast<const char*>(&subLength), 4);
    output.write(CellName.c_str(), subLength);
  }

  return output.good();
}
#endif // MW_UNSAVEABLE_RECORDS

} // namespace
