/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Thoronador

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

#ifndef SR_QUEST_ALIASENTRY_HPP
#define SR_QUEST_ALIASENTRY_HPP

#include <optional>
#include <string>
#include <vector>
#include "../ComponentData.hpp"
#include "../CTDAData.hpp"

namespace SRTP
{

/// Holds for alias-related data of a quest record.
struct AliasEntry
{
  std::optional<uint32_t> unknownALST;
  std::optional<uint32_t> unknownALLS;
  std::string aliasID; // subrecord ALID
  uint32_t unknownFNAM;
  std::optional<uint32_t> unknownALFA;
  uint32_t locationRefTypeFormID; // subrecord ALRT
  uint32_t createReferenceToObjectFormID; // subrecord ALCO
  std::optional<uint32_t> unknownALCA;
  std::optional<uint32_t> unknownALCL;
  uint32_t displayNameFormID; // subrecord ALDN
  std::vector<ComponentData> components;
  std::vector<uint32_t> keywordArray;
  std::optional<uint32_t> unknownALFE;
  std::optional<uint32_t> unknownALFD;
  std::optional<uint32_t> forcedIntoAliasID; // subrecord ALFI
  uint32_t specificLocationFormID; // subrecord ALFL
  uint32_t specificReferenceID; // subrecord ALFR
  std::optional<uint32_t> unknownALNA;
  std::optional<uint32_t> unknownALNT;
  uint32_t uniqueActorFormID; // subrecord ALUA
  uint32_t externalAliasReferenceFormID; // subrecord ALEQ
  std::optional<uint32_t> unknownALEA;
  uint32_t keywordFormID; // subrecord KNAM
  std::vector<CTDA_CIS2_compound> unknownCTDA_CIS2s;
  uint32_t spectatorOverridePackageListFormID; // subrecord SPOR
  uint32_t observeDeadBodyOverridePackageListFormID; // subrecord OCOR
  uint32_t guardWarnOverridePackageListFormID; // subrecord GWOR
  uint32_t combatOverridePackageListFormID; // subrecord ECOR
  std::vector<uint32_t> spellFormIDs; // subrecords ALSP
  std::vector<uint32_t> factionFormIDs; // subrecords ALFC
  std::vector<uint32_t> packageDataFormIDs; // subrecords ALPC
  std::optional<uint32_t> unknownVTCK;

  /* constructor */
  AliasEntry();

  /* clears all data members */
  void clear();

  #ifndef SR_NO_RECORD_EQUALITY
  /* equality operator */
  bool operator==(const AliasEntry& other) const;
  #endif // SR_NO_RECORD_EQUALITY

  #ifndef SR_UNSAVEABLE_RECORDS
  /** Tries to save the AliasEntry to the given stream.
   *
   * \param output    the output stream
   * \return Returns true in case of success, false on failure.
   */
  bool saveToStream(std::ostream& output) const;

  /** Gets the size in bytes that the AliasEntry's data would occupy.
   *
   * \return Returns the size in bytes that the AliasEntry would occupy in a
   *         file stream.
   */
  uint32_t getWriteSize() const;
  #endif
}; // struct

} // namespace

#endif // SR_QUEST_ALIASENTRY_HPP
