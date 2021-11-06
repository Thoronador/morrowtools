/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2013  Thoronador

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
  Description("")
{
  UseValue[0] = UseValue[1] = UseValue[2] = UseValue[3] = 0.0f;
}

bool SkillRecord::equals(const SkillRecord& other) const
{
  return ((Attribute==other.Attribute) and (Specialization==other.Specialization)
         and (Description==other.Description) and (UseValue[0]==other.UseValue[0])
         and (UseValue[1]==other.UseValue[1]) and (UseValue[2]==other.UseValue[2])
         and (UseValue[3]==other.UseValue[3]) and (SkillIndex==other.SkillIndex));
}

bool SkillRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cSKIL, 4);
  uint32_t Size;
  Size = 4 /* INDX */ +4 /* 4 bytes for length */ +4 /* 4 bytes for index */
        +4 /* SKDT */ +4 /* 4 bytes for length */ +24 /* 24 bytes for Skill data */
        +4 /* DESC */ +4 /* 4 bytes for length */
        +Description.length()+1 /* length of description +1 for NUL-termination */;
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*Skills:
    INDX = Skill ID (4 bytes, long)
        The Skill ID (0 to 26) since skills are hardcoded in the game
    SKDT = Skill Data (24 bytes)
        long Attribute
        long Specialization (0 = Combat,1 = Magic,2 = Stealth)
        float UseValue[4] (The use types for each skill are hard-coded.)
    DESC = Skill description string */

  //write INDX
  output.write((const char*) &cINDX, 4);
  //write length
  uint32_t SubLength = 4;
  output.write((const char*) &SubLength, 4);
  //write index
  output.write((const char*) &SkillIndex, 4);
  //write SKDT
  output.write((const char*) &cSKDT, 4);
  //write length
  SubLength = 24;
  output.write((const char*) &SubLength, 4);
  //write skill data
  output.write((const char*) &Attribute, 4);
  output.write((const char*) &Specialization, 4);
  output.write((const char*) &(UseValue[0]), 4);
  output.write((const char*) &(UseValue[1]), 4);
  output.write((const char*) &(UseValue[2]), 4);
  output.write((const char*) &(UseValue[3]), 4);
  //write DESC
  output.write((const char*) &cDESC, 4);
  //write length
  SubLength = Description.length()+1;
  output.write((const char*) &SubLength, 4);
  //write description text
  output.write(Description.c_str(), SubLength);
  return output.good();
}

bool SkillRecord::loadFromStream(std::istream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Skills:
    INDX = Skill ID (4 bytes, long)
        The Skill ID (0 to 26) since skills are hardcoded in the game
    SKDT = Skill Data (24 bytes)
        long Attribute
        long Specialization (0 = Combat,1 = Magic,2 = Stealth)
        float UseValue[4] (The use types for each skill are hard-coded.)
    DESC = Skill description string */

  uint32_t SubRecName;
  uint32_t SubLength;
  SubRecName = SubLength = 0;

  //read INDX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cINDX)
  {
    UnexpectedRecord(cINDX, SubRecName);
    return false;
  }
  //INDX's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout << "Error: sub record INDX of SKIL has invalid length ("
              <<SubLength<< " bytes). Should be four bytes.\n";
    return false;
  }
  SkillIndex = -1;
  in_File.read((char*) &SkillIndex, 4);//read the skill index
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record INDX of SKIL.\n";
    return false;
  }

  //read SKDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cSKDT)
  {
    UnexpectedRecord(cSKDT, SubRecName);
    return false;
  }
  //SKDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=24)
  {
    std::cout << "Error: sub record SKDT of SKIL has invalid length ("
              <<SubLength<< " bytes). Should be 24 bytes.\n";
    return false;
  }
  //read skill data
  in_File.read((char*) &Attribute, 4);
  in_File.read((char*) &Specialization, 4);
  int i;
  for (i=0; i<4; ++i)
  {
    in_File.read((char*) &(UseValue[i]), 4);
  }
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record SKDT of SKIL.\n";
    return false;
  }

  //read DESC
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cDESC)
  {
    UnexpectedRecord(cDESC, SubRecName);
    return false;
  }
  //DESC's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>511)
  {
    std::cout << "Sub record DESC of SKIL is longer than 511 characters.\n";
    return false;
  }
  //read the skill description
  char Buffer[512];
  memset(Buffer, '\0', 512);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record DESC of SKIL.\n";
    return false;
  }
  Description = std::string(Buffer);
  return true;
}

} //namespace
