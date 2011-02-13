/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as published
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

#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H
#include <stdint.h>
#include <string>

const int32_t AttributeStrength = 0;
const int32_t AttributeIntelligence = 1;
const int32_t AttributeWillpower = 2;
const int32_t AttributeAgility = 3;
const int32_t AttributeSpeed = 4;
const int32_t AttributeEndurance = 5;
const int32_t AttributePersonality = 6;
const int32_t AttributeLuck = 7;

namespace Attributes
{
  std::string getSettingNameForAttribute(const int32_t AttributeIndex);
}//namespace

#endif // ATTRIBUTES_H
