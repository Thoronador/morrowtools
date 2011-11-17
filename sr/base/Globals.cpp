/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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

#include "Globals.h"
#include <iostream>

namespace SRTP
{

Globals::Globals()
{
  //empty
}

Globals::~Globals()
{
  //empty
}

Globals& Globals::getSingleton()
{
  static Globals Instance;
  return Instance;
}

void Globals::addGlobal(const GlobalRecord& record)
{
  if (record.editorID!="")
  {
    m_Globals[record.editorID] = record;
  }
}

bool Globals::hasGlobal(const std::string& ID) const
{
  return (m_Globals.find(ID)!=m_Globals.end());
}

unsigned int Globals::getNumberOfGlobals() const
{
  return m_Globals.size();
}

const GlobalRecord& Globals::getGlobal(const std::string& ID) const
{
  std::map<std::string, GlobalRecord>::const_iterator iter = m_Globals.find(ID);
  if (iter!=m_Globals.end())
  {
    return iter->second;
  }
  std::cout << "No global with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

GlobalListIterator Globals::getBegin() const
{
  return m_Globals.begin();
}

GlobalListIterator Globals::getEnd() const
{
  return m_Globals.end();
}

bool Globals::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Globals::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  GlobalListIterator iter = m_Globals.begin();
  const GlobalListIterator end_iter = m_Globals.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Globals::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Globals::clearAll()
{
  m_Globals.clear();
}

int Globals::readNextRecord(std::ifstream& in_File)
{
  GlobalRecord temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "Globals::readNextRecord: Error while reading global record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasGlobal(temp.editorID))
  {
    if (getGlobal(temp.editorID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if global present
  addGlobal(temp);
  return 1;
} //readNextRecord

} //namespace
