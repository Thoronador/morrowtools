/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#ifndef MWTP_BASE_REGISTRYFUNCTIONS_H
#define MWTP_BASE_REGISTRYFUNCTIONS_H

#include <string>

namespace MWTP
{

/* tries to read a string value from the HKEY_LOCAL_MACHINE branch of the
   Windows registry. In case of success, the funtion will return true and the
   retrieved path will be stored in the string referenced by theString. In case
   of failure, the function will return false and the string referenced by
   theString will not be changed.

   parameters:
       theString - reference to a string that shall hold the read string value
       subKey    - name of the registry key in HKEY_LOCAL_MACHINE
       valueName - name of the registry value

   remarks:
       This function will always return false on non-Win32-OSes.
*/
bool getRegistryStringValueHKLM(std::string& theString, const std::string& subKey, const std::string& valueName);

} //namespace

#endif // MWTP_BASE_REGISTRYFUNCTIONS_H
