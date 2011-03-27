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

#ifndef LEVELEDITEMRECORD_H
#define LEVELEDITEMRECORD_H

#include <string>
#include <fstream>
#include <vector>
#include "BasicRecord.h"

struct LevItemListEntry
{
  std::string ItemID;
  int16_t Level;
};//struct

struct LeveledItemRecord: public BasicRecord
{
  std::string LevItemID;
  int32_t ListData;
  uint8_t ChanceNone;
  uint32_t NumberOfItems;
  std::vector<LevItemListEntry> Entries;

  /* constructor */
  LeveledItemRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const LeveledItemRecord& other) const;

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

#endif // LEVELEDITEMRECORD_H
