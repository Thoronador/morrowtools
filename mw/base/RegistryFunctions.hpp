/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2012 Thoronador

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

#ifndef MW_REGISTRYFUNCTIONS_HPP
#define MW_REGISTRYFUNCTIONS_HPP

#include <string>
#include "../../base/RegistryFunctions.hpp"

namespace MWTP
{

/* tries to read Morrowind's install path from the Windows registry. In case of
   success, the function will return true and the retrieved path will be stored
   in the string referenced by pathData. In case of failure, the function will
   return false and the string referenced by pathData will not be changed.

   parameters:
       pathData - reference to a string that shall hold the read path

   remarks:
       This function will always return false on non-Win32-OSes.
*/
inline bool getMorrowindPathFromRegistry(std::string& pathData)
{
  return getRegistryStringValueHKLM(pathData, "SOFTWARE\\Bethesda Softworks\\Morrowind", "Installed Path");
}

} //namespace

#endif // MW_REGISTRYFUNCTIONS_HPP
