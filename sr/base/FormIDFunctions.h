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

#ifndef SR_FORMIDFUNCTIONS_H
#define SR_FORMIDFUNCTIONS_H

#include <stdint.h>
#include <string>

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

}//namespace

#endif // SR_FORMIDFUNCTIONS_H
