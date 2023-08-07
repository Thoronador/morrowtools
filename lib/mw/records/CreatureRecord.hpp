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

#ifndef MW_CREATURERECORD_HPP
#define MW_CREATURERECORD_HPP

#include <string>
#include <vector>
#include "PreNPCRecord.hpp"

namespace MWTP
{

struct CreatureRecord: public PreNPCRecord
{
  std::string recordID;
  std::string ModelPath;
  std::string Name;
  std::string SoundGenCreature;
  // creature data
  int32_t CreatureType;
  int32_t Level;
  int32_t Strength;
  int32_t Intelligence;
  int32_t Willpower;
  int32_t Agility;
  int32_t Speed;
  int32_t Endurance;
  int32_t Personality;
  int32_t Luck;
  int32_t Health;
  int32_t SpellPoints;
  int32_t Fatigue;
  int32_t Soul;
  int32_t Combat;
  int32_t Magic;
  int32_t Stealth;
  int32_t AttackMin1;
  int32_t AttackMax1;
  int32_t AttackMin2;
  int32_t AttackMax2;
  int32_t AttackMin3;
  int32_t AttackMax3;
  int32_t Gold;
  // end of creature data
  uint32_t CreatureFlag;
  // Items and spells are already declared in PreNPCRecord.
  // The AI-related data members are already declared in PreNPCRecord.

  std::string ScriptID;
  float Scale;

  CreatureRecord();
  ~CreatureRecord();
  CreatureRecord& operator=(const CreatureRecord& source);

  /** \brief Checks whether another instance contains the same data.
   *
   * \param other   the other record to compare with
   * \return Returns true, if @other contains the same data as this instance.
   *         Returns false otherwise.
   */
  bool equals(const CreatureRecord& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Gets the size in bytes that the record's data would occupy in a
   *         stream, NOT including the header data.
   *
   * \return Returns the size in bytes that the record would need. Size of the
   *         header is not included.
   */
  uint32_t getWriteSize() const;

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

} // namespace

#endif // MW_CREATURERECORD_HPP
