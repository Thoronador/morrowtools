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

#ifndef WEAPONRECORD_H
#define WEAPONRECORD_H

#include <fstream>
#include <string>
#include "BasicRecord.h"

struct WeaponRecord: public BasicRecord
{
  std::string WeaponID;
  std::string ModelPath;
  std::string Name;
  //weapon data
  float Weight;
  int32_t Value;
  int16_t Type;
  int16_t Health;
  float Speed;
  float Reach;
  int16_t EnchantPts;
  uint8_t ChopMin;
  uint8_t ChopMax;
  uint8_t SlashMin;
  uint8_t SlashMax;
  uint8_t ThrustMin;
  uint8_t ThrustMax;
  int32_t WeaponFlags;
  //end of weapon data
  std::string InventoryIcon;
  std::string EnchantmentID;
  std::string ScriptID;

  /* constructor */
  WeaponRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const WeaponRecord& other) const;

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

#endif // WEAPONRECORD_H