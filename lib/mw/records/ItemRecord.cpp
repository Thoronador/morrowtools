/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013, 2021, 2022  Dirk Stolle

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

#include "ItemRecord.hpp"
#include <cstring>
#include <iostream>
#ifndef MW_UNSAVEABLE_RECORDS
#include "../MW_Constants.hpp"
#endif // MW_UNSAVEABLE_RECORDS

namespace MWTP
{

ItemRecord::ItemRecord()
: Count(0),
  Item(std::string())
{ }

bool ItemRecord::operator==(const ItemRecord& other) const
{
  return (Count == other.Count) && (Item == other.Item);
}

bool ItemRecord::loadFromStream(std::istream& input, char* buffer, uint32_t& bytesRead)
{
  // NPCO's length
  uint32_t subLength = 0;
  input.read(reinterpret_cast<char*>(&subLength), 4);
  bytesRead += 4;
  if (subLength != 36)
  {
    std::cerr << "Error: Sub record NPCO has invalid length ("
              << subLength << " bytes). Should be 36 bytes.\n";
    return false;
  }
  // read count and name
  // ---- count
  input.read(reinterpret_cast<char*>(&Count), 4);
  memset(buffer, '\0', 33);
  input.read(buffer, 32);
  bytesRead += 36;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record NPCO!\n";
    return false;
  }
  Item = std::string(buffer);
  return true;
}

#ifndef MW_UNSAVEABLE_RECORDS
bool ItemRecord::saveToStream(std::ostream& output) const
{
  // write NPCO
  output.write(reinterpret_cast<const char*>(&cNPCO), 4);
  const uint32_t subLength = 36; // fixed size: 36 bytes
  output.write(reinterpret_cast<const char*>(&subLength), 4);
  // write item count
  output.write(reinterpret_cast<const char*>(&Count), 4);
  // write item ID
  /* The item ID gets truncated here, if it's longer than 31 characters. */
  auto len = Item.length() + 1;
  if (len > 32)
  {
    len = 31;
    std::clog << "ItemRecord::saveToStream: Warning: Item name \"" << Item
              << "\" got truncated.\n";
  }
  output.write(Item.c_str(), len);
  if (len < 32)
  {
    // fill rest with zeroes
    output.write(NULof32, 32 - len);
  }
  return output.good();
}
#endif

} // namespace
