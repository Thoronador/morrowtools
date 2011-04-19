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

#include "DialogueInfos.h"
#include <iostream>

DialogueInfos::DialogueInfos()
{
  //empty
}

DialogueInfos::~DialogueInfos()
{
  //empty
}

DialogueInfos& DialogueInfos::getSingleton()
{
  static DialogueInfos Instance;
  return Instance;
}

void DialogueInfos::addDialogueInfo(const DialogueInfoRecord& record)
{
  if (record.InfoID!="")
  {
    m_DialogueInfos[record.InfoID] = record;
  }
}

bool DialogueInfos::hasDialogueInfo(const std::string& ID) const
{
  return (m_DialogueInfos.find(ID)!=m_DialogueInfos.end());
}

unsigned int DialogueInfos::getNumberOfDialogueInfos() const
{
  return m_DialogueInfos.size();
}

const DialogueInfoRecord& DialogueInfos::getDialogueInfo(const std::string& ID) const
{
  std::map<std::string, DialogueInfoRecord>::const_iterator iter = m_DialogueInfos.find(ID);
  if (iter!=m_DialogueInfos.end())
  {
    return iter->second;
  }
  std::cout << "No dialogue info with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

DialogueInfoListIterator DialogueInfos::getBegin() const
{
  return m_DialogueInfos.begin();
}

DialogueInfoListIterator DialogueInfos::getEnd() const
{
  return m_DialogueInfos.end();
}

bool DialogueInfos::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "DialogueInfos::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  DialogueInfoListIterator iter = m_DialogueInfos.begin();
  const DialogueInfoListIterator end_iter = m_DialogueInfos.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "DialogueInfos::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void DialogueInfos::clearAll()
{
  m_DialogueInfos.clear();
}

int DialogueInfos::readRecordINFO(std::ifstream& in_File)
{
  DialogueInfoRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "DialogueInfos::readRecordINFO: Error while reading dialogue "
              << "info record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasDialogueInfo(temp.InfoID))
  {
    if (getDialogueInfo(temp.InfoID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if info present
  addDialogueInfo(temp);
  return 1;
} //readRecordINFO
