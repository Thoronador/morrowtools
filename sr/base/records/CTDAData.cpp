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

#include "CTDAData.hpp"
#include <iostream>
#include <cstring>

namespace SRTP
{

CTDAData::CTDAData()
: content(std::array<uint8_t, 32>({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}))
{
}

bool CTDAData::loadFromStream(std::istream& in_Stream, uint32_t& bytesRead)
{
  // CTDA's length
  uint16_t subLength = 0;
  in_Stream.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength != 32)
  {
    std::cerr << "Error: Sub record CTDA has invalid length (" << subLength
              << " bytes). Should be 32 bytes.\n";
    return false;
  }
  // read CTDA data
  in_Stream.read(reinterpret_cast<char*>(content.data()), 32);
  bytesRead += 32;
  if (!in_Stream.good())
  {
    std::cerr << "Error while reading subrecord CTDA!\n";
    return false;
  }
  return true;
}

#ifndef SR_UNSAVEABLE_RECORDS
bool CTDAData::saveToStream(std::ostream& output) const
{
  // write CTDA
  output.write(reinterpret_cast<const char*>(&cCTDA), 4);
  // CTDA's length
  const uint16_t subLength = 32; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write content
  output.write(reinterpret_cast<const char*>(content.data()), 32);
  return output.good();
}
#endif

#ifndef SR_NO_RECORD_EQUALITY
bool CTDAData::operator==(const CTDAData& other) const
{
  return content == other.content;
}
#endif // SR_NO_RECORD_EQUALITY

void CTDAData::clear()
{
  content.fill(0);
}

} // namespace
