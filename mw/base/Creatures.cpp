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

#include "Creatures.h"
#include <iostream>

namespace MWTP
{

Creatures::Creatures()
{
  //empty
}

Creatures::~Creatures()
{
  //empty
}

Creatures& Creatures::getSingleton()
{
  static Creatures Instance;
  return Instance;
}

void Creatures::addCreature(const CreatureRecord& record)
{
  if (record.CreatureID!="")
  {
    m_Creatures[record.CreatureID] = record;
  }
}

bool Creatures::hasCreature(const std::string& ID) const
{
  return (m_Creatures.find(ID)!=m_Creatures.end());
}

unsigned int Creatures::getNumberOfCreatures() const
{
  return m_Creatures.size();
}

const CreatureRecord& Creatures::getCreature(const std::string& ID) const
{
  std::map<std::string, CreatureRecord>::const_iterator iter = m_Creatures.find(ID);
  if (iter!=m_Creatures.end())
  {
    return iter->second;
  }
  std::cout << "No creature with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

CreatureListIterator Creatures::getBegin() const
{
  return m_Creatures.begin();
}

CreatureListIterator Creatures::getEnd() const
{
  return m_Creatures.end();
}

bool Creatures::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Creatures::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  CreatureListIterator iter = m_Creatures.begin();
  const CreatureListIterator end_iter = m_Creatures.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Creatures::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Creatures::clearAll()
{
  m_Creatures.clear();
}

int Creatures::readRecordCREA(std::ifstream& in_File)
{
  CreatureRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Creatures::readRecordCREA: Error while reading creature "
              << "record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasCreature(temp.CreatureID))
  {
    if (getCreature(temp.CreatureID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if creature present
  addCreature(temp);
  return 1;
} //readRecordCREA

} //namespace
