/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef SR_FORMIDFUNCTIONS_H
#define SR_FORMIDFUNCTIONS_H

#include <stdint.h>
#include <string>

namespace SRTP
{

// extracts the mod index from a form ID
uint8_t extractModIndex(const uint32_t fID);

//extracts the object indef from a form ID
uint32_t extractObjectIndex(const uint32_t fID);

// returns a string representation of the form ID
std::string getFormIDAsString(uint32_t fID);

}//namespace

#endif // SR_FORMIDFUNCTIONS_H
