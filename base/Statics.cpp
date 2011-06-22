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

#include "Statics.h"
#include <iostream>

namespace MWTP
{

Statics::Statics()
{
  //empty
}

Statics::~Statics()
{
  //empty
}

Statics& Statics::getSingleton()
{
  static Statics Instance;
  return Instance;
}

void Statics::addStatic(const StaticRecord& record)
{
  if (record.StaticID!="")
  {
    m_Statics[record.StaticID] = record;
  }
}

bool Statics::hasStatic(const std::string& ID) const
{
  return (m_Statics.find(ID)!=m_Statics.end());
}

unsigned int Statics::getNumberOfStatics() const
{
  return m_Statics.size();
}

const StaticRecord& Statics::getStatic(const std::string& ID) const
{
  std::map<std::string, StaticRecord>::const_iterator iter = m_Statics.find(ID);
  if (iter!=m_Statics.end())
  {
    return iter->second;
  }
  std::cout << "No static with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

StaticListIterator Statics::getBegin() const
{
  return m_Statics.begin();
}

StaticListIterator Statics::getEnd() const
{
  return m_Statics.end();
}

bool Statics::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Statics::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  StaticListIterator iter = m_Statics.begin();
  const StaticListIterator end_iter = m_Statics.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Statics::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Statics::clearAll()
{
  m_Statics.clear();
}

int Statics::readRecordSTAT(std::ifstream& in_File)
{
  StaticRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "readRecordSTAT: Error while reading record!\n";
    return -1;
  }//if

  //now add static to the list
  if (hasStatic(temp.StaticID))
  {
    if (getStatic(temp.StaticID).equals(temp))
    {
      //same static already present - add nothing and return zero
      return 0;
    }
  }
  addStatic(temp);
  return 1;
} //readRecordSTAT

} //namespace
