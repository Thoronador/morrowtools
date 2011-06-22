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

#ifndef LOCKPICKRECORD_H
#define LOCKPICKRECORD_H

#include <string>
#include <fstream>
#include "BasicRecord.h"

namespace MWTP
{

struct LockpickRecord: public BasicRecord
{
  std::string LockpickID;
  std::string Name;
  std::string ModelPath;
  //lockpick data
  float Weight;
  int32_t Value;
  float Quality;
  int32_t Uses;
  //end of lockpick data
  std::string InventoryIcon;
  std::string ScriptID;

  /* constructor */
  LockpickRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const LockpickRecord& other) const;

  /* writes the record to the given output stream and returns true on success

    parameters:
        output   - the output file stream
  */
  bool saveToStream(std::ofstream& output) const;

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input file stream
  */
  bool loadFromStream(std::ifstream& in_File);
};//struct

} //namespace

#endif // LOCKPICKRECORD_H
