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

#include "Keywords.h"
#include <iostream>

namespace SRTP
{

Keywords::Keywords()
{
  //empty
}

Keywords::~Keywords()
{
  //empty
}

Keywords& Keywords::getSingleton()
{
  static Keywords Instance;
  return Instance;
}

void Keywords::addKeyword(const KeywordRecord& record)
{
  if (record.editorID!="")
  {
    m_Keywords[record.editorID] = record;
  }
}

bool Keywords::hasKeyword(const std::string& ID) const
{
  return (m_Keywords.find(ID)!=m_Keywords.end());
}

unsigned int Keywords::getNumberOfKeywords() const
{
  return m_Keywords.size();
}

const KeywordRecord& Keywords::getKeyword(const std::string& ID) const
{
  std::map<std::string, KeywordRecord>::const_iterator iter = m_Keywords.find(ID);
  if (iter!=m_Keywords.end())
  {
    return iter->second;
  }
  std::cout << "No keyword with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

KeywordListIterator Keywords::getBegin() const
{
  return m_Keywords.begin();
}

KeywordListIterator Keywords::getEnd() const
{
  return m_Keywords.end();
}

bool Keywords::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Keywords::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  KeywordListIterator iter = m_Keywords.begin();
  const KeywordListIterator end_iter = m_Keywords.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Keywords::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Keywords::clearAll()
{
  m_Keywords.clear();
}

int Keywords::readRecordKYWD(std::ifstream& in_File)
{
  KeywordRecord temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "Keywords::readRecordKYWD: Error while reading keyword record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasKeyword(temp.editorID))
  {
    if (getKeyword(temp.editorID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if keyword present
  addKeyword(temp);
  return 1;
} //readRecordKYWD

int Keywords::readNextRecord(std::ifstream& in_File)
{
  return readRecordKYWD(in_File);
}

} //namespace
