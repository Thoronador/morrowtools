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

#include "Probes.h"
#include <iostream>

Probes::Probes()
{
  //empty
}

Probes::~Probes()
{
  //empty
}

Probes& Probes::getSingleton()
{
  static Probes Instance;
  return Instance;
}

void Probes::addProbe(const ProbeRecord& record)
{
  if (record.ProbeID!="")
  {
    m_Probes.insert(record);
  }
}

bool Probes::hasProbe(const std::string& ID) const
{
  return (m_Probes.find(ID)!=m_Probes.end());
}

unsigned int Probes::getNumberOfProbes() const
{
  return m_Probes.size();
}

const ProbeRecord& Probes::getProbe(const std::string& ID) const
{
  ProbeListIterator iter = m_Probes.find(ID);
  if (iter!=m_Probes.end())
  {
    return *iter;
  }
  std::cout << "No probe item with ID \""<<ID<<"\" is present.\n";
  throw 42;
}

ProbeListIterator Probes::getBegin() const
{
  return m_Probes.begin();
}

ProbeListIterator Probes::getEnd() const
{
  return m_Probes.end();
}

bool Probes::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Probes::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  ProbeListIterator iter = m_Probes.begin();
  const ProbeListIterator end_iter = m_Probes.end();
  while (iter!=end_iter)
  {
    if (!iter->saveToStream(output))
    {
      std::cout << "Probes::saveAllToStream: Error while writing record for \""
                << iter->ProbeID <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Probes::clearAll()
{
  m_Probes.clear();
}

int Probes::readRecordPROB(std::ifstream& in_File)
{
  ProbeRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Probes::readRecordPROB: Error while reading probe record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasProbe(temp.ProbeID))
  {
    if (getProbe(temp.ProbeID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if probe present
  addProbe(temp);
  return 1;
} //readRecordPROB
