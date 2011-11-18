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

#include "Actions.h"
#include <iostream>

namespace SRTP
{

Actions::Actions()
{
  //empty
}

Actions::~Actions()
{
  //empty
}

Actions& Actions::getSingleton()
{
  static Actions Instance;
  return Instance;
}

void Actions::addAction(const ActionRecord& record)
{
  if (record.editorID!="")
  {
    m_Actions[record.editorID] = record;
  }
}

bool Actions::hasAction(const std::string& ID) const
{
  return (m_Actions.find(ID)!=m_Actions.end());
}

unsigned int Actions::getNumberOfActions() const
{
  return m_Actions.size();
}

const ActionRecord& Actions::getAction(const std::string& ID) const
{
  std::map<std::string, ActionRecord>::const_iterator iter = m_Actions.find(ID);
  if (iter!=m_Actions.end())
  {
    return iter->second;
  }
  std::cout << "No action with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

ActionListIterator Actions::getBegin() const
{
  return m_Actions.begin();
}

ActionListIterator Actions::getEnd() const
{
  return m_Actions.end();
}

bool Actions::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Actions::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  ActionListIterator iter = m_Actions.begin();
  const ActionListIterator end_iter = m_Actions.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Actions::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Actions::clearAll()
{
  m_Actions.clear();
}

int Actions::readNextRecord(std::ifstream& in_File)
{
  ActionRecord temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "Actions::readNextRecord: Error while reading action record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasAction(temp.editorID))
  {
    if (getAction(temp.editorID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if action present
  addAction(temp);
  return 1;
} //readNextRecord

} //namespace
