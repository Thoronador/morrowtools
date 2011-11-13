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

#include "Activators.h"
#include <iostream>

namespace MWTP
{

Activators::Activators()
{
  //empty
}

Activators::~Activators()
{
  //empty
}

Activators& Activators::getSingleton()
{
  static Activators Instance;
  return Instance;
}

void Activators::addActivator(const ActivatorRecord& record)
{
  if (record.ActivatorID!="")
  {
    m_Activators[record.ActivatorID] = record;
  }
}

bool Activators::hasActivator(const std::string& ID) const
{
  return (m_Activators.find(ID)!=m_Activators.end());
}

unsigned int Activators::getNumberOfActivators() const
{
  return m_Activators.size();
}

const ActivatorRecord& Activators::getActivator(const std::string& ID) const
{
  std::map<std::string, ActivatorRecord>::const_iterator iter = m_Activators.find(ID);
  if (iter!=m_Activators.end())
  {
    return iter->second;
  }
  std::cout << "No activator with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

ActivatorListIterator Activators::getBegin() const
{
  return m_Activators.begin();
}

ActivatorListIterator Activators::getEnd() const
{
  return m_Activators.end();
}

bool Activators::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Activators::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  ActivatorListIterator iter = m_Activators.begin();
  const ActivatorListIterator end_iter = m_Activators.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Activators::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Activators::clearAll()
{
  m_Activators.clear();
}

int Activators::readRecordACTI(std::ifstream& in_File)
{
  ActivatorRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Activators::readRecordACTI: Error while reading activator "
              << "record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasActivator(temp.ActivatorID))
  {
    if (getActivator(temp.ActivatorID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if activator present
  addActivator(temp);
  return 1;
} //readRecordACTI

} //namespace
