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

namespace MWTP
{

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

void Skills::addSkill(const SkillRecord& SkillData)
{
  m_Skills[SkillData.SkillIndex] = SkillData;
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
  SkillRecord temp;
  if (temp.loadFromStream(in_File))
  {
    if (hasSkill(temp.SkillIndex))
    {
      if (getSkillData(temp.SkillIndex).equals(temp))
      {
        //Skill is equal to the already present skill record, return zero.
        return 0;
      }
    }//if skill present
    //add new or changed skill
    addSkill(temp);
    return 1;
  }
  std::cout << "readRecordSKIL: Error while reading record.\n";
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
    if (!iter->second.saveToStream(output))
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

} //namespace
