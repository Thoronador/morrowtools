/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2012 Thoronador

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

#include "ESMWriterSkillRebalance.h"
#include "../base/GameSettings.h"
#include "../base/Skills.h"

namespace MWTP
{

ESMWriterSkillRebalance::ESMWriterSkillRebalance()
{
  //empty
}

ESMWriterSkillRebalance::~ESMWriterSkillRebalance()
{
  //empty
}

int32_t ESMWriterSkillRebalance::getTotalRecords() const
{
  //writes settings and skills
  return GameSettings::getSingleton().getNumberOfSettings()
        +Skills::getSingleton().getNumberOfSkills();
}

bool ESMWriterSkillRebalance::writeRecords(std::ofstream& output) const
{
  if (!GameSettings::getSingleton().saveAllToStream(output)) return false;
  return Skills::getSingleton().saveAllToStream(output);
}

} //namespace
