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

#ifndef MW_NPCRECORD_HPP
#define MW_NPCRECORD_HPP

#include <array>
#include <string>
#include "PreNPCRecord.hpp"

namespace MWTP
{

/* enumeration type for the NPDT subrecord: there are two versions. One is
    12 bytes and the other one is 52 bytes. This enumeration type will be used
    to indicate which type of NPC data was read from the stream.
*/
enum NPDT_Type {ndtNone, ndt12Bytes, ndt52Bytes};

/* the NPC record - holds all data about an NPC */
struct NPCRecord: public PreNPCRecord
{
  std::string recordID;
  std::string Name;
  std::string ModelPath;
  std::string RaceID;
  std::string FactionID;
  std::string HeadModel;
  std::string ClassID;
  std::string HairModel;
  std::string ScriptID;
  // NPC data
  int16_t Level;
  uint8_t Strength;
  uint8_t Intelligence;
  uint8_t Willpower;
  uint8_t Agility;
  uint8_t Speed;
  uint8_t Endurance;
  uint8_t Personality;
  uint8_t Luck;
  std::array<uint8_t, 27> Skills;
  uint8_t Reputation;
  int16_t Health;
  int16_t SpellPoints;
  int16_t Fatigue;
  uint8_t Disposition;
  uint8_t Data_FactionID;
  int8_t Rank;
  uint8_t Unknown1;
  uint8_t Unknown2;
  uint8_t Unknown3;
  int32_t Gold;
  NPDT_Type NPCDataType;
  // end of NPC data
  uint32_t NPC_Flag;
  //Items and spells are now part of PreNPCRecord structure, search there.
  //AI data
  //This stuff is now part of PreNPCRecord structure, search there.

  NPCRecord();

  virtual ~NPCRecord();

  NPCRecord& operator=(const NPCRecord& source);

  /** \brief Checks whether another instance contains the same data.
   *
   * \param other   the other record to compare with
   * \return Returns true, if @other contains the same data as this instance.
   *         Returns false otherwise.
   */
  bool equals(const NPCRecord& other) const;

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

  /** \brief Checks whether the female flag is set.
   *
   * \return Returns true, if the female flag is set.
   */
  bool isFemale() const;

  /** \brief Checks whether the essential flag is set.
   *
   * \return Returns true, if the essential flag is set.
   */
  bool isEssential() const;

  /** \brief Checks whether the respawn flag is set.
   *
   * \return Returns true, if the respawn flag is set.
   */
  bool doesRespawn() const;

  /** \brief Checks whether the Auto Calculate Stats flag is set.
   *
   * \return Returns true, if the Auto Calculate Stats flag is set.
   */
  bool hasAutoCalcStats() const;

  /** \brief Checks whether the white blood texture (skeleton) flag is set.
   *
   * \return Returns true, if the white blood texture (skeleton) flag is set.
   */
  bool hasWhiteBloodTex() const;

  /** \brief Checks whether the gold blood texture (metal) flag is set.
   *
   * \return Returns true, if the gold blood texture (metal) flag is set.
   */
  bool hasGoldBloodTex() const;
}; // struct

} // namespace

#endif // MW_NPCRECORD_HPP
