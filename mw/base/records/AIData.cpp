/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2021  Thoronador

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
  Flags(0),
  isPresent(false)
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

  isPresent = false;
}

bool NPC_AIData::operator==(const NPC_AIData& other) const
{
  if (!isPresent && !other.isPresent)
    return true;
  return (isPresent == other.isPresent) && (Hello == other.Hello)
      && (Unknown1 == other.Unknown1) && (Fight == other.Fight)
      && (Flee == other.Flee) && (Alarm == other.Alarm)
      && (Unknown2 == other.Unknown2) && (Unknown3 == other.Unknown3)
      && (Unknown4 == other.Unknown4) && (Flags == other.Flags);
}

} // namespace
