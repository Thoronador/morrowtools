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

#include "AIData.h"

namespace MWTP
{

/* *** AIData's functions ****/

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
  if (!isPresent and !other.isPresent) return true;
  return ((isPresent==other.isPresent) and (Hello==other.Hello)
      and (Unknown1==other.Unknown1) and (Fight==other.Fight)
      and (Flee==other.Flee) and (Alarm==other.Alarm)
      and (Unknown2==other.Unknown2) and (Unknown3==other.Unknown3)
      and (Unknown4==other.Unknown4) and (Flags==other.Flags));
}

} //namespace
