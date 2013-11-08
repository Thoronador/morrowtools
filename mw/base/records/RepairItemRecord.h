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

#ifndef MW_REPAIRITEMRECORD_H
#define MW_REPAIRITEMRECORD_H

#include <string>
#include "BasicRecord.h"

namespace MWTP
{

struct RepairItemRecord: public BasicRecord
{
  std::string recordID; //formerly known as RepairItemID
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

  #ifndef MW_UNSAVEABLE_RECORDS
  /* writes the record to the given output stream and returns true on success

    parameters:
        output - the output file stream
  */
  bool saveToStream(std::ofstream& output) const;
  #endif

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input file stream
  */
  bool loadFromStream(std::ifstream& in_File);
};//struct

//comparison operator for ordered set
bool operator<(const RepairItemRecord& left, const RepairItemRecord& right);

} //namespace

#endif // MW_REPAIRITEMRECORD_H
