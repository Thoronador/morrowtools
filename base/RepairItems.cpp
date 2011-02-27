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

#include "RepairItems.h"
#include <iostream>

RepairItems::RepairItems()
{
  //empty
}

RepairItems::~RepairItems()
{
  //empty
}

RepairItems& RepairItems::getSingleton()
{
  static RepairItems Instance;
  return Instance;
}

void RepairItems::addRepairItem(const RepairItemRecord& record)
{
  if (record.RepairItemID!="")
  {
    m_RepairItems.insert(record);
  }
}

bool RepairItems::hasRepairItem(const std::string& ID) const
{
  return (m_RepairItems.find(ID)!=m_RepairItems.end());
}

unsigned int RepairItems::getNumberOfRepairItems() const
{
  return m_RepairItems.size();
}

const RepairItemRecord& RepairItems::getRepairItem(const std::string& ID) const
{
  RepairItemListIterator iter = m_RepairItems.find(ID);
  if (iter!=m_RepairItems.end())
  {
    return *iter;
  }
  std::cout << "No repair item with ID \""<<ID<<"\" is present.\n";
  throw 42;
}

RepairItemListIterator RepairItems::getBegin() const
{
  return m_RepairItems.begin();
}

RepairItemListIterator RepairItems::getEnd() const
{
  return m_RepairItems.end();
}

bool RepairItems::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "RepairItems::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  RepairItemListIterator iter = m_RepairItems.begin();
  const RepairItemListIterator end_iter = m_RepairItems.end();
  while (iter!=end_iter)
  {
    if (!iter->saveToStream(output))
    {
      std::cout << "RepairItems::saveAllToStream: Error while writing record for \""
                << iter->RepairItemID <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void RepairItems::clearAll()
{
  m_RepairItems.clear();
}

int RepairItems::readRecordREPA(std::ifstream& in_File)
{
  RepairItemRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "RepairItems::readRecordREPA: Error while reading repair item"
              << " record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasRepairItem(temp.RepairItemID))
  {
    if (getRepairItem(temp.RepairItemID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if repair item present
  addRepairItem(temp);
  return 1;
} //readRecordREPA
