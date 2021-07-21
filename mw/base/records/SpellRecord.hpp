/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012, 2013  Thoronador

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

#ifndef MW_SPELLRECORD_HPP
#define MW_SPELLRECORD_HPP

#include <fstream>
#include <string>
#include <vector>
#include "../Enchantment.hpp"
#include "BasicRecord.hpp"

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
  /* constructor */
  SpellRecord();

  std::string recordID; //formerly SpellID
  std::string Name;
  //spell data
  int32_t Type;
  int32_t Cost;
  int32_t Flags;
  //end of spell data
  std::vector<EnchantmentData> Enchs;

  #ifndef MW_UNSAVEABLE_RECORDS
  /* tries to save the spell record to an output file stream and returns true
     on success, false on failure.

     parameters:
         output  - the output file stream that is used to save the spell data
  */
  bool saveToStream(std::ofstream& output) const;
  #endif

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

  /* constant that holds the maximum length of spell name, that Morrowind can
     still handle properly (excluding terminating NUL character)
  */
  static const size_t cMaximumSpellNameLength;
};//struct

} //namespace

#endif // MW_SPELLRECORD_HPP
