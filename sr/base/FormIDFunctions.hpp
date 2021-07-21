/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2013  Thoronador

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

#ifndef SR_FORMIDFUNCTIONS_HPP
#define SR_FORMIDFUNCTIONS_HPP

#include <stdint.h>
#include <string>
#include <vector>

namespace SRTP
{

/* extracts the mod index from a form ID */
uint8_t extractModIndex(const uint32_t fID);

/* extracts the object index from a form ID */
uint32_t extractObjectIndex(const uint32_t fID);

/* changes the mode index of a form ID to newModIndex */
inline void changeModIndexInSitu(uint32_t& fID, const uint8_t newModIndex)
{
  fID = ((fID & 0x00FFFFFF) | (newModIndex<<24));
}

/* returns a string representation of the form ID */
std::string getFormIDAsString(uint32_t fID);

/* returns a string representation of the form ID. If the mod index of the form
   ID is non-zero, it will show 'xx' instead of the mod index part.
*/
std::string getFormIDAsStringXX(const uint32_t fID);

/* returns a string representation of the form ID. If the mod index of the form
   ID is non-zero, it will show 'xx' instead of the mod index part.
   Additionally, it shows the file that the form ID came from, based on the mod
   index.

   parameters:
       fID       - the form ID
       loadOrder - list of master and plugin files in the order they were loaded
       withFiles - if set to true, the file name is included (default) in the
                   result, otherwise just the form ID is shown.
*/
std::string getFormIDAsStringWithFile(const uint32_t fID, const std::vector<std::string>& loadOrder, const bool withFiles);

}//namespace

#endif // SR_FORMIDFUNCTIONS_HPP
