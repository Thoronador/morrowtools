/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

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

#ifndef SPELLRECORD_H
#define SPELLRECORD_H

#include <fstream>
#include <string>
#include <vector>
#include "../Enchantment.h"
#include "BasicRecord.h"

namespace MWTP
{

const int32_t stSpell = 0;
const int32_t stAbility = 1;
const int32_t stBlight = 2;
const int32_t stDisease = 3;
const int32_t stCurse = 4;
const int32_t stPower = 5;

const int32_t sfAutoCalcCosts = 1;
const int32_t sfPCStartSpell = 2;
const int32_t sfAlwaysSucceeds = 4;

struct SpellRecord: public BasicRecord
{
  std::string SpellID;
  std::string Name;
  //spell data
  int32_t Type;
  int32_t Cost;
  int32_t Flags;
  //end of spell data
  std::vector<EnchantmentData> Enchs;

  /* tries to save the spell record to an output file stream and returns true
     on success, false on failure.

     parameters:
         output  - the output file stream that is used to save the spell data
  */
  bool saveToStream(std::ofstream& output) const;

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input file stream
  */
  bool loadFromStream(std::ifstream& in_File);

  /* returns true, if the other spell record contains the same data */
  bool equals(const SpellRecord& other) const;

  /* returns true, if the Auto Calculate Costs flag is set */
  bool autoCalculateCosts() const;

  /* returns true, if the spell is a start spell */
  bool isPCStartSpell() const;

  /* returns true, if the "always succeed" flag is set */
  bool alwaysSucceeds() const;
};//struct

} //namespace

#endif // SPELLRECORD_H