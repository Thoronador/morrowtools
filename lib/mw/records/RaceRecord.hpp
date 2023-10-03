/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2023  Dirk Stolle

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

#ifndef MW_RACERECORD_HPP
#define MW_RACERECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"

namespace MWTP
{

struct SkillBonus
{
  int32_t SkillID;
  uint32_t Bonus; /**< amount of additional skill points */

  bool operator==(const SkillBonus& other) const;
};

struct RaceRecord: public BasicRecord
{
  std::string recordID;
  std::string Name; /**< localized name of the race */
  // race data
  std::vector<SkillBonus> Boni;
  int32_t Strength[2];
  int32_t Intelligence[2];
  int32_t Willpower[2];
  int32_t Agility[2];
  int32_t Speed[2];
  int32_t Endurance[2];
  int32_t Personality[2];
  int32_t Luck[2];
  float Height[2];
  float Weight[2];
  uint32_t Flags;
  // end of race data
  std::vector<std::string> Powers;
  std::string Description;

  RaceRecord();

  /** \brief Creates a record with a preset ID.
   *
   * \param ID  the ID of the record
   */
  RaceRecord(const std::string& ID);


  /** \brief Checks whether another instance contains the same data.
   *
   * \param other   the other record to compare with
   * \return Returns true, if @other contains the same data as this instance.
   *         Returns false otherwise.
   */
  bool equals(const RaceRecord& other) const;

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

  /** \brief Checks whether this is a playable race.
   *
   * \return Returns true, if the "playable" flag is set.
   */
  bool isPlayable() const;

  /** \brief Checks whether this is a beast race.
   *
   * \return Returns true, if the "beast race" flag is set.
   */
  bool isBeastRace() const;
}; // struct

// comparison operator for use in ordered set
bool operator<(const RaceRecord& left, const RaceRecord& right);

} // namespace

#endif // MW_RACERECORD_HPP
