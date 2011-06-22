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

#include "TravelDestination.h"

namespace MWTP
{

/* **** TravelDestination's functions ****/

bool TravelDestination::operator==(const TravelDestination& other) const
{
  return ((XPos==other.XPos) and (YPos==other.YPos) and (ZPos==other.ZPos)
      and (XRot==other.XRot) and (YRot==other.YRot) and (ZRot==other.ZRot)
      and (CellName==other.CellName));
}

} //namespace
