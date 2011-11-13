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

#include "MiscItems.h"
#include <iostream>

namespace MWTP
{

MiscItems::MiscItems()
{
  //empty
}

MiscItems::~MiscItems()
{
  //empty
}

MiscItems& MiscItems::getSingleton()
{
  static MiscItems Instance;
  return Instance;
}

void MiscItems::addMiscItem(const MiscItemRecord& record)
{
  if (record.MiscItemID!="")
  {
    m_MiscItems[record.MiscItemID] = record;
  }
}

bool MiscItems::hasMiscItem(const std::string& ID) const
{
  return (m_MiscItems.find(ID)!=m_MiscItems.end());
}

unsigned int MiscItems::getNumberOfMiscItems() const
{
  return m_MiscItems.size();
}

const MiscItemRecord& MiscItems::getMiscItem(const std::string& ID) const
{
  std::map<std::string, MiscItemRecord>::const_iterator iter = m_MiscItems.find(ID);
  if (iter!=m_MiscItems.end())
  {
    return iter->second;
  }
  std::cout << "No misc. item with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

MiscItemListIterator MiscItems::getBegin() const
{
  return m_MiscItems.begin();
}

MiscItemListIterator MiscItems::getEnd() const
{
  return m_MiscItems.end();
}

bool MiscItems::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "MiscItems::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  MiscItemListIterator iter = m_MiscItems.begin();
  const MiscItemListIterator end_iter = m_MiscItems.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "MiscItems::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void MiscItems::clearAll()
{
  m_MiscItems.clear();
}

int MiscItems::readRecordMISC(std::ifstream& in_File)
{
  MiscItemRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "MiscItems::readRecordMISC: Error while reading misc. item "
              << "record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasMiscItem(temp.MiscItemID))
  {
    if (getMiscItem(temp.MiscItemID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if misc. item present
  addMiscItem(temp);
  return 1;
} //readRecordMISC

} //namespace
