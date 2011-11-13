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

#include "BodyParts.h"
#include <iostream>

namespace MWTP
{

BodyParts::BodyParts()
{
  //empty
}

BodyParts::~BodyParts()
{
  //empty
}

BodyParts& BodyParts::getSingleton()
{
  static BodyParts Instance;
  return Instance;
}

void BodyParts::addBodyPart(const BodyPartRecord& record)
{
  if (record.BodyPartID!="")
  {
    m_BodyParts.insert(record);
  }
}

bool BodyParts::hasBodyPart(const std::string& ID) const
{
  return (m_BodyParts.find(ID)!=m_BodyParts.end());
}

unsigned int BodyParts::getNumberOfBodyParts() const
{
  return m_BodyParts.size();
}

const BodyPartRecord& BodyParts::getBodyPart(const std::string& ID) const
{
  std::set<BodyPartRecord>::const_iterator iter = m_BodyParts.find(ID);
  if (iter!=m_BodyParts.end())
  {
    return *iter;
  }
  std::cout << "No body part with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

BodyPartListIterator BodyParts::getBegin() const
{
  return m_BodyParts.begin();
}

BodyPartListIterator BodyParts::getEnd() const
{
  return m_BodyParts.end();
}

bool BodyParts::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "BodyParts::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  BodyPartListIterator iter = m_BodyParts.begin();
  const BodyPartListIterator end_iter = m_BodyParts.end();
  while (iter!=end_iter)
  {
    if (!iter->saveToStream(output))
    {
      std::cout << "BodyParts::saveAllToStream: Error while writing record for \""
                << iter->BodyPartID <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void BodyParts::clearAll()
{
  m_BodyParts.clear();
}

int BodyParts::readRecordBODY(std::ifstream& in_File)
{
  BodyPartRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Error while reading record LTEX!\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasBodyPart(temp.BodyPartID))
  {
    if (getBodyPart(temp.BodyPartID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if activator present
  addBodyPart(temp);
  return 1;
} //readRecordACTI

} //namespace
