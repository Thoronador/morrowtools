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

#include "QOBJEntry.hpp"

namespace SRTP
{

QOBJEntry::QOBJEntry()
: unknownQOBJ(0),
  unknownFNAM(0),
  displayText(LocalizedString()),
  theQSTAs(std::vector<QSTAEntry>())
{
}

void QOBJEntry::clear()
{
  unknownQOBJ = 0;
  unknownFNAM = 0;
  displayText.reset();
  theQSTAs.clear();
}

#ifndef SR_NO_RECORD_EQUALITY
bool QOBJEntry::operator==(const QOBJEntry& other) const
{
  return (unknownQOBJ == other.unknownQOBJ) && (unknownFNAM == other.unknownFNAM)
      && (displayText == other.displayText) && (theQSTAs == other.theQSTAs);
}
#endif // SR_NO_RECORD_EQUALITY

#ifndef SR_UNSAVEABLE_RECORDS
bool QOBJEntry::saveToStream(std::ostream& output) const
{
  // write QOBJ
  output.write(reinterpret_cast<const char*>(&cQOBJ), 4);
  uint16_t subLength = 2;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownQOBJ), 2);

  // write FNAM
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownFNAM), 4);

  // write localized text
  if (!displayText.saveToStream(output, cNNAM))
    return false;

  for (const auto& qsta: theQSTAs)
  {
    if (!qsta.saveToStream(output))
      return false;
  }

  return output.good();
}

uint32_t QOBJEntry::getWriteSize() const
{
  uint32_t writeSize = 4 /* QOBJ */ + 2 /* 2 bytes for length */ + 2 /* size */
                     + 4 /* FNAM */ + 2 /* 2 bytes for length */ + 4 /* size */
                     + displayText.getWriteSize();
  for (const auto& qsta: theQSTAs)
  {
    writeSize += qsta.getWriteSize();
  }
  return writeSize;
}
#endif

} // namespace
