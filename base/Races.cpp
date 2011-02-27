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

#include "Races.h"
#include <iostream>

Races::Races()
{
  //empty
}

Races::~Races()
{
  //empty
}

Races& Races::getSingleton()
{
  static Races Instance;
  return Instance;
}

void Races::addRace(const RaceRecord& record)
{
  if (record.RaceID!="")
  {
    m_Races.insert(record);
  }
}

bool Races::hasRace(const std::string& ID) const
{
  return (m_Races.find(ID)!=m_Races.end());
}

unsigned int Races::getNumberOfRaces() const
{
  return m_Races.size();
}

const RaceRecord& Races::getRace(const std::string& ID) const
{
  RaceListIterator iter = m_Races.find(ID);
  if (iter!=m_Races.end())
  {
    return *iter;
  }
  std::cout << "No race with ID \""<<ID<<"\" is present.\n";
  throw 42;
}

RaceListIterator Races::getBegin() const
{
  return m_Races.begin();
}

RaceListIterator Races::getEnd() const
{
  return m_Races.end();
}

bool Races::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Races::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  RaceListIterator iter = m_Races.begin();
  const RaceListIterator end_iter = m_Races.end();
  while (iter!=end_iter)
  {
    if (!iter->saveToStream(output))
    {
      std::cout << "Races::saveAllToStream: Error while writing record for \""
                << iter->RaceID <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Races::clearAll()
{
  m_Races.clear();
}

int Races::readRecordRACE(std::ifstream& in_File)
{
  RaceRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Races::readRecordRACE: Error while reading race record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasRace(temp.RaceID))
  {
    if (getRace(temp.RaceID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if race present
  addRace(temp);
  return 1;
} //readRecordRACE
