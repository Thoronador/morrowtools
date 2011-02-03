/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

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

#ifndef ENCHANTMENT_H
#define ENCHANTMENT_H

#include <stdint.h>

//range types
const int32_t rtSelf = 0;
const int32_t rtTouch = 1;
const int32_t rtTarget = 2;

//single enchantment data
struct EnchantmentData //24 bytes
{
  short int EffectID;
  char SkillID;
  char AttributeID;
  int32_t RangeType;
  int32_t Area;
  int32_t Duration;
  int32_t MagnitudeMin;
  int32_t MagnitudeMax;

  /* returns true, if the other enchantment is the same */
  bool equals(const EnchantmentData& other) const;
}; //struct

#endif // ENCHANTMENT_H
