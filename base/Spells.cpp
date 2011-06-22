/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

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

#include "Spells.h"
#include <iostream>

namespace MWTP
{

const size_t Spells::cMaximumSpellNameLength = 31;

/* **** Spells functions **** */

Spells::Spells()
{
  //empty
}

Spells::~Spells()
{
  m_Spells.clear();
}

Spells& Spells::getSingleton()
{
  static Spells Instance;
  return Instance;
}

void Spells::addSpell(const SpellRecord& record)
{
  if (record.SpellID!="")
  {
    m_Spells[record.SpellID] = record;
    //truncate names that are too long (more than 31 characters) in order to avoid errors
    if (record.Name.length()>cMaximumSpellNameLength)
    {
      m_Spells[record.SpellID].Name = record.Name.substr(0, cMaximumSpellNameLength);
    }//if (inner)
  }
}

unsigned int Spells::getNumberOfSpells() const
{
  return m_Spells.size();
}

bool Spells::hasSpell(const std::string& ID) const
{
  return m_Spells.find(ID)!=m_Spells.end();
}

const SpellRecord& Spells::getSpell(const std::string& ID) const
{
  std::map<std::string, SpellRecord>::const_iterator iter;
  iter = m_Spells.find(ID);
  if (iter!=m_Spells.end())
  {
    return iter->second;
  }
  //nothing found
  std::cout << "Error: No spell with ID \""<<ID<<"\" found!\n";
  throw 2;
}

int Spells::readRecordSPEL(std::ifstream& in_File)
{
  SpellRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Error while reading spell record from stream.\n";
    return -1;
  }
  //check for presence of spell
  if (hasSpell(temp.SpellID))
  {
    if (getSpell(temp.SpellID).equals(temp))
    {
      return 0; //return zero and do nothing, spell record is the same
    }
  }//if
  //add spell and return 1, since one record was added/changed
  addSpell(temp);
  return 1;
}//readRecordSPEL (integer version of readSPEL()

SpellListIterator Spells::getBegin() const
{
  return m_Spells.begin();
}

SpellListIterator Spells::getEnd() const
{
  return m_Spells.end();
}

bool Spells::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Spells::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  SpellListIterator iter = m_Spells.begin();
  const SpellListIterator end_iter = m_Spells.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Spells::saveAllToStream: Error while writing record.\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

bool Spells::removeSpell(const std::string& ID)
{
  const std::map<std::string, SpellRecord>::iterator iter = m_Spells.find(ID);
  if (iter!=m_Spells.end())
  {
    m_Spells.erase(iter);
    return true;
  }
  return false;
}

void Spells::clearAll()
{
  m_Spells.clear();
}

} //namespace
