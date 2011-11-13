/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
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

#include "LeveledCreatures.h"
#include <iostream>

namespace MWTP
{

LeveledCreatures::LeveledCreatures()
{
  //empty
}

LeveledCreatures::~LeveledCreatures()
{
  //empty
}

LeveledCreatures& LeveledCreatures::getSingleton()
{
  static LeveledCreatures Instance;
  return Instance;
}

void LeveledCreatures::addLeveledCreature(const LeveledCreatureRecord& record)
{
  if (record.LevCreatureID!="")
  {
    m_LeveledCreatures[record.LevCreatureID] = record;
  }
}

bool LeveledCreatures::hasLeveledCreature(const std::string& ID) const
{
  return (m_LeveledCreatures.find(ID)!=m_LeveledCreatures.end());
}

unsigned int LeveledCreatures::getNumberOfLeveledCreatures() const
{
  return m_LeveledCreatures.size();
}

const LeveledCreatureRecord& LeveledCreatures::getLeveledCreature(const std::string& ID) const
{
  std::map<std::string, LeveledCreatureRecord>::const_iterator iter = m_LeveledCreatures.find(ID);
  if (iter!=m_LeveledCreatures.end())
  {
    return iter->second;
  }
  std::cout << "No leveled creature with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

LeveledCreatureListIterator LeveledCreatures::getBegin() const
{
  return m_LeveledCreatures.begin();
}

LeveledCreatureListIterator LeveledCreatures::getEnd() const
{
  return m_LeveledCreatures.end();
}

bool LeveledCreatures::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "LeveledCreatures::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  LeveledCreatureListIterator iter = m_LeveledCreatures.begin();
  const LeveledCreatureListIterator end_iter = m_LeveledCreatures.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "LeveledCreatures::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void LeveledCreatures::clearAll()
{
  m_LeveledCreatures.clear();
}

int LeveledCreatures::readRecordLEVC(std::ifstream& in_File)
{
  LeveledCreatureRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "LeveledCreatures::readRecordLEVC: Error while reading leveled"
              << " creature record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasLeveledCreature(temp.LevCreatureID))
  {
    if (getLeveledCreature(temp.LevCreatureID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if leveled creature present
  addLeveledCreature(temp);
  return 1;
} //readRecordLEVC

} //namespace
