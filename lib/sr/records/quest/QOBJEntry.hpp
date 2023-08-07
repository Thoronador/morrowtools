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

#ifndef SR_QUEST_QOBJENTRY_HPP
#define SR_QUEST_QOBJENTRY_HPP

#include <cstdint>
#include <vector>
#include "QSTAEntry.hpp"
#include "../LocalizedString.hpp"

namespace SRTP
{

/// Holds data of QOBJ in quest records.
struct QOBJEntry
{
  uint16_t unknownQOBJ;
  uint32_t unknownFNAM;
  LocalizedString displayText; // subrecord NNAM
  std::vector<QSTAEntry> theQSTAs;

  QOBJEntry();

  /** Clears all data members. */
  void clear();

  #ifndef SR_NO_RECORD_EQUALITY
  /** \brief Checks whether another instance contains the same data.
   *
   * \param other   the other entry to compare with
   * \return Returns true, if @other contains the same data as instance.
   *         Returns false otherwise.
   */
  bool operator==(const QOBJEntry& other) const;
  #endif // SR_NO_RECORD_EQUALITY

  #ifndef SR_UNSAVEABLE_RECORDS
  /** Tries to save the QOBJEntry to the given stream.
   *
   * \param output    the output stream
   * \return Returns true in case of success, false on failure.
   */
  bool saveToStream(std::ostream& output) const;

  /** Gets the size in bytes that the QOBJEntry's data would occupy.
   *
   * \return Returns the size in bytes that the QOBJEntry would occupy in a
   *         file stream.
   */
  uint32_t getWriteSize() const;
  #endif
}; // struct

} // namespace

#endif // SR_QUEST_QOBJENTRY_HPP
