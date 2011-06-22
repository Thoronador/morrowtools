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

#include "Factions.h"
#include <iostream>

namespace MWTP
{

Factions::Factions()
{
  //empty
}

Factions::~Factions()
{
  //empty
}

Factions& Factions::getSingleton()
{
  static Factions Instance;
  return Instance;
}

void Factions::addFaction(const FactionRecord& record)
{
  if (record.FactionID!="")
  {
    m_Factions[record.FactionID] = record;
  }
}

bool Factions::hasFaction(const std::string& ID) const
{
  return (m_Factions.find(ID)!=m_Factions.end());
}

unsigned int Factions::getNumberOfFactions() const
{
  return m_Factions.size();
}

const FactionRecord& Factions::getFaction(const std::string& ID) const
{
  std::map<std::string, FactionRecord>::const_iterator iter = m_Factions.find(ID);
  if (iter!=m_Factions.end())
  {
    return iter->second;
  }
  std::cout << "No faction with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

FactionListIterator Factions::getBegin() const
{
  return m_Factions.begin();
}

FactionListIterator Factions::getEnd() const
{
  return m_Factions.end();
}

bool Factions::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Factions::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  FactionListIterator iter = m_Factions.begin();
  const FactionListIterator end_iter = m_Factions.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Factions::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Factions::clearAll()
{
  m_Factions.clear();
}

int Factions::readRecordFACT(std::ifstream& in_File)
{
  FactionRecord temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "readRecordFACT: Error while reading faction record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasFaction(temp.FactionID))
  {
    if (getFaction(temp.FactionID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if faction present
  addFaction(temp);
  return 1;
} //readRecordFACT

} //namespace
