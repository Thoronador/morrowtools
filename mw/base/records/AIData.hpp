/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2021  Dirk Stolle

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

#ifndef MW_AIDATA_HPP
#define MW_AIDATA_HPP

#include <cstdint>

namespace MWTP
{

/** Holds the AI data of an NPC or creature. */
struct NPC_AIData
{
  NPC_AIData();

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

  /** Sets all members to zero. */
  void clear();

  bool operator==(const NPC_AIData& other) const;
}; // struct

} // namespace

#endif // MW_AIDATA_HPP
