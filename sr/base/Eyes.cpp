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

#include "Eyes.h"
#include <iostream>

namespace SRTP
{

Eyes::Eyes()
{
  //empty
}

Eyes::~Eyes()
{
  //empty
}

Eyes& Eyes::getSingleton()
{
  static Eyes Instance;
  return Instance;
}

void Eyes::addEye(const EyeRecord& record)
{
  if (record.editorID!="")
  {
    m_Eyes[record.editorID] = record;
  }
}

bool Eyes::hasEye(const std::string& ID) const
{
  return (m_Eyes.find(ID)!=m_Eyes.end());
}

unsigned int Eyes::getNumberOfEyes() const
{
  return m_Eyes.size();
}

const EyeRecord& Eyes::getEye(const std::string& ID) const
{
  std::map<std::string, EyeRecord>::const_iterator iter = m_Eyes.find(ID);
  if (iter!=m_Eyes.end())
  {
    return iter->second;
  }
  std::cout << "No eye with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

EyeListIterator Eyes::getBegin() const
{
  return m_Eyes.begin();
}

EyeListIterator Eyes::getEnd() const
{
  return m_Eyes.end();
}

bool Eyes::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Eyes::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  EyeListIterator iter = m_Eyes.begin();
  const EyeListIterator end_iter = m_Eyes.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Eyes::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Eyes::clearAll()
{
  m_Eyes.clear();
}

int Eyes::readNextRecord(std::ifstream& in_File)
{
  EyeRecord temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "Eyes::readNextRecord: Error while reading eye record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasEye(temp.editorID))
  {
    if (getEye(temp.editorID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if eye present
  addEye(temp);
  return 1;
} //readNextRecord

} //namespace
