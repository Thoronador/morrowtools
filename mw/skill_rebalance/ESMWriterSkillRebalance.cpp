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

#include "ESMWriterSkillRebalance.hpp"
#include "../base/GameSettings.hpp"
#include "../base/Skills.hpp"

namespace MWTP
{

ESMWriterSkillRebalance::ESMWriterSkillRebalance()
{
}

int32_t ESMWriterSkillRebalance::getTotalRecords() const
{
  //writes settings and skills
  return GameSettings::get().getNumberOfSettings()
       + Skills::get().getNumberOfSkills();
}

bool ESMWriterSkillRebalance::writeRecords(std::ofstream& output) const
{
  if (!GameSettings::get().saveAllToStream(output))
    return false;
  return Skills::get().saveAllToStream(output);
}

} //namespace
