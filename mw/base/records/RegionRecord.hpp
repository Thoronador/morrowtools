/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2022  Dirk Stolle

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

#ifndef MW_REGIONRECORD_HPP
#define MW_REGIONRECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"

namespace MWTP
{

struct SoundChanceRecord
{
  std::string Sound;
  uint8_t Chance;

  SoundChanceRecord();

  bool operator==(const SoundChanceRecord& other) const;
}; // struct

struct RegionRecord: public BasicRecord
{
  std::string recordID;
  std::string RegionName;
  // weather data
  uint8_t Clear;
  uint8_t Cloudy;
  uint8_t Foggy;
  uint8_t Overcast;
  uint8_t Rain;
  uint8_t Thunder;
  uint8_t Ash;
  uint8_t Blight;
  uint8_t Snow; // Bloodmoon only
  uint8_t Blizzard; // Bloodmoon only
  // end of weather data
  std::string SleepCreature;
  // Region Map Colour
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;
  uint8_t Zero;
  std::vector<SoundChanceRecord> SoundChances;

  RegionRecord();

  /** \brief Constructs a RegionRecord with the given region ID.
   *
   * \param ID  the region ID
   */
  RegionRecord(const std::string& ID);

  /** \brief Checks whether another instance contains the same data.
   *
   * \param other   the other record to compare with
   * \return Returns true, if @other contains the same data as this instance.
   *         Returns false otherwise.
   */
  bool equals(const RegionRecord& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Writes the record to the given output stream, enforcing
   *         Bloodmoon-styled weather data.
   *
   * \param output  the output stream
   * \param forceBloodmoonStyle  if set to true, the written WEAT subrecord will
                                 have a size of 10 bytes, even when it wouldn't
                                 be necessary.
   * \return Returns true on success (record was written to stream).
   *         Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output, const bool forceBloodmoonStyle) const;

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
}; // struct

// comparison operator for ordered set
bool operator<(const RegionRecord& left, const RegionRecord& right);

} // namespace

#endif // MW_REGIONRECORD_HPP
