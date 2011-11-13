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

#include "Regions.h"
#include <iostream>

namespace MWTP
{

Regions::Regions()
{
  //empty
}

Regions::~Regions()
{
  //empty
}

Regions& Regions::getSingleton()
{
  static Regions Instance;
  return Instance;
}

void Regions::addRegion(const RegionRecord& record)
{
  if (record.RegionID!="")
  {
    m_Regions.insert(record);
  }
}

bool Regions::hasRegion(const std::string& ID) const
{
  return (m_Regions.find(ID)!=m_Regions.end());
}

unsigned int Regions::getNumberOfRegions() const
{
  return m_Regions.size();
}

const RegionRecord& Regions::getRegion(const std::string& ID) const
{
  RegionListIterator iter = m_Regions.find(ID);
  if (iter!=m_Regions.end())
  {
    return *iter;
  }
  std::cout << "No region with ID \""<<ID<<"\" is present.\n";
  throw 42;
}

RegionListIterator Regions::getBegin() const
{
  return m_Regions.begin();
}

RegionListIterator Regions::getEnd() const
{
  return m_Regions.end();
}

bool Regions::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Regions::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  RegionListIterator iter = m_Regions.begin();
  const RegionListIterator end_iter = m_Regions.end();
  while (iter!=end_iter)
  {
    if (!iter->saveToStream(output))
    {
      std::cout << "Regions::saveAllToStream: Error while writing record for \""
                << iter->RegionID <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Regions::clearAll()
{
  m_Regions.clear();
}

int Regions::readRecordREGN(std::ifstream& in_File)
{
  RegionRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Regions::readRecordREGN: Error while reading region "
              << "record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasRegion(temp.RegionID))
  {
    if (getRegion(temp.RegionID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if region present
  addRegion(temp);
  return 1;
} //readRecordREGN

} //namespace
