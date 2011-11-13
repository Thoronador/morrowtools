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

#include "BirthSigns.h"
#include <iostream>

namespace MWTP
{

BirthSigns::BirthSigns()
{
  //empty
}

BirthSigns::~BirthSigns()
{
  //empty
}

BirthSigns& BirthSigns::getSingleton()
{
  static BirthSigns Instance;
  return Instance;
}

void BirthSigns::addBirthSign(const BirthSignRecord& record)
{
  if (record.BirthSignID!="")
  {
    m_BirthSigns.insert(record);
  }
}

bool BirthSigns::hasBirthSign(const std::string& ID) const
{
  return (m_BirthSigns.find(ID)!=m_BirthSigns.end());
}

unsigned int BirthSigns::getNumberOfBirthSigns() const
{
  return m_BirthSigns.size();
}

const BirthSignRecord& BirthSigns::getBirthSign(const std::string& ID) const
{
  std::set<BirthSignRecord>::const_iterator iter = m_BirthSigns.find(ID);
  if (iter!=m_BirthSigns.end())
  {
    return *iter;
  }
  std::cout << "No birth sign with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

BirthSignListIterator BirthSigns::getBegin() const
{
  return m_BirthSigns.begin();
}

BirthSignListIterator BirthSigns::getEnd() const
{
  return m_BirthSigns.end();
}

bool BirthSigns::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "BirthSigns::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  BirthSignListIterator iter = m_BirthSigns.begin();
  const BirthSignListIterator end_iter = m_BirthSigns.end();
  while (iter!=end_iter)
  {
    if (!iter->saveToStream(output))
    {
      std::cout << "BirthSigns::saveAllToStream: Error while writing record for \""
                << iter->BirthSignID <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void BirthSigns::clearAll()
{
  m_BirthSigns.clear();
}

int BirthSigns::readRecordBSGN(std::ifstream& in_File)
{
  BirthSignRecord temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "readRecordBSGN: Error while reading birth sign record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasBirthSign(temp.BirthSignID))
  {
    if (getBirthSign(temp.BirthSignID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if birth sign present
  addBirthSign(temp);
  return 1;
} //readRecordBSGN

} //namespace
