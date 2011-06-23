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

#include "Sounds.h"
#include <iostream>

namespace MWTP
{

Sounds::Sounds()
{
  //empty
}

Sounds::~Sounds()
{
  //empty
}

Sounds& Sounds::getSingleton()
{
  static Sounds Instance;
  return Instance;
}

void Sounds::addSound(const SoundRecord& record)
{
  if (record.SoundID!="")
  {
    m_Sounds[record.SoundID] = record;
  }
}

bool Sounds::hasSound(const std::string& ID) const
{
  return (m_Sounds.find(ID)!=m_Sounds.end());
}

unsigned int Sounds::getNumberOfSounds() const
{
  return m_Sounds.size();
}

const SoundRecord& Sounds::getSound(const std::string& ID) const
{
  std::map<std::string, SoundRecord>::const_iterator iter = m_Sounds.find(ID);
  if (iter!=m_Sounds.end())
  {
    return iter->second;
  }
  std::cout << "No sound with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

SoundListIterator Sounds::getBegin() const
{
  return m_Sounds.begin();
}

SoundListIterator Sounds::getEnd() const
{
  return m_Sounds.end();
}

bool Sounds::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Sounds::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  SoundListIterator iter = m_Sounds.begin();
  const SoundListIterator end_iter = m_Sounds.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Sounds::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Sounds::clearAll()
{
  m_Sounds.clear();
}

int Sounds::readRecordSOUN(std::ifstream& in_File)
{
  SoundRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Sounds::readRecordSOUN: Error while reading sound record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasSound(temp.SoundID))
  {
    if (getSound(temp.SoundID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if sound present
  addSound(temp);
  return 1;
} //readRecordSOUN

} //namespace
