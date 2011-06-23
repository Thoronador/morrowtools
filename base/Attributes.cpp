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

#include "Attributes.h"
#include <iostream>

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
         throw 42;
         break;
  }//swi
}

} //namespace Attributes

} //namespace MWTP
