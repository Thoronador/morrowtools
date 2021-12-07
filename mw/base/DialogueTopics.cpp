/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2014, 2021  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "DialogueTopics.hpp"
#include <iostream>
#include <stdexcept>

namespace MWTP
{

DialogueTopics::DialogueTopics()
{
}

DialogueTopics& DialogueTopics::get()
{
  static DialogueTopics Instance;
  return Instance;
}

void DialogueTopics::addDialogueTopic(const DialogueTopicRecord& record)
{
  if (!record.DialogueID.empty())
  {
    m_DialogueTopics[record.DialogueID] = record;
  }
}

bool DialogueTopics::hasDialogueTopic(const std::string& ID) const
{
  return (m_DialogueTopics.find(ID)!=m_DialogueTopics.end());
}

unsigned int DialogueTopics::getNumberOfDialogueTopics() const
{
  return m_DialogueTopics.size();
}

const DialogueTopicRecord& DialogueTopics::getDialogueTopic(const std::string& ID) const
{
  std::map<std::string, DialogueTopicRecord>::const_iterator iter = m_DialogueTopics.find(ID);
  if (iter!=m_DialogueTopics.end())
  {
    return iter->second;
  }
  std::cout << "No dialogue topic with the ID \""<<ID<<"\" is present.\n";
  throw std::runtime_error("DialogueTopics::getDialogueTopic(): No dialogue topic with the ID \""+ID+"\" is present.");
}

DialogueTopicListIterator DialogueTopics::begin() const
{
  return m_DialogueTopics.begin();
}

DialogueTopicListIterator DialogueTopics::end() const
{
  return m_DialogueTopics.end();
}

bool DialogueTopics::saveAllToStream(std::ostream& output) const
{
  if (!output.good())
  {
    std::cout << "DialogueTopics::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  DialogueTopicListIterator iter = m_DialogueTopics.begin();
  const DialogueTopicListIterator end_iter = m_DialogueTopics.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "DialogueTopics::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void DialogueTopics::clear()
{
  m_DialogueTopics.clear();
}

int DialogueTopics::readRecordDIAL(std::istream& input)
{
  DialogueTopicRecord temp;
  if (!temp.loadFromStream(input))
  {
    std::cout << "DialogueTopics::readRecordDIAL: Error while reading dialogue "
              << "topic record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasDialogueTopic(temp.DialogueID))
  {
    if (getDialogueTopic(temp.DialogueID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if topic present
  addDialogueTopic(temp);
  return 1;
} //readRecordDIAL

} //namespace
