/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012  Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "LeveledListBaseRecord.h"

namespace SRTP
{

const uint8_t LeveledListBaseRecord::cFlagCalcFromAllLevels = 0x01;
const uint8_t LeveledListBaseRecord::cFlagCalcForEach       = 0x02;

bool LeveledListBaseRecord::LeveledListEntry::operator==(const LeveledListBaseRecord::LeveledListEntry& other) const
{
  return ((level==other.level) and (formID==other.formID) and (count==other.count));
}

bool LeveledListBaseRecord::COEDData::operator==(const LeveledListBaseRecord::COEDData& other) const
{
  return ((factionFormID==other.factionFormID) and (requiredRank==other.requiredRank)
      and (unknownFloat==other.unknownFloat));
}

void LeveledListBaseRecord::COEDData::init()
{
  factionFormID = 0;
  requiredRank = 0xFFFFFFFF;
  unknownFloat = 1.0f;
}

bool LeveledListBaseRecord::COEDData::isInit() const
{
  return ((factionFormID==0) and (requiredRank==0xFFFFFFFF) and (unknownFloat==1.0f));
}

bool LeveledListBaseRecord::EntryWithCOED::operator==(const LeveledListBaseRecord::EntryWithCOED& other) const
{
  return ((entry==other.entry) and (coed==other.coed));
}

LeveledListBaseRecord::LeveledListBaseRecord()
: BasicRecord()
{
  entries.clear();
}

LeveledListBaseRecord::~LeveledListBaseRecord()
{
  //empty
}

bool LeveledListBaseRecord::calculateFromAllLevels() const
{
  return ((flags & cFlagCalcFromAllLevels) !=0);
}

bool LeveledListBaseRecord::calculateForEachItem() const
{
  return ((flags & cFlagCalcForEach) !=0);
}

} //namespace
