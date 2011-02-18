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

#include "Apparatuses.h"
#include <iostream>

Apparatuses::Apparatuses()
{
  //empty
}

Apparatuses::~Apparatuses()
{
  //empty
}

Apparatuses& Apparatuses::getSingleton()
{
  static Apparatuses Instance;
  return Instance;
}

void Apparatuses::addApparatus(const ApparatusRecord& record)
{
  if (record.ApparatusID!="")
  {
    m_Apparatuses[record.ApparatusID] = record;
  }
}

bool Apparatuses::hasApparatus(const std::string& ID) const
{
  return (m_Apparatuses.find(ID)!=m_Apparatuses.end());
}

unsigned int Apparatuses::getNumberOfApparatuses() const
{
  return m_Apparatuses.size();
}

const ApparatusRecord& Apparatuses::getApparatus(const std::string& ID) const
{
  std::map<std::string, ApparatusRecord>::const_iterator iter = m_Apparatuses.find(ID);
  if (iter!=m_Apparatuses.end())
  {
    return iter->second;
  }
  std::cout << "No apparatus with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

ApparatusListIterator Apparatuses::getBegin() const
{
  return m_Apparatuses.begin();
}

ApparatusListIterator Apparatuses::getEnd() const
{
  return m_Apparatuses.end();
}

bool Apparatuses::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Apparatuses::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  ApparatusListIterator iter = m_Apparatuses.begin();
  const ApparatusListIterator end_iter = m_Apparatuses.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Apparatusess::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Apparatuses::clearAll()
{
  m_Apparatuses.clear();
}

int Apparatuses::readRecordAPPA(std::ifstream& in_File)
{
  ApparatusRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Error while reading apparatus record!\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasApparatus(temp.ApparatusID))
  {
    if (getApparatus(temp.ApparatusID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if apparatus present
  addApparatus(temp);
  return 1;
} //readRecordAPPA
