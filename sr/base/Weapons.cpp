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

#include "Weapons.h"
#include <iostream>

namespace SRTP
{

Weapons::Weapons()
{
  //empty
}

Weapons::~Weapons()
{
  //empty
}

Weapons& Weapons::getSingleton()
{
  static Weapons Instance;
  return Instance;
}

void Weapons::addWeapon(const WeaponRecord& record)
{
  if (record.editorID!="")
  {
    m_Weapons[record.editorID] = record;
  }
}

bool Weapons::hasWeapon(const std::string& ID) const
{
  return (m_Weapons.find(ID)!=m_Weapons.end());
}

unsigned int Weapons::getNumberOfWeapons() const
{
  return m_Weapons.size();
}

const WeaponRecord& Weapons::getWeapon(const std::string& ID) const
{
  std::map<std::string, WeaponRecord>::const_iterator iter = m_Weapons.find(ID);
  if (iter!=m_Weapons.end())
  {
    return iter->second;
  }
  std::cout << "No weapon with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

WeaponListIterator Weapons::getBegin() const
{
  return m_Weapons.begin();
}

WeaponListIterator Weapons::getEnd() const
{
  return m_Weapons.end();
}

bool Weapons::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Weapons::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  WeaponListIterator iter = m_Weapons.begin();
  const WeaponListIterator end_iter = m_Weapons.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Weapons::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Weapons::clearAll()
{
  m_Weapons.clear();
}

int Weapons::readRecordWEAP(std::ifstream& in_File)
{
  WeaponRecord temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "Weapons::readRecordWEAP: Error while reading weapon record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasWeapon(temp.editorID))
  {
    if (getWeapon(temp.editorID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if weapon present
  addWeapon(temp);
  return 1;
} //readRecordWEAP

} //namespace
