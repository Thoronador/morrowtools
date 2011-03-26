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

#ifndef CONTAINERRECORD_H
#define CONTAINERRECORD_H

#include <string>
#include <fstream>
#include <vector>
#include "BasicRecord.h"

struct ItemRecord
{
  int32_t Count;
  std::string Item;
};//struct

struct ContainerRecord: public BasicRecord
{
  std::string ContainerID;
  std::string ModelPath;
  std::string ContainerName;
  float Weight;
  int32_t ContainerFlags;
  std::string ScriptName;
  //items
  std::vector<ItemRecord> Items;

  /* constructor */
  ContainerRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const ContainerRecord& other) const;

  /* writes the record to the given output stream and returns true on success

    parameters:
        output      - the output file stream
  */
  bool saveToStream(std::ofstream& output) const;

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input file stream
  */
  bool loadFromStream(std::ifstream& in_File);

  /* returns true, if the container is an organic container */
  bool isOrganic() const;

  /* returns true, if the container content will respawn (orangic containers only) */
  bool doesRespawn() const;
};//struct

#endif // CONTAINERRECORD_H
