/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012, 2013, 2021  Dirk Stolle

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

#include <string>
#include <vector>
#include "../Enchantment.hpp"
#include "BasicRecord.hpp"

namespace MWTP
{

/** Enumeration for possible types of spells. */
enum class SpellType: uint32_t
{
  Spell = 0,
  Ability = 1,
  Blight = 2,
  Disease = 3,
  Curse = 4,
  Power = 5
};

const int32_t sfAutoCalcCosts = 1;
const int32_t sfPCStartSpell = 2;
const int32_t sfAlwaysSucceeds = 4;

struct SpellRecord: public BasicRecord
{
  SpellRecord();

  std::string recordID;
  std::string Name;
  // spell data
  SpellType Type;
  int32_t Cost;
  int32_t Flags;
  // end of spell data
  std::vector<EnchantmentData> Enchs;

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Writes the record to the given output stream.
   *
   * \param output  the output stream
   * \return Returns true on success (record was written to stream).
   *         Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const override;
  #endif

  /** \brief Loads the record from the given input stream.
   *
   * \param input    the input stream
   * \return Returns true on success (record was loaded from stream).
   *         Returns false, if an error occurred.
   */
  bool loadFromStream(std::istream& input) override;

  /** \brief Checks whether another instance contains the same data.
   *
   * \param other   the other record to compare with
   * \return Returns true, if @other contains the same data as this instance.
   *         Returns false otherwise.
   */
  bool equals(const SpellRecord& other) const;

  /** \brief Checks whether the "Auto Calculate Costs" flag is set.
   *
   * \return Returns true, if the "Auto Calculate Costs" flag is set.
   */
  bool autoCalculateCosts() const;

  /** \brief Checks whether the spell is a start spell.
   *
   * \return Returns true, if the spell is a start spell.
   */
  bool isPCStartSpell() const;

  /** \brief Checks whether the "always succeed" flag is set.
   *
   * \return Returns true, if the "always succeed" flag is set.
   */
  bool alwaysSucceeds() const;

  /** Holds the maximum length of spell name that Morrowind can
      still handle properly (excluding terminating NUL character).
  */
  static const size_t cMaximumSpellNameLength;
}; // struct

} // namespace

#endif // MW_SPELLRECORD_HPP
