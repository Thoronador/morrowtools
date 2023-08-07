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

#ifndef SR_QUEST_QSDTRECORD_HPP
#define SR_QUEST_QSDTRECORD_HPP

#ifndef SR_UNSAVEABLE_RECORDS
#include <fstream>
#endif
#include <optional>
#include <string>
#include <vector>
#include "../BinarySubRecord.hpp"
#include "../CTDAData.hpp"
#include "../LocalizedString.hpp"

namespace SRTP
{

struct QSDTRecord
{
  bool isFinisher; // true, if entry finishes the quest
  uint32_t nextQuestFormID; // subrecord NAM0
  BinarySubRecord unknownSCHR;
  std::string unknownSCTX;
  std::optional<uint32_t> unknownQNAM;
  std::vector<CTDA_CIS2_compound> unknownCTDA_CIS2s;
  LocalizedString logEntry; // CNAM - string ID of log entry

  /* constructor */
  QSDTRecord();

  #ifndef SR_NO_RECORD_EQUALITY
  /* equality operator */
  bool operator==(const QSDTRecord& other) const;
  #endif // SR_NO_RECORD_EQUALITY

  #ifndef SR_UNSAVEABLE_RECORDS
  /** Gets the size in bytes that the instances's data would occupy.
   *
   * \return Returns the size in bytes that the instances's data would occupy.
   */
  uint32_t getWriteSize() const;

  /** \brief Writes the record to the given output stream.
   *
   * \param output  the output stream
   * \return Returns true on success (record was written to stream).
   *         Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const;
  #endif
}; // struct

} // namespace

#endif // SR_QUEST_QSDTRECORD_HPP
