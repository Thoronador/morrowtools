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

#include "WordsOfPower.h"
#include <iostream>

namespace SRTP
{

WordsOfPower::WordsOfPower()
{
  //empty
}

WordsOfPower::~WordsOfPower()
{
  //empty
}

WordsOfPower& WordsOfPower::getSingleton()
{
  static WordsOfPower Instance;
  return Instance;
}

void WordsOfPower::addWordOfPower(const WordOfPowerRecord& record)
{
  if (record.editorID!="")
  {
    m_WordsOfPower[record.editorID] = record;
  }
}

bool WordsOfPower::hasWordOfPower(const std::string& ID) const
{
  return (m_WordsOfPower.find(ID)!=m_WordsOfPower.end());
}

unsigned int WordsOfPower::getNumberOfWordsOfPower() const
{
  return m_WordsOfPower.size();
}

const WordOfPowerRecord& WordsOfPower::getWordOfPower(const std::string& ID) const
{
  std::map<std::string, WordOfPowerRecord>::const_iterator iter = m_WordsOfPower.find(ID);
  if (iter!=m_WordsOfPower.end())
  {
    return iter->second;
  }
  std::cout << "No word of power with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

WordOfPowerListIterator WordsOfPower::getBegin() const
{
  return m_WordsOfPower.begin();
}

WordOfPowerListIterator WordsOfPower::getEnd() const
{
  return m_WordsOfPower.end();
}

bool WordsOfPower::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "WordsOfPower::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  WordOfPowerListIterator iter = m_WordsOfPower.begin();
  const WordOfPowerListIterator end_iter = m_WordsOfPower.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "WordsOfPower::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void WordsOfPower::clearAll()
{
  m_WordsOfPower.clear();
}

int WordsOfPower::readNextRecord(std::ifstream& in_File)
{
  WordOfPowerRecord temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "WordsOfPower::readNextRecord: Error while reading word of power record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasWordOfPower(temp.editorID))
  {
    if (getWordOfPower(temp.editorID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if word of power present
  addWordOfPower(temp);
  return 1;
} //readNextRecord

} //namespace
