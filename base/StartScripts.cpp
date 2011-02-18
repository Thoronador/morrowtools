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

#include "StartScripts.h"
#include <iostream>

StartScripts::StartScripts()
{
  //empty
}

StartScripts::~StartScripts()
{
  //empty
}

StartScripts& StartScripts::getSingleton()
{
  static StartScripts Instance;
  return Instance;
}

void StartScripts::addStartScript(const StartScriptRecord& record)
{
  if (record.Name!="")
  {
    m_StartScripts[record.Name] = record;
  }
}

bool StartScripts::hasStartScript(const std::string& ID) const
{
  return (m_StartScripts.find(ID)!=m_StartScripts.end());
}

unsigned int StartScripts::getNumberOfStartScripts() const
{
  return m_StartScripts.size();
}

const StartScriptRecord& StartScripts::getStartScript(const std::string& ID) const
{
  std::map<std::string, StartScriptRecord>::const_iterator iter = m_StartScripts.find(ID);
  if (iter!=m_StartScripts.end())
  {
    return iter->second;
  }
  std::cout << "No book with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

StartScriptListIterator StartScripts::getBegin() const
{
  return m_StartScripts.begin();
}

StartScriptListIterator StartScripts::getEnd() const
{
  return m_StartScripts.end();
}

bool StartScripts::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "StartScripts::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  StartScriptListIterator iter = m_StartScripts.begin();
  const StartScriptListIterator end_iter = m_StartScripts.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "StartScripts::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void StartScripts::clearAll()
{
  m_StartScripts.clear();
}

int StartScripts::readRecordSSCR(std::ifstream& in_File)
{
  StartScriptRecord temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "readRecordSSCR: Error while reading start script record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasStartScript(temp.Name))
  {
    if (getStartScript(temp.Name).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if start script present
  addStartScript(temp);
  return 1;
} //readRecordSSCR
