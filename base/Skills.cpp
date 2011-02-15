/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "Skills.h"
#include <iostream>
#include "MW_Constants.h"
#include "HelperIO.h"

bool SkillRecord::equals(const SkillRecord& other) const
{
  return ((Attribute==other.Attribute) and (Specialization==other.Specialization)
         and (Description==other.Description) and (UseValue[0]==other.UseValue[0])
         and (UseValue[1]==other.UseValue[1]) and (UseValue[2]==other.UseValue[2])
         and (UseValue[3]==other.UseValue[3]));
}

bool SkillRecord::saveToStream(std::ofstream& output, const int32_t SkillIndex) const
{
  output.write((char*) &cSKIL, 4);
  int32_t Size, HeaderOne, H_Flags;
  HeaderOne = H_Flags = 0;
  Size = 4 /* INDX */ +4 /* 4 bytes for length */ +4 /* 4 bytes for index */
        +4 /* SKDT */ +4 /* 4 bytes for length */ +24 /* 24 bytes for Skill data */
        +4 /* DESC */ +4 /* 4 bytes for length */
        +Description.length()+1 /* length of description +1 for NUL-termination */;
  output.write((char*) &Size, 4);
  #warning HeaderOne and H_Flags might not be initialized properly!
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &H_Flags, 4);

  /*Skills:
    INDX = Skill ID (4 bytes, long)
        The Skill ID (0 to 26) since skills are hardcoded in the game
    SKDT = Skill Data (24 bytes)
        long Attribute
        long Specialization (0 = Combat,1 = Magic,2 = Stealth)
        float UseValue[4] (The use types for each skill are hard-coded.)
    DESC = Skill description string */

  //write INDX
  output.write((char*) &cINDX, 4);
  //write length
  int32_t SubLength = 4;
  output.write((char*) &SubLength, 4);
  //write index
  output.write((char*) &SkillIndex, 4);
  //write SKDT
  output.write((char*) &cSKDT, 4);
  //write length
  SubLength = 24;
  output.write((char*) &SubLength, 4);
  //write skill data
  output.write((char*) &Attribute, 4);
  output.write((char*) &Specialization, 4);
  output.write((char*) &(UseValue[0]), 4);
  output.write((char*) &(UseValue[1]), 4);
  output.write((char*) &(UseValue[2]), 4);
  output.write((char*) &(UseValue[3]), 4);
  //write DESC
  output.write((char*) &cDESC, 4);
  //write length
  SubLength = Description.length()+1;
  output.write((char*) &SubLength, 4);
  //write description text
  output.write(Description.c_str(), SubLength);
  return output.good();
}

Skills::Skills()
{
  //empty
}

Skills::~Skills()
{
  m_Skills.clear();
}

Skills& Skills::getSingleton()
{
  static Skills Instance;
  return Instance;
}

void Skills::addSkill(const int Index, const SkillRecord& SkillData)
{
  m_Skills[Index] = SkillData;
}

bool Skills::hasSkill(const int Index) const
{
  return m_Skills.find(Index)!=m_Skills.end();
}

const SkillRecord& Skills::getSkillData(const int Index) const
{
  const std::map<int, SkillRecord>::const_iterator iter = m_Skills.find(Index);
  if (iter!=m_Skills.end())
  {
    return iter->second;
  }
  throw 42;
}

std::string Skills::getSettingNameForSkill(const int32_t Index)
{
  if ((Index<0) or (Index>26))
  {
    std::cout << "No setting defined for skill index "<<Index<<".\n";
    throw 42;
  }
  switch(Index)
  {
    case SkillBlock:
         return "sSkillBlock";
         break;
    case SkillArmorer:
         return "sSkillArmorer";
         break;
    case SkillMediumArmor:
         return "sSkillMediumarmor";
         break;
    case SkillHeavyArmor:
         return "sSkillHeavyarmor";
         break;
    case SkillBluntWeapon:
         return "sSkillBluntweapon";
         break;
    case SkillLongBlade:
         return "sSkillLongblade";
         break;
    case SkillAxe:
         return "sSkillAxe";
         break;
    case SkillSpear:
         return "sSkillSpear";
         break;
    case SkillAthletics:
         return "sSkillAthletics";
         break;
    case SkillEnchant:
         return "sSkillEnchant";
         break;
    case SkillDestruction:
         return "sSkillDestruction";
         break;
    case SkillAlteration:
         return "sSkillAlteration";
         break;
    case SkillIllusion:
         return "sSkillIllusion";
         break;
    case SkillConjuration:
         return "sSkillConjuration";
         break;
    case SkillMysticism:
         return "sSkillMysticism";
         break;
    case SkillRestoration:
         return "sSkillRestoration";
         break;
    case SkillAlchemy:
         return "sSkillAlchemy";
         break;
    case SkillUnarmored:
         return "sSkillUnarmored";
         break;
    case SkillSecurity:
         return "sSkillSecurity";
         break;
    case SkillSneak:
         return "sSkillSneak";
         break;
    case SkillAcrobatics:
         return "sSkillAcrobatics";
         break;
    case SkillLightArmor:
         return "sSkillLightarmor";
         break;
    case SkillShortBlade:
         return "sSkillShortblade";
         break;
    case SkillMarksman:
         return "sSkillMarksman";
         break;
    case SkillMercantile:
         return "sSkillMercantile";
         break;
    case SkillSpeechcraft:
         return "sSkillSpeechcraft";
         break;
    case SkillHandToHand:
         return "sSkillHandtohand";
         break;
  }//swi
  std::cout << "No setting defined for skill index "<<Index<<".\n";
  throw 42;
}

unsigned int Skills::getNumberOfSkills() const
{
  return m_Skills.size();
}

int Skills::readRecordSKIL(std::ifstream& in_File)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Skills:
    INDX = Skill ID (4 bytes, long)
        The Skill ID (0 to 26) since skills are hardcoded in the game
    SKDT = Skill Data (24 bytes)
        long Attribute
        long Specialization (0 = Combat,1 = Magic,2 = Stealth)
        float UseValue[4] (The use types for each skill are hard-coded.)
    DESC = Skill description string */

  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read INDX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cINDX)
  {
    UnexpectedRecord(cINDX, SubRecName);
    return -1;
  }
  //INDX's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout << "Error: sub record INDX of SKIL has invalid length ("
              <<SubLength<< " bytes). Should be four bytes.\n";
    return -1;
  }
  int32_t SkillIndex = -1;
  in_File.read((char*) &SkillIndex, 4);//read the skill index
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record INDX of SKIL.\n";
    return -1;
  }

  //read SKDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cSKDT)
  {
    UnexpectedRecord(cSKDT, SubRecName);
    return -1;
  }
  //SKDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=24)
  {
    std::cout << "Error: sub record SKDT of SKIL has invalid length ("
              <<SubLength<< " bytes). Should be 24 bytes.\n";
    return -1;
  }
  //read skill data
  SkillRecord temp;
  in_File.read((char*) &(temp.Attribute), 4);
  in_File.read((char*) &(temp.Specialization), 4);
  int i;
  for (i=0; i<4; ++i)
  {
    in_File.read((char*) &(temp.UseValue[i]), 4);
  }
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record SKDT of SKIL.\n";
    return -1;
  }

  //read DESC
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cDESC)
  {
    UnexpectedRecord(cDESC, SubRecName);
    return -1;
  }
  //DESC's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>511)
  {
    std::cout << "Sub record DESC of SKIL is longer than 511 characters.\n";
    return -1;
  }
  //read the skill description
  char Buffer[512];
  memset(Buffer, '\0', 512);
  in_File.read(Buffer, SubLength);
  if (in_File.good())
  {
    temp.Description = std::string(Buffer);
    if (hasSkill(SkillIndex))
    {
      if (getSkillData(SkillIndex).equals(temp))
      {
        //Skill is equal to the already present skill record, return zero.
        return 0;
      }
    }//if skill present
    //add new or changed skill
    addSkill(SkillIndex, temp);
    return 1;
  }
  return -1;
}//readRecordSKIL

SkillListIterator Skills::getBegin() const
{
  return m_Skills.begin();
}

SkillListIterator Skills::getEnd() const
{
  return m_Skills.end();
}

bool Skills::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Skills::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  SkillListIterator iter = m_Skills.begin();
  const SkillListIterator end_iter = m_Skills.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output, iter->first))
    {
      std::cout << "Skills::saveAllToStream: Error while writing record for "
                << "skill index "<<iter->first<<".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Skills::clearAll()
{
  m_Skills.clear();
}
