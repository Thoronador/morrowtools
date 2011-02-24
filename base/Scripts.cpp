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

#include "Scripts.h"
#include <iostream>

Scripts::Scripts()
{
  //empty
}

Scripts::~Scripts()
{
  //empty
}

Scripts& Scripts::getSingleton()
{
  static Scripts Instance;
  return Instance;
}

void Scripts::addScript(const ScriptRecord& record)
{
  if (record.ScriptID!="")
  {
    m_Scripts[record.ScriptID] = record;
  }
}

bool Scripts::hasScript(const std::string& ID) const
{
  return (m_Scripts.find(ID)!=m_Scripts.end());
}

unsigned int Scripts::getNumberOfScripts() const
{
  return m_Scripts.size();
}

const ScriptRecord& Scripts::getScript(const std::string& ID) const
{
  std::map<std::string, ScriptRecord>::const_iterator iter = m_Scripts.find(ID);
  if (iter!=m_Scripts.end())
  {
    return iter->second;
  }
  std::cout << "No script with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

ScriptListIterator Scripts::getBegin() const
{
  return m_Scripts.begin();
}

ScriptListIterator Scripts::getEnd() const
{
  return m_Scripts.end();
}

bool Scripts::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Scripts::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  ScriptListIterator iter = m_Scripts.begin();
  const ScriptListIterator end_iter = m_Scripts.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Scripts::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Scripts::clearAll()
{
  m_Scripts.clear();
}

int Scripts::readRecordSCPT(std::ifstream& in_File)
{
  ScriptRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Scripts::readRecordSCPT: Error while reading script "
              << "record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasScript(temp.ScriptID))
  {
    if (getScript(temp.ScriptID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if script present
  addScript(temp);
  return 1;
} //readRecordSCPT
