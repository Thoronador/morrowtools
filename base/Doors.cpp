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

#include "Doors.h"
#include <iostream>

namespace MWTP
{

Doors::Doors()
{
  //empty
}

Doors::~Doors()
{
  //empty
}

Doors& Doors::getSingleton()
{
  static Doors Instance;
  return Instance;
}

void Doors::addDoor(const DoorRecord& record)
{
  if (record.DoorID!="")
  {
    m_Doors[record.DoorID] = record;
  }
}

bool Doors::hasDoor(const std::string& ID) const
{
  return (m_Doors.find(ID)!=m_Doors.end());
}

unsigned int Doors::getNumberOfDoors() const
{
  return m_Doors.size();
}

const DoorRecord& Doors::getDoor(const std::string& ID) const
{
  std::map<std::string, DoorRecord>::const_iterator iter = m_Doors.find(ID);
  if (iter!=m_Doors.end())
  {
    return iter->second;
  }
  std::cout << "No door with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

DoorListIterator Doors::getBegin() const
{
  return m_Doors.begin();
}

DoorListIterator Doors::getEnd() const
{
  return m_Doors.end();
}

bool Doors::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Doors::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  DoorListIterator iter = m_Doors.begin();
  const DoorListIterator end_iter = m_Doors.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Doors::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Doors::clearAll()
{
  m_Doors.clear();
}

int Doors::readRecordDOOR(std::ifstream& in_File)
{
  DoorRecord temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "readRecordDOOR: Error while reading door record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasDoor(temp.DoorID))
  {
    if (getDoor(temp.DoorID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if door present
  addDoor(temp);
  return 1;
} //readRecordDOOR

} //namespace
