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

#include "NPCs.h"
#include <iostream>

namespace MWTP
{

NPCs::NPCs()
{
  //empty
}

NPCs::~NPCs()
{
  //empty
}

NPCs& NPCs::getSingleton()
{
  static NPCs Instance;
  return Instance;
}

void NPCs::addNPC(const NPCRecord& record)
{
  if (record.NPCID!="")
  {
    m_NPCs[record.NPCID] = record;
  }
}

bool NPCs::hasNPC(const std::string& ID) const
{
  return (m_NPCs.find(ID)!=m_NPCs.end());
}

unsigned int NPCs::getNumberOfNPCs() const
{
  return m_NPCs.size();
}

const NPCRecord& NPCs::getNPC(const std::string& ID) const
{
  std::map<std::string, NPCRecord>::const_iterator iter = m_NPCs.find(ID);
  if (iter!=m_NPCs.end())
  {
    return iter->second;
  }
  std::cout << "No NPC with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

NPCListIterator NPCs::getBegin() const
{
  return m_NPCs.begin();
}

NPCListIterator NPCs::getEnd() const
{
  return m_NPCs.end();
}

bool NPCs::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "NPCs::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  NPCListIterator iter = m_NPCs.begin();
  const NPCListIterator end_iter = m_NPCs.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "NPCs::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void NPCs::clearAll()
{
  m_NPCs.clear();
}

int NPCs::readRecordNPC_(std::ifstream& in_File)
{
  NPCRecord temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "readRecordNPC_: Error while reading NPC record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasNPC(temp.NPCID))
  {
    if (getNPC(temp.NPCID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if NPC present
  addNPC(temp);
  return 1;
} //readRecordNPC_

} //namespace
