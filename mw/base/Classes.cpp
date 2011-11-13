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

#include "Classes.h"
#include <iostream>

namespace MWTP
{

Classes::Classes()
{
  //empty
}

Classes::~Classes()
{
  //empty
}

Classes& Classes::getSingleton()
{
  static Classes Instance;
  return Instance;
}

void Classes::addClass(const ClassRecord& record)
{
  if (record.ClassID!="")
  {
    m_Classes[record.ClassID] = record;
  }
}

bool Classes::hasClass(const std::string& ID) const
{
  return (m_Classes.find(ID)!=m_Classes.end());
}

unsigned int Classes::getNumberOfClasses() const
{
  return m_Classes.size();
}

const ClassRecord& Classes::getClass(const std::string& ID) const
{
  std::map<std::string, ClassRecord>::const_iterator iter = m_Classes.find(ID);
  if (iter!=m_Classes.end())
  {
    return iter->second;
  }
  std::cout << "No class with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

ClassListIterator Classes::getBegin() const
{
  return m_Classes.begin();
}

ClassListIterator Classes::getEnd() const
{
  return m_Classes.end();
}

bool Classes::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Classes::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  ClassListIterator iter = m_Classes.begin();
  const ClassListIterator end_iter = m_Classes.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Classes::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Classes::clearAll()
{
  m_Classes.clear();
}

int Classes::readRecordCLAS(std::ifstream& in_File)
{
  ClassRecord temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "readRecordCLAS: Error while reading class record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasClass(temp.ClassID))
  {
    if (getClass(temp.ClassID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if class present
  addClass(temp);
  return 1;
} //readRecordCLAS

} //namespace
