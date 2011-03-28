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

#ifndef REPAIRITEMRECORD_H
#define REPAIRITEMRECORD_H

#include <string>
#include "BasicRecord.h"

struct RepairItemRecord: public BasicRecord
{
  std::string RepairItemID;
  std::string ModelPath;
  std::string Name;
  //repair data
  float Weight;
  int32_t Value;
  int32_t Uses;
  float Quality;
  //end of repair data
  std::string InventoryIcon;
  std::string ScriptName;

  /* constructor */
  RepairItemRecord();

  /* alternative constructor */
  RepairItemRecord(const std::string& ID);

  /* destructor */
  ~RepairItemRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const RepairItemRecord& other) const;

  /* writes the record to the given output stream and returns true on success

    parameters:
        output - the output file stream
  */
  bool saveToStream(std::ofstream& output) const;

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input file stream
  */
  bool loadFromStream(std::ifstream& in_File);
};//struct

//comparison operator for ordered set
bool operator<(const RepairItemRecord& left, const RepairItemRecord& right);

#endif // REPAIRITEMRECORD_H
