/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2023  Dirk Stolle

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

#include "BirthSignRecord.hpp"
#include <cstring>
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"
#include "../../base/UtilityFunctions.hpp"

namespace MWTP
{

BirthSignRecord::BirthSignRecord()
: BasicRecord(),
  recordID(""),
  Name(""),
  Texture(""),
  Description(""),
  SignSpells(std::vector<std::string>())
{}

BirthSignRecord::BirthSignRecord(const std::string& ID)
: BasicRecord(),
  recordID(ID),
  Name(""),
  Texture(""),
  Description(""),
  SignSpells(std::vector<std::string>())
{}

bool BirthSignRecord::equals(const BirthSignRecord& other) const
{
  return (recordID == other.recordID) && (Name == other.Name)
      && (Texture == other.Texture) && (Description == other.Description)
      && (SignSpells == other.SignSpells);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool BirthSignRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cBSGN), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* FNAM */ + 4 /* 4 bytes for length */
        + Name.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* TNAM */ + 4 /* 4 bytes for length */
        + Texture.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* DESC */ + 4 /* 4 bytes for length */
        + Description.length() + 1 /* length of name +1 byte for NUL termination */
        + SignSpells.size() * (4 /* NPCS */ + 4 /* 4 bytes for length */ + 32 /* fixed length of 32 bytes - rest is filled with NUL */);
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Birth Sign
	NAME = Sign ID string
	FNAM = Sign name string
	TNAM = Texture filename
	DESC = Description string
	NPCS = Spell/ability (32 bytes), multiple
  */

  // write ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write sign's name (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = Name.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Name.c_str(), SubLength);

  // write texture name (TNAM)
  output.write(reinterpret_cast<const char*>(&cTNAM), 4);
  SubLength = Texture.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Texture.c_str(), SubLength);

  // write description tet (DESC)
  output.write(reinterpret_cast<const char*>(&cDESC), 4);
  SubLength = Description.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Description.c_str(), SubLength);

  // write spells / abilities
  for (const auto& spell_id: SignSpells)
  {
    // write spell ID (NPCS)
    output.write(reinterpret_cast<const char*>(&cNPCS), 4);
    // length is fixed at 32 bytes, rest is filled with NULs
    SubLength = 32;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    //write spell ID
    const unsigned int len = spell_id.length() + 1;
    output.write(spell_id.c_str(), len);
    if (len < 32)
    {
      // fill rest up to 32 bytes with null bytes
      output.write(NULof32, 32 - len);
    }
  }

  return output.good();
}
#endif

bool BirthSignRecord::loadFromStream(std::istream& input)
{
  uint32_t Size { 0 };
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Birth Sign
    NAME = Sign ID string
    FNAM = Sign name string
    TNAM = Texture filename
    DESC = Description string
    NPCS = Spell/ability (32 bytes), multiple
  */

  // read record id (NAME)
  uint32_t BytesRead = 0;
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading sub record NAME of BSGN!\n";
    return false;
  }

  // read localized name (FNAM)
  if (!loadString256WithHeader(input, Name, Buffer, cFNAM, BytesRead))
  {
    std::cerr << "Error while reading sub record FNAM of BSGN!\n";
    return false;
  }

  // read texture path (TNAM)
  if (!loadString256WithHeader(input, Texture, Buffer, cTNAM, BytesRead))
  {
    std::cerr << "Error while reading sub record TNAM of BSGN!\n";
    return false;
  }

  // read description text (DESC)
  if (!loadString256WithHeader(input, Description, Buffer, cDESC, BytesRead))
  {
    std::cerr << "Error while reading sub record DESC of BSGN!\n";
    return false;
  }

  // read spells
  SignSpells.clear();

  while (BytesRead < Size)
  {
    // read spell id (NPCS)
    std::string temp;
    if (!loadString256WithHeader(input, temp, Buffer, cNPCS, BytesRead))
    {
      std::cerr << "Error while reading sub record NPCS of BSGN!\n";
      return false;
    }
    SignSpells.emplace_back(temp);
  }

  return input.good();
}

bool operator<(const BirthSignRecord& left, const BirthSignRecord& right)
{
  return lowerCaseCompare(left.recordID, right.recordID) < 0;
}

} // namespace
