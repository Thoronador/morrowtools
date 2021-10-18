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

#include "QSTAEntry.hpp"

namespace SRTP
{

QSTAEntry::QSTAEntry()
: unknownQSTA(0),
  unknownCTDA_CIS2s(std::vector<CTDA_CIS2_compound>())
{
}

void QSTAEntry::clear()
{
  unknownQSTA = 0;
  unknownCTDA_CIS2s.clear();
}

#ifndef SR_NO_RECORD_EQUALITY
bool QSTAEntry::operator==(const QSTAEntry& other) const
{
  return (unknownQSTA == other.unknownQSTA) && (unknownCTDA_CIS2s == other.unknownCTDA_CIS2s);
}
#endif // SR_NO_RECORD_EQUALITY

} // namespace
