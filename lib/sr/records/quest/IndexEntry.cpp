/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2014, 2021  Thoronador

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

#include "IndexEntry.hpp"

namespace SRTP
{

IndexEntry::IndexEntry()
: index(0),
  indexUnknownPart(0),
  theQSDTs(std::vector<QSDTRecord>())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool IndexEntry::operator==(const IndexEntry& other) const
{
  return (index == other.index) && (indexUnknownPart == other.indexUnknownPart)
      && (theQSDTs == other.theQSDTs);
}
#endif // SR_NO_RECORD_EQUALITY

bool IndexEntry::hasFinishingQSDT() const
{
  for (const auto& qsdt: theQSDTs)
  {
    if (qsdt.isFinisher)
      return true;
  }
  return false;
}

} // namespace
