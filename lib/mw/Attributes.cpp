/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2014  Dirk Stolle

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

#include "Attributes.hpp"
#include <iostream>
#include <stdexcept>
#include "../base/UtilityFunctions.hpp"

namespace MWTP
{

namespace Attributes
{

std::string getSettingNameForAttribute(const int32_t AttributeIndex)
{
  switch(AttributeIndex)
  {
    case AttributeStrength:
         return "sAttributeStrength";
         break;
    case AttributeIntelligence:
         return "sAttributeIntelligence";
         break;
    case AttributeWillpower:
         return "sAttributeWillpower";
         break;
    case AttributeAgility:
         return "sAttributeAgility";
         break;
    case AttributeSpeed:
         return "sAttributeSpeed";
         break;
    case AttributeEndurance:
         return "sAttributeEndurance";
         break;
    case AttributeLuck:
         return "sAttributeLuck";
         break;
    case AttributePersonality:
         return "sAttributePersonality";
         break;
    default:
         std::cout << "No setting defined for attribute index "<<AttributeIndex<<".\n";
         throw std::runtime_error(std::string("getSettingNameForAttribute(): No setting defined for attribute index ")
                                  + intToString(AttributeIndex) + std::string("."));
         break;
  }//swi
}

} //namespace Attributes

} //namespace MWTP
