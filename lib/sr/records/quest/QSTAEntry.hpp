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

#ifndef SR_QUEST_QSTAENTRY_HPP
#define SR_QUEST_QSTAENTRY_HPP

#ifndef SR_UNSAVEABLE_RECORDS
#include <fstream>
#endif // SR_UNSAVEABLE_RECORDS
#include <vector>
#include "../CTDAData.hpp"

namespace SRTP
{

//struct for QSTA stuff
struct QSTAEntry
{
  uint64_t unknownQSTA;
  std::vector<CTDA_CIS2_compound> unknownCTDA_CIS2s;

  /* constructor */
  QSTAEntry();

  /* clears all data members */
  void clear();

  #ifndef SR_NO_RECORD_EQUALITY
  /* equality operator */
  bool operator==(const QSTAEntry& other) const;
  #endif // SR_NO_RECORD_EQUALITY

  #ifndef SR_UNSAVEABLE_RECORDS
  /** Tries to save the QSTAEntry to the given stream.
   *
   * \param output    the output stream
   * \return Returns true in case of success, false on failure.
   */
  bool saveToStream(std::ostream& output) const;

  /** Gets the size in bytes that the QSTAEntry's data would occupy.
   *
   * \return Returns the size in bytes that the QSTAEntry would occupy in a
   *         file stream.
   */
  uint32_t getWriteSize() const;
  #endif
}; // struct

} // namespace

#endif // SR_QUEST_QSTAENTRY_HPP
