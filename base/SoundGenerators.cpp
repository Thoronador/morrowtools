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

#include "SoundGenerators.h"
#include <iostream>

SoundGenerators::SoundGenerators()
{
  //empty
}

SoundGenerators::~SoundGenerators()
{
  //empty
}

SoundGenerators& SoundGenerators::getSingleton()
{
  static SoundGenerators Instance;
  return Instance;
}

void SoundGenerators::addSoundGenerator(const SoundGeneratorRecord& record)
{
  if (record.Name!="")
  {
    m_SoundGenerators[record.Name] = record;
  }
}

bool SoundGenerators::hasSoundGenerator(const std::string& ID) const
{
  return (m_SoundGenerators.find(ID)!=m_SoundGenerators.end());
}

unsigned int SoundGenerators::getNumberOfSoundGenerators() const
{
  return m_SoundGenerators.size();
}

const SoundGeneratorRecord& SoundGenerators::getSoundGenerator(const std::string& ID) const
{
  std::map<std::string, SoundGeneratorRecord>::const_iterator iter = m_SoundGenerators.find(ID);
  if (iter!=m_SoundGenerators.end())
  {
    return iter->second;
  }
  std::cout << "No sound generator with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

SoundGeneratorListIterator SoundGenerators::getBegin() const
{
  return m_SoundGenerators.begin();
}

SoundGeneratorListIterator SoundGenerators::getEnd() const
{
  return m_SoundGenerators.end();
}

bool SoundGenerators::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "SoundGenerators::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  SoundGeneratorListIterator iter = m_SoundGenerators.begin();
  const SoundGeneratorListIterator end_iter = m_SoundGenerators.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "SoundGenerators::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void SoundGenerators::clearAll()
{
  m_SoundGenerators.clear();
}

int SoundGenerators::readRecordSNDG(std::ifstream& in_File)
{
  SoundGeneratorRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "SoundGenerators::readRecordSNDG: Error while reading sound "
              << "generator record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasSoundGenerator(temp.Name))
  {
    if (getSoundGenerator(temp.Name).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if sound generator present
  addSoundGenerator(temp);
  return 1;
} //readRecordSNDG
