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

#include "LeveledItems.h"
#include <iostream>

LeveledItems::LeveledItems()
{
  //empty
}

LeveledItems::~LeveledItems()
{
  //empty
}

LeveledItems& LeveledItems::getSingleton()
{
  static LeveledItems Instance;
  return Instance;
}

void LeveledItems::addLeveledItem(const LeveledItemRecord& record)
{
  if (record.LevItemID!="")
  {
    m_LeveledItems[record.LevItemID] = record;
  }
}

bool LeveledItems::hasLeveledItem(const std::string& ID) const
{
  return (m_LeveledItems.find(ID)!=m_LeveledItems.end());
}

unsigned int LeveledItems::getNumberOfLeveledItems() const
{
  return m_LeveledItems.size();
}

const LeveledItemRecord& LeveledItems::getLeveledItem(const std::string& ID) const
{
  std::map<std::string, LeveledItemRecord>::const_iterator iter = m_LeveledItems.find(ID);
  if (iter!=m_LeveledItems.end())
  {
    return iter->second;
  }
  std::cout << "No leveled item with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

LeveledItemListIterator LeveledItems::getBegin() const
{
  return m_LeveledItems.begin();
}

LeveledItemListIterator LeveledItems::getEnd() const
{
  return m_LeveledItems.end();
}

bool LeveledItems::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "LeveledItems::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  LeveledItemListIterator iter = m_LeveledItems.begin();
  const LeveledItemListIterator end_iter = m_LeveledItems.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "LeveledItems::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void LeveledItems::clearAll()
{
  m_LeveledItems.clear();
}

int LeveledItems::readRecordLEVI(std::ifstream& in_File)
{
  LeveledItemRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "LeveledItems::readRecordLEVI: Error while reading leveled"
              << " item record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasLeveledItem(temp.LevItemID))
  {
    if (getLeveledItem(temp.LevItemID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if leveled item present
  addLeveledItem(temp);
  return 1;
} //readRecordLEVI
