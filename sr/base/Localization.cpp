/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021  Dirk Stolle

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

#include "Localization.hpp"
#include <stdexcept>

namespace SRTP
{

std::string stringTableSuffix(const Localization loc)
{
  switch (loc)
  {
    case Localization::English:
         return "english";
    case Localization::French:
         return "french";
    case Localization::German:
         return "german";
    case Localization::Italian:
         return "italian";
    case Localization::Polish:
         return "polish";
    case Localization::Russian:
         return "russian";
    case Localization::Spanish:
         return "spanish";
    default:
         // Keep this here as a guard for possible extensions in future Skyrim editions.
         throw std::logic_error("Encountered unknown Localization enumeration value!");
  }
}

} // namespace
