/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2021, 2022  Dirk Stolle

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

#include "AIData.hpp"
#include <iostream>
#include "../MW_Constants.hpp"

namespace MWTP
{

NPC_AIData::NPC_AIData()
: Hello(0),
  Unknown1(0),
  Fight(0),
  Flee(0),
  Alarm(0),
  Unknown2(0),
  Unknown3(0),
  Unknown4(0),
  Flags(0)
{
}

void NPC_AIData::clear()
{
  Hello = 0;
  Unknown1 = 0;
  Fight = 0;
  Flee = 0;
  Alarm = 0;
  Unknown2 = 0;
  Unknown3 = 0;
  Unknown4 = 0;
  Flags = 0;
}

bool NPC_AIData::operator==(const NPC_AIData& other) const
{
  return (Hello == other.Hello)
      && (Unknown1 == other.Unknown1) && (Fight == other.Fight)
      && (Flee == other.Flee) && (Alarm == other.Alarm)
      && (Unknown2 == other.Unknown2) && (Unknown3 == other.Unknown3)
      && (Unknown4 == other.Unknown4) && (Flags == other.Flags);
}

bool NPC_AIData::loadFromStream(std::istream& input, uint32_t& bytesRead)
{
  // AIDT's length
  uint32_t subLength = 0;
  input.read(reinterpret_cast<char*>(&subLength), 4);
  bytesRead += 4;
  if (subLength != 12)
  {
    std::cerr << "Error: Sub record AIDT has invalid length ("
              << subLength << " bytes). Should be 12 bytes.\n";
    return false;
  }
  // read AI data
  input.read(reinterpret_cast<char*>(&Hello), 1);
  input.read(reinterpret_cast<char*>(&Unknown1), 1);
  input.read(reinterpret_cast<char*>(&Fight), 1);
  input.read(reinterpret_cast<char*>(&Flee), 1);
  input.read(reinterpret_cast<char*>(&Alarm), 1);
  input.read(reinterpret_cast<char*>(&Unknown2), 1);
  input.read(reinterpret_cast<char*>(&Unknown3), 1);
  input.read(reinterpret_cast<char*>(&Unknown4), 1);
  input.read(reinterpret_cast<char*>(&Flags), 4);
  bytesRead += 12;
  return input.good();
}

#ifndef MW_UNSAVEABLE_RECORDS
bool NPC_AIData::saveToStream(std::ostream& output) const
{
  // write AI data (AIDT)
  output.write(reinterpret_cast<const char*>(&cAIDT), 4);
  const uint32_t subLength = 12;
  output.write(reinterpret_cast<const char*>(&subLength), 4);
  // write actual data
  output.write(reinterpret_cast<const char*>(&Hello), 1);
  output.write(reinterpret_cast<const char*>(&Unknown1), 1);
  output.write(reinterpret_cast<const char*>(&Fight), 1);
  output.write(reinterpret_cast<const char*>(&Flee), 1);
  output.write(reinterpret_cast<const char*>(&Alarm), 1);
  output.write(reinterpret_cast<const char*>(&Unknown2), 1);
  output.write(reinterpret_cast<const char*>(&Unknown3), 1);
  output.write(reinterpret_cast<const char*>(&Unknown4), 1);
  output.write(reinterpret_cast<const char*>(&Flags), 4);
  return output.good();
}
#endif

} // namespace
