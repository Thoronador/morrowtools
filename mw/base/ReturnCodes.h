/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef RETURNCODES_H
#define RETURNCODES_H

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

} //namespace

#endif // RETURNCODES_H
