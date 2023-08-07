/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2013, 2014, 2021  Dirk Stolle

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

#include "Skills.hpp"
#include <iostream>
#include <stdexcept>

namespace MWTP
{

Skills::Skills()
: m_Skills(std::map<int32_t, SkillRecord>())
{
}

Skills& Skills::get()
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

const SkillRecord& Skills::getSkill(const int Index) const
{
  const auto iter = m_Skills.find(Index);
  if (iter != m_Skills.end())
  {
    return iter->second;
  }
  throw std::runtime_error("Skills::getSkill(): No skill data for given index!");
}

std::string Skills::getSettingNameForSkill(const int32_t Index)
{
  switch(Index)
  {
    case SkillBlock:
         return "sSkillBlock";
    case SkillArmorer:
         return "sSkillArmorer";
    case SkillMediumArmor:
         return "sSkillMediumarmor";
    case SkillHeavyArmor:
         return "sSkillHeavyarmor";
    case SkillBluntWeapon:
         return "sSkillBluntweapon";
    case SkillLongBlade:
         return "sSkillLongblade";
    case SkillAxe:
         return "sSkillAxe";
    case SkillSpear:
         return "sSkillSpear";
    case SkillAthletics:
         return "sSkillAthletics";
    case SkillEnchant:
         return "sSkillEnchant";
    case SkillDestruction:
         return "sSkillDestruction";
    case SkillAlteration:
         return "sSkillAlteration";
    case SkillIllusion:
         return "sSkillIllusion";
    case SkillConjuration:
         return "sSkillConjuration";
    case SkillMysticism:
         return "sSkillMysticism";
    case SkillRestoration:
         return "sSkillRestoration";
    case SkillAlchemy:
         return "sSkillAlchemy";
    case SkillUnarmored:
         return "sSkillUnarmored";
    case SkillSecurity:
         return "sSkillSecurity";
    case SkillSneak:
         return "sSkillSneak";
    case SkillAcrobatics:
         return "sSkillAcrobatics";
    case SkillLightArmor:
         return "sSkillLightarmor";
    case SkillShortBlade:
         return "sSkillShortblade";
    case SkillMarksman:
         return "sSkillMarksman";
    case SkillMercantile:
         return "sSkillMercantile";
    case SkillSpeechcraft:
         return "sSkillSpeechcraft";
    case SkillHandToHand:
         return "sSkillHandtohand";
  }
  std::cerr << "No setting defined for skill index " << Index << ".\n";
  throw std::runtime_error("Skills::getSettingNameForSkill(): No setting defined for given skill index.");
}

unsigned int Skills::getNumberOfSkills() const
{
  return m_Skills.size();
}

int Skills::readNextRecord(std::istream& input)
{
  SkillRecord temp;
  if (!temp.loadFromStream(input))
  {
    std::cerr << "Skills::readNextRecord: Error while reading record.\n";
    return -1;
  }
  if (hasSkill(temp.SkillIndex))
  {
    if (getSkill(temp.SkillIndex).equals(temp))
    {
      // Skill is equal to the already present skill record, return zero.
      return 0;
    }
  }
  // add new or changed skill
  addSkill(temp);
  return 1;
}

Skills::Iterator Skills::begin() const
{
  return m_Skills.begin();
}

Skills::Iterator Skills::end() const
{
  return m_Skills.end();
}

#ifndef MW_UNSAVEABLE_RECORDS
bool Skills::saveAllToStream(std::ostream& output) const
{
  if (!output.good())
  {
    std::cerr << "Skills::saveAllToStream: Error: Bad stream.\n";
    return false;
  }
  for (const auto& [idx, record]: m_Skills)
  {
    if (!record.saveToStream(output))
    {
      std::cerr << "Skills::saveAllToStream: Error while writing record for "
                << "skill index " << idx << ".\n";
      return false;
    }
  }
  return output.good();
}
#endif // MW_UNSAVEABLE_RECORDS

void Skills::clear()
{
  m_Skills.clear();
}

} // namespace
