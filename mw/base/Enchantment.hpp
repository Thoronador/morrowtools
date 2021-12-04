/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012, 2021  Thoronador

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

#ifndef MW_ENCHANTMENT_HPP
#define MW_ENCHANTMENT_HPP

#include <cstdint>
#include <fstream>

namespace MWTP
{

/// Enumeration type for range of spells.
enum class RangeType: uint32_t
{
  Self = 0,
  Touch = 1,
  Target = 2
};

/** Holds data of a single enchantment or spell effect. */
struct EnchantmentData
{
  int16_t EffectID;
  uint8_t SkillID;
  uint8_t AttributeID;
  RangeType Range;
  int32_t Area;
  int32_t Duration;
  int32_t MagnitudeMin;
  int32_t MagnitudeMax;

  EnchantmentData();

  bool operator==(const EnchantmentData& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Writes the enchantment data to the given output stream.
   *
   * \param output  the output stream
   * \return Returns true on success (data was written to stream).
   *         Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const;
  #endif

  /** \brief Loads the enchantment data from the given input stream.
   *
   * \param input    the input stream
   * \return Returns true on success (data was loaded from stream).
   *         Returns false, if an error occurred.
   */
  bool loadFromStream(std::istream& input);
}; // struct

} // namespace

#endif // MW_ENCHANTMENT_HPP
