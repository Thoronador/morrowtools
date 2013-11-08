/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012  Thoronador

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

#ifndef MW_RETURNCODES_H
#define MW_RETURNCODES_H

namespace MWTP
{

//return codes
const int rcInvalidParameter = 1;
const int rcFileError = 2;
const int rcDataError = 3;
const int rcOutputFailed = 4;
const int rcNoSpells = 5; /* for spell renamer */
const int rcXMLError = 6; /* for cell translator */
const int rcXMLEmpty = 7; /* for cell translator */
const int rcScriptError = 8; /* for cell translator */
const int rcNoChange = 9; /* for cell translator */
const int rcFileNameConflict = 10; /* for cell translator */
const int rcCouldNotDo = 11; /* for cell translator */

} //namespace

#endif // MW_RETURNCODES_H
