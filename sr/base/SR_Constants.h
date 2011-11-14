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

#ifndef SR_CONSTANTS_H
#define SR_CONSTANTS_H

#include <stdint.h>

namespace SRTP
{

const int32_t cCNAM = 0x4D414E43; //"CNAM" = 43 4E 41 4D
const int32_t cGRUP = 0x50555247; //"GRUP" = 47 52 55 50
const int32_t cHEDR = 0x52444548; //"HEDR" = 48 45 44 52
const int32_t cINTV = 0x56544E49; //"INTV" = 49 4E 54 56
const int32_t cTES4 = 0x34534554; //"TES4" = 54 45 53 34

} //namespace

#endif // SR_CONSTANTS_H
