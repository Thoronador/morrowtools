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

#ifndef COMPILERCODES_H
#define COMPILERCODES_H

namespace ScriptCompiler
{
  /* op-codes for scripts' compiled data */
  const uint16_t CodeEnd   = 0x0101; //"0101"
  //const uint16_t CodeEndif = 0x0109; //"0901"
  const uint16_t CodeJournal = 0x10CC; //"CC10"
  const uint16_t CodeReturn = 0x0124; //"2401"
  const uint16_t CodeStopScript = 0x101C; //"1C10"
} //namespace

#endif // COMPILERCODES_H
