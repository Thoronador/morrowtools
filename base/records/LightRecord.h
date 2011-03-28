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

#ifndef LIGHTRECORD_H
#define LIGHTRECORD_H

#include <string>
#include <fstream>
#include "BasicRecord.h"

struct LightRecord: public BasicRecord
{
  std::string LightID;
  std::string ModelPath;
  std::string Name;
  std::string InventoryIcon;
  //light data
  float Weight;
  int32_t Value;
  int32_t Time;
  int32_t Radius;
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;
  uint8_t Zero;
  int32_t LightFlags;
  //end of light data
  std::string ScriptID;
  std::string SoundID;

  /* constructor */
  LightRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const LightRecord& other) const;

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

#endif // LIGHTRECORD_H
