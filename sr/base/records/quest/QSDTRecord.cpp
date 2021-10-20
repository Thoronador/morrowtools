/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2014  Thoronador

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

#include "QSDTRecord.hpp"

namespace SRTP
{

QSDTRecord::QSDTRecord()
: isFinisher(false),
  nextQuestFormID(0),
  unknownSCHR(BinarySubRecord()),
  unknownSCTX(""),
  hasQNAM(false), unknownQNAM(0),
  unknownCTDA_CIS2s(std::vector<CTDA_CIS2_compound>()),
  logEntry(LocalizedString())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool QSDTRecord::operator==(const QSDTRecord& other) const
{
  return (isFinisher == other.isFinisher) && (unknownCTDA_CIS2s == other.unknownCTDA_CIS2s)
      && (nextQuestFormID == other.nextQuestFormID)
      && (hasQNAM == other.hasQNAM) && ((unknownQNAM == other.unknownQNAM) || !hasQNAM)
      && (logEntry == other.logEntry)
      && (unknownSCHR == other.unknownSCHR)
      && (unknownSCTX == other.unknownSCTX);
}
#endif // SR_NO_RECORD_EQUALITY

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t QSDTRecord::getWriteSize() const
{
  uint32_t writeSize = 4 /* QSDT */ + 2 /* 2 bytes for length */ + 1 /* fixed size */
                     + logEntry.getWriteSize() /* CNAM */;
  if (nextQuestFormID != 0)
  {
    writeSize = writeSize + 4 /* NAM0 */ +2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (unknownSCHR.isPresent())
  {
    writeSize = writeSize + 4 /* SCHR */ + 2 /* 2 bytes for length */ + unknownSCHR.size() /* size */;
  }
  if (!unknownSCTX.empty())
  {
    writeSize = writeSize + 4 /* SCTX */ + 2 /* 2 bytes for length */ + unknownSCTX.length() /* length */;
  }
  if (hasQNAM)
  {
    writeSize = writeSize + 4 /* QNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  for (const auto& compound: unknownCTDA_CIS2s)
  {
    writeSize += compound.getWriteSize();
  }
  return writeSize;
}
#endif // SR_UNSAVEABLE_RECORDS

} // namespace
