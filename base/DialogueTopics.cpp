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

#include "DialogueTopics.h"
#include <iostream>

DialogueTopics::DialogueTopics()
{
  //empty
}

DialogueTopics::~DialogueTopics()
{
  //empty
}

DialogueTopics& DialogueTopics::getSingleton()
{
  static DialogueTopics Instance;
  return Instance;
}

void DialogueTopics::addDialogueTopic(const DialogueTopicRecord& record)
{
  if (record.DialogueID!="")
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
  throw 42;
}

DialogueTopicListIterator DialogueTopics::getBegin() const
{
  return m_DialogueTopics.begin();
}

DialogueTopicListIterator DialogueTopics::getEnd() const
{
  return m_DialogueTopics.end();
}

bool DialogueTopics::saveAllToStream(std::ofstream& output) const
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

void DialogueTopics::clearAll()
{
  m_DialogueTopics.clear();
}

int DialogueTopics::readRecordDIAL(std::ifstream& in_File)
{
  DialogueTopicRecord temp;
  if (!temp.loadFromStream(in_File))
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
