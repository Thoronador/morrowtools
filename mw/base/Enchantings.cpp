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

#include "Enchantings.h"
#include <iostream>

namespace MWTP
{

Enchantings::Enchantings()
{
  //empty
}

Enchantings::~Enchantings()
{
  //empty
}

Enchantings& Enchantings::getSingleton()
{
  static Enchantings Instance;
  return Instance;
}

void Enchantings::addEnchanting(const EnchantingRecord& record)
{
  if (record.EnchantingID!="")
  {
    m_Enchantings[record.EnchantingID] = record;
  }
}

bool Enchantings::hasEnchanting(const std::string& ID) const
{
  return (m_Enchantings.find(ID)!=m_Enchantings.end());
}

unsigned int Enchantings::getNumberOfEnchantings() const
{
  return m_Enchantings.size();
}

const EnchantingRecord& Enchantings::getEnchanting(const std::string& ID) const
{
  std::map<std::string, EnchantingRecord>::const_iterator iter = m_Enchantings.find(ID);
  if (iter!=m_Enchantings.end())
  {
    return iter->second;
  }
  std::cout << "No enchanting with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

EnchantingListIterator Enchantings::getBegin() const
{
  return m_Enchantings.begin();
}

EnchantingListIterator Enchantings::getEnd() const
{
  return m_Enchantings.end();
}

bool Enchantings::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Enchantings::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  EnchantingListIterator iter = m_Enchantings.begin();
  const EnchantingListIterator end_iter = m_Enchantings.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Enchantings::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Enchantings::clearAll()
{
  m_Enchantings.clear();
}

int Enchantings::readRecordENCH(std::ifstream& in_File)
{
  EnchantingRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Enchantings::readRecordENCH: Error while reading enchanting "
              << "effect record.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasEnchanting(temp.EnchantingID))
  {
    if (getEnchanting(temp.EnchantingID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if enchanting present
  addEnchanting(temp);
  return 1;
} //readRecordENCH

} //namespace
