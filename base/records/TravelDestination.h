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

#ifndef TRAVELDESTINATION_H
#define TRAVELDESTINATION_H

#include <string>

namespace MWTP
{

struct TravelDestination
{
  float XPos, YPos, ZPos;
  float XRot, YRot, ZRot;
  std::string CellName;//is usually empty for exterior cells

  /* comparison operator */
  bool operator==(const TravelDestination& other) const;
};//struct

} //namespace

#endif // TRAVELDESTINATION_H
