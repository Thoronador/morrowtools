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

#include "Lockpicks.h"
#include <iostream>

namespace MWTP
{

Lockpicks::Lockpicks()
{
  //empty
}

Lockpicks::~Lockpicks()
{
  //empty
}

Lockpicks& Lockpicks::getSingleton()
{
  static Lockpicks Instance;
  return Instance;
}

void Lockpicks::addLockpick(const LockpickRecord& record)
{
  if (record.LockpickID!="")
  {
    m_Lockpicks[record.LockpickID] = record;
  }
}

bool Lockpicks::hasLockpick(const std::string& ID) const
{
  return (m_Lockpicks.find(ID)!=m_Lockpicks.end());
}

unsigned int Lockpicks::getNumberOfLockpicks() const
{
  return m_Lockpicks.size();
}

const LockpickRecord& Lockpicks::getLockpick(const std::string& ID) const
{
  std::map<std::string, LockpickRecord>::const_iterator iter = m_Lockpicks.find(ID);
  if (iter!=m_Lockpicks.end())
  {
    return iter->second;
  }
  std::cout << "No lockpick with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

LockpickListIterator Lockpicks::getBegin() const
{
  return m_Lockpicks.begin();
}

LockpickListIterator Lockpicks::getEnd() const
{
  return m_Lockpicks.end();
}

bool Lockpicks::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Lockpicks::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  LockpickListIterator iter = m_Lockpicks.begin();
  const LockpickListIterator end_iter = m_Lockpicks.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Lockpicks::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Lockpicks::clearAll()
{
  m_Lockpicks.clear();
}

int Lockpicks::readRecordLOCK(std::ifstream& in_File)
{
  LockpickRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Lockpicks::readRecordLOCK: Error while reading lockpick record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasLockpick(temp.LockpickID))
  {
    if (getLockpick(temp.LockpickID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if lockpick present
  addLockpick(temp);
  return 1;
} //readRecordLOCK

} //namespace
