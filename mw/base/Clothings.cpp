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

#include "Clothings.h"
#include <iostream>

namespace MWTP
{

Clothings::Clothings()
{
  //empty
}

Clothings::~Clothings()
{
  //empty
}

Clothings& Clothings::getSingleton()
{
  static Clothings Instance;
  return Instance;
}

void Clothings::addClothing(const ClothingRecord& record)
{
  if (record.ClothingID!="")
  {
    m_Clothings[record.ClothingID] = record;
  }
}

bool Clothings::hasClothing(const std::string& ID) const
{
  return (m_Clothings.find(ID)!=m_Clothings.end());
}

unsigned int Clothings::getNumberOfClothings() const
{
  return m_Clothings.size();
}

const ClothingRecord& Clothings::getClothing(const std::string& ID) const
{
  std::map<std::string, ClothingRecord>::const_iterator iter = m_Clothings.find(ID);
  if (iter!=m_Clothings.end())
  {
    return iter->second;
  }
  std::cout << "No clothing with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

ClothingListIterator Clothings::getBegin() const
{
  return m_Clothings.begin();
}

ClothingListIterator Clothings::getEnd() const
{
  return m_Clothings.end();
}

bool Clothings::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Clothings::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  ClothingListIterator iter = m_Clothings.begin();
  const ClothingListIterator end_iter = m_Clothings.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Clothings::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Clothings::clearAll()
{
  m_Clothings.clear();
}

int Clothings::readRecordCLOT(std::ifstream& in_File)
{
  ClothingRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Clothings::readRecordCLOT: Error while reading clothing "
              << "record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasClothing(temp.ClothingID))
  {
    if (getClothing(temp.ClothingID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if clothing present
  addClothing(temp);
  return 1;
} //readRecordCLOT

} //namespace MWTP
