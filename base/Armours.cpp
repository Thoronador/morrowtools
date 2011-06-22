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

#include "Armours.h"
#include <iostream>

namespace MWTP
{

Armours::Armours()
{
  //empty
}

Armours::~Armours()
{
  //empty
}

Armours& Armours::getSingleton()
{
  static Armours Instance;
  return Instance;
}

void Armours::addArmour(const ArmourRecord& record)
{
  if (record.ArmourID!="")
  {
    m_Armours[record.ArmourID] = record;
  }
}

bool Armours::hasArmour(const std::string& ID) const
{
  return (m_Armours.find(ID)!=m_Armours.end());
}

unsigned int Armours::getNumberOfArmours() const
{
  return m_Armours.size();
}

const ArmourRecord& Armours::getArmour(const std::string& ID) const
{
  std::map<std::string, ArmourRecord>::const_iterator iter = m_Armours.find(ID);
  if (iter!=m_Armours.end())
  {
    return iter->second;
  }
  std::cout << "No armour with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

ArmourListIterator Armours::getBegin() const
{
  return m_Armours.begin();
}

ArmourListIterator Armours::getEnd() const
{
  return m_Armours.end();
}

bool Armours::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Armours::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  ArmourListIterator iter = m_Armours.begin();
  const ArmourListIterator end_iter = m_Armours.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Armours::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Armours::clearAll()
{
  m_Armours.clear();
}

int Armours::readRecordARMO(std::ifstream& in_File)
{
  ArmourRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Armours::readRecordARMO: Error while reading armour "
              << "record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasArmour(temp.ArmourID))
  {
    if (getArmour(temp.ArmourID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if armour present
  addArmour(temp);
  return 1;
} //readRecordARMO

} //namespace
