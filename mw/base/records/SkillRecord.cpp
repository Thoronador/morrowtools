/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2013, 2021  Thoronador

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

#include "SkillRecord.hpp"
#include <cstring>
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

SkillRecord::SkillRecord()
: BasicRecord(),
  SkillIndex(0),
  Attribute(0),
  Specialization(0),
  UseValues({ 0.0f, 0.0f, 0.0f, 0.0f }),
  Description("")
{
}

bool SkillRecord::equals(const SkillRecord& other) const
{
  return (Attribute == other.Attribute) && (Specialization == other.Specialization)
      && (Description == other.Description) && (UseValues == other.UseValues)
      && (SkillIndex == other.SkillIndex);
}

bool SkillRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cSKIL), 4);
  const uint32_t Size = 4 /* INDX */ + 4 /* 4 bytes for length */ + 4 /* 4 bytes for index */
        + 4 /* SKDT */ + 4 /* 4 bytes for length */ + 24 /* 24 bytes for Skill data */
        + 4 /* DESC */ + 4 /* 4 bytes for length */
        + Description.length() /* length of description, no NUL-termination */;
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Skills:
    INDX = Skill ID (4 bytes, long)
        The Skill ID (0 to 26) since skills are hardcoded in the game
    SKDT = Skill Data (24 bytes)
        long Attribute
        long Specialization (0 = Combat,1 = Magic,2 = Stealth)
        float UseValue[4] (The use types for each skill are hard-coded.)
    DESC = Skill description string */

  // write skill index (INDX)
  output.write(reinterpret_cast<const char*>(&cINDX), 4);
  uint32_t SubLength = 4;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&SkillIndex), 4);

  // write skill data (SKDT)
  output.write(reinterpret_cast<const char*>(&cSKDT), 4);
  SubLength = 24;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // skill data
  output.write(reinterpret_cast<const char*>(&Attribute), 4);
  output.write(reinterpret_cast<const char*>(&Specialization), 4);
  output.write(reinterpret_cast<const char*>(UseValues.data()), 16);

  // write description text (DESC)
  output.write(reinterpret_cast<const char*>(&cDESC), 4);
  SubLength = Description.length();
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Description.c_str(), SubLength);
  return output.good();
}

bool SkillRecord::loadFromStream(std::istream& in_File)
{
  uint32_t Size = 0;
  in_File.read(reinterpret_cast<char*>(&Size), 4);
  in_File.read(reinterpret_cast<char*>(&HeaderOne), 4);
  in_File.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Skills:
    INDX = Skill ID (4 bytes, long)
        The Skill ID (0 to 26) since skills are hardcoded in the game
    SKDT = Skill Data (24 bytes)
        long Attribute
        long Specialization (0 = Combat,1 = Magic,2 = Stealth)
        float UseValue[4] (The use types for each skill are hard-coded.)
    DESC = Skill description string */

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;

  // read INDX
  in_File.read(reinterpret_cast<char*>(&SubRecName), 4);
  if (SubRecName != cINDX)
  {
    UnexpectedRecord(cINDX, SubRecName);
    return false;
  }
  // INDX's length
  in_File.read(reinterpret_cast<char*>(&SubLength), 4);
  if (SubLength != 4)
  {
    std::cerr << "Error: sub record INDX of SKIL has invalid length ("
              << SubLength << " bytes). Should be four bytes.\n";
    return false;
  }
  SkillIndex = -1;
  in_File.read(reinterpret_cast<char*>(&SkillIndex), 4);
  if (!in_File.good())
  {
    std::cerr << "Error while reading sub record INDX of SKIL.\n";
    return false;
  }

  // read SKDT
  in_File.read(reinterpret_cast<char*>(&SubRecName), 4);
  if (SubRecName != cSKDT)
  {
    UnexpectedRecord(cSKDT, SubRecName);
    return false;
  }
  // SKDT's length
  in_File.read(reinterpret_cast<char*>(&SubLength), 4);
  if (SubLength != 24)
  {
    std::cerr << "Error: sub record SKDT of SKIL has invalid length ("
              << SubLength << " bytes). Should be 24 bytes.\n";
    return false;
  }
  // read skill data
  in_File.read(reinterpret_cast<char*>(&Attribute), 4);
  in_File.read(reinterpret_cast<char*>(&Specialization), 4);
  in_File.read(reinterpret_cast<char*>(UseValues.data()), 16);
  if (!in_File.good())
  {
    std::cerr << "Error while reading sub record SKDT of SKIL.\n";
    return false;
  }

  // read DESC
  in_File.read(reinterpret_cast<char*>(&SubRecName), 4);
  if (SubRecName != cDESC)
  {
    UnexpectedRecord(cDESC, SubRecName);
    return false;
  }
  // DESC's length
  in_File.read(reinterpret_cast<char*>(&SubLength), 4);
  if (SubLength > 511)
  {
    std::cerr << "Sub record DESC of SKIL is longer than 511 characters.\n";
    return false;
  }
  // read the skill description
  char Buffer[512];
  memset(Buffer, '\0', 512);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cerr << "Error while reading sub record DESC of SKIL.\n";
    return false;
  }
  Description = std::string(Buffer);
  return true;
}

} // namespace
