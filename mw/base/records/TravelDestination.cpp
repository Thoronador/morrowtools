/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013  Thoronador

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

#include "TravelDestination.h"

namespace MWTP
{

/* **** TravelDestination's functions ****/

TravelDestination::TravelDestination()
: XPos(0.0f), YPos(0.0f), ZPos(0.0f),
  XRot(0.0f), YRot(0.0f), ZRot(0.0f),
  CellName("")
{ }

bool TravelDestination::operator==(const TravelDestination& other) const
{
  return ((XPos==other.XPos) and (YPos==other.YPos) and (ZPos==other.ZPos)
      and (XRot==other.XRot) and (YRot==other.YRot) and (ZRot==other.ZRot)
      and (CellName==other.CellName));
}

} //namespace
