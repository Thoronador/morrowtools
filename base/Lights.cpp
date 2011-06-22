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

#include "Lights.h"
#include <iostream>

namespace MWTP
{

Lights::Lights()
{
  //empty
}

Lights::~Lights()
{
  //empty
}

Lights& Lights::getSingleton()
{
  static Lights Instance;
  return Instance;
}

void Lights::addLight(const LightRecord& record)
{
  if (record.LightID!="")
  {
    m_Lights[record.LightID] = record;
  }
}

bool Lights::hasLight(const std::string& ID) const
{
  return (m_Lights.find(ID)!=m_Lights.end());
}

unsigned int Lights::getNumberOfLights() const
{
  return m_Lights.size();
}

const LightRecord& Lights::getLight(const std::string& ID) const
{
  std::map<std::string, LightRecord>::const_iterator iter = m_Lights.find(ID);
  if (iter!=m_Lights.end())
  {
    return iter->second;
  }
  std::cout << "No light with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

LightListIterator Lights::getBegin() const
{
  return m_Lights.begin();
}

LightListIterator Lights::getEnd() const
{
  return m_Lights.end();
}

bool Lights::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Lights::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  LightListIterator iter = m_Lights.begin();
  const LightListIterator end_iter = m_Lights.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Lights::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Lights::clearAll()
{
  m_Lights.clear();
}

int Lights::readRecordLIGH(std::ifstream& in_File)
{
  LightRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Lights::readRecordLIGH: Error while reading light record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasLight(temp.LightID))
  {
    if (getLight(temp.LightID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if light present
  addLight(temp);
  return 1;
} //readRecordLIGH

} //namespace
