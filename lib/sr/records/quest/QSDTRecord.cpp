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

#include "QSDTRecord.hpp"
#ifndef SR_UNSAVEABLE_RECORDS
#include <iostream>
#endif // SR_UNSAVEABLE_RECORDS

namespace SRTP
{

QSDTRecord::QSDTRecord()
: isFinisher(false),
  nextQuestFormID(0),
  unknownSCHR(BinarySubRecord()),
  unknownSCTX(""),
  unknownQNAM(std::nullopt),
  unknownCTDA_CIS2s(std::vector<CTDA_CIS2_compound>()),
  logEntry(LocalizedString())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool QSDTRecord::operator==(const QSDTRecord& other) const
{
  return (isFinisher == other.isFinisher)
      && (nextQuestFormID == other.nextQuestFormID)
      && (unknownSCHR == other.unknownSCHR)
      && (unknownSCTX == other.unknownSCTX)
      && (unknownQNAM == other.unknownQNAM)
      && (unknownCTDA_CIS2s == other.unknownCTDA_CIS2s)
      && (logEntry == other.logEntry);
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
  if (unknownQNAM.has_value())
  {
    writeSize = writeSize + 4 /* QNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  for (const auto& compound: unknownCTDA_CIS2s)
  {
    writeSize += compound.getWriteSize();
  }
  return writeSize;
}

bool QSDTRecord::saveToStream(std::ostream& output) const
{
  // write QSDT
  output.write(reinterpret_cast<const char*>(&cQSDT), 4);
  uint16_t subLength = 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&isFinisher), 1);

  if (nextQuestFormID != 0)
  {
    // write NAM0
    output.write(reinterpret_cast<const char*>(&cNAM0), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&nextQuestFormID), 4);
  }
  if (unknownSCHR.isPresent())
  {
    if (!unknownSCHR.saveToStream(output, cSCHR))
    {
      std::cerr << "Error while writing subrecord SCHR of QUST!\n";
      return false;
    }
  }
  if (!unknownSCTX.empty())
  {
    // write script text (SCTX)
    output.write(reinterpret_cast<const char*>(&cSCTX), 4);
    subLength = unknownSCTX.length(); // not NUL-terminated
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(unknownSCTX.c_str(), subLength);
  }
  if (unknownQNAM.has_value())
  {
    // write QNAM
    output.write(reinterpret_cast<const char*>(&cQNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownQNAM.value()), 4);
  }
  for (const auto& ctda_cis2: unknownCTDA_CIS2s)
  {
    if (!ctda_cis2.saveToStream(output))
    {
      std::cerr << "Error while writing subrecord CTDA or CIS2 of QUST!\n";
      return false;
    }
  }
  // write CNAM
  return logEntry.saveToStream(output, cCNAM);
}
#endif // SR_UNSAVEABLE_RECORDS

} // namespace
