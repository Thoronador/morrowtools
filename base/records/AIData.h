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

#ifndef AIDATA_H
#define AIDATA_H

#include <stdint.h>

namespace MWTP
{

/* structure to hold the AI data of an NPC */
struct NPC_AIData
{
  uint8_t Hello;
  uint8_t Unknown1;
  uint8_t Fight;
  uint8_t Flee;
  uint8_t Alarm;
  uint8_t Unknown2;
  uint8_t Unknown3;
  uint8_t Unknown4;
  int32_t Flags;

  bool isPresent;

  /* sets all members to zero */
  void clear();

  /* comparison operator */
  bool operator==(const NPC_AIData& other) const;
};//struct

} //namespace

#endif // AIDATA_H
