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

#include "AlchemyPotions.h"
#include <iostream>

AlchemyPotions::AlchemyPotions()
{
  //empty
}

AlchemyPotions::~AlchemyPotions()
{
  //empty
}

AlchemyPotions& AlchemyPotions::getSingleton()
{
  static AlchemyPotions Instance;
  return Instance;
}

void AlchemyPotions::addAlchemyPotion(const AlchemyPotionRecord& record)
{
  if (record.AlchemyID!="")
  {
    m_AlchemyPotions[record.AlchemyID] = record;
  }
}

bool AlchemyPotions::hasAlchemyPotion(const std::string& ID) const
{
  return (m_AlchemyPotions.find(ID)!=m_AlchemyPotions.end());
}

unsigned int AlchemyPotions::getNumberOfAlchemyPotions() const
{
  return m_AlchemyPotions.size();
}

const AlchemyPotionRecord& AlchemyPotions::getAlchemyPotion(const std::string& ID) const
{
  std::map<std::string, AlchemyPotionRecord>::const_iterator iter = m_AlchemyPotions.find(ID);
  if (iter!=m_AlchemyPotions.end())
  {
    return iter->second;
  }
  std::cout << "No potion with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

AlchemyPotionListIterator AlchemyPotions::getBegin() const
{
  return m_AlchemyPotions.begin();
}

AlchemyPotionListIterator AlchemyPotions::getEnd() const
{
  return m_AlchemyPotions.end();
}

bool AlchemyPotions::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "AlchemyPotions::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  AlchemyPotionListIterator iter = m_AlchemyPotions.begin();
  const AlchemyPotionListIterator end_iter = m_AlchemyPotions.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "AlchemyPotions::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void AlchemyPotions::clearAll()
{
  m_AlchemyPotions.clear();
}

int AlchemyPotions::readRecordALCH(std::ifstream& in_File)
{
  AlchemyPotionRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "AlchemyPotions::readRecordALCH: Error while reading "
              << "alchemy potion record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasAlchemyPotion(temp.AlchemyID))
  {
    if (getAlchemyPotion(temp.AlchemyID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if potion present
  addAlchemyPotion(temp);
  return 1;
} //readRecordALCH
