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

#include "Containers.h"
#include <iostream>

Containers::Containers()
{
  //empty
}

Containers::~Containers()
{
  //empty
}

Containers& Containers::getSingleton()
{
  static Containers Instance;
  return Instance;
}

void Containers::addContainer(const ContainerRecord& record)
{
  if (record.ContainerID!="")
  {
    m_Containers[record.ContainerID] = record;
  }
}

bool Containers::hasContainer(const std::string& ID) const
{
  return (m_Containers.find(ID)!=m_Containers.end());
}

unsigned int Containers::getNumberOfContainers() const
{
  return m_Containers.size();
}

const ContainerRecord& Containers::getContainer(const std::string& ID) const
{
  std::map<std::string, ContainerRecord>::const_iterator iter = m_Containers.find(ID);
  if (iter!=m_Containers.end())
  {
    return iter->second;
  }
  std::cout << "No container with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

ContainerListIterator Containers::getBegin() const
{
  return m_Containers.begin();
}

ContainerListIterator Containers::getEnd() const
{
  return m_Containers.end();
}

bool Containers::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Containers::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  ContainerListIterator iter = m_Containers.begin();
  const ContainerListIterator end_iter = m_Containers.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Containers::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Containers::clearAll()
{
  m_Containers.clear();
}

int Containers::readRecordCONT(std::ifstream& in_File)
{
  ContainerRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Containers::readRecordCONT: Error while reading container "
              << "record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasContainer(temp.ContainerID))
  {
    if (getContainer(temp.ContainerID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if container present
  addContainer(temp);
  return 1;
} //readRecordCONT
