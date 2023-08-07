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

#ifndef SR_QUEST_INDEXENTRY_HPP
#define SR_QUEST_INDEXENTRY_HPP

#include <cstdint>
#include <vector>
#include "QSDTRecord.hpp"

namespace SRTP
{

/* type for holding quest stages / indices*/
struct IndexEntry
{
  uint16_t index; //stage index
  uint16_t indexUnknownPart; //flags or something?
  std::vector<QSDTRecord> theQSDTs;

  /* constructor */
  IndexEntry();

  #ifndef SR_NO_RECORD_EQUALITY
  /* equality operator */
  bool operator==(const IndexEntry& other) const;
  #endif // SR_NO_RECORD_EQUALITY

  /* returns true, if the finishing flag is set for one of the index' QSDTs */
  bool hasFinishingQSDT() const;
}; // struct

} // namespace

#endif // SR_QUEST_INDEXENTRY_HPP
