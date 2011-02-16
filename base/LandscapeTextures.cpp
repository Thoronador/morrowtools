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

#include "LandscapeTextures.h"
#include <iostream>

LandscapeTextures::LandscapeTextures()
{
  //empty
}

LandscapeTextures::~LandscapeTextures()
{
  //empty
}

LandscapeTextures& LandscapeTextures::getSingleton()
{
  static LandscapeTextures Instance;
  return Instance;
}

void LandscapeTextures::addLandscapeTexture(const std::string& ID, const LandscapeTextureRecord& record)
{
  if (ID!="")
  {
    m_LandscapeTextures[ID] = record;
  }
}

bool LandscapeTextures::hasLandscapeTexture(const std::string& ID) const
{
  return (m_LandscapeTextures.find(ID)!=m_LandscapeTextures.end());
}

unsigned int LandscapeTextures::getNumberOfLandscapeTextures() const
{
  return m_LandscapeTextures.size();
}

const LandscapeTextureRecord& LandscapeTextures::getLandscapeTexture(const std::string& ID) const
{
  std::map<std::string, LandscapeTextureRecord>::const_iterator iter = m_LandscapeTextures.find(ID);
  if (iter!=m_LandscapeTextures.end())
  {
    return iter->second;
  }
  std::cout << "No landscape texture with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

LandscapeTextureListIterator LandscapeTextures::getBegin() const
{
  return m_LandscapeTextures.begin();
}

LandscapeTextureListIterator LandscapeTextures::getEnd() const
{
  return m_LandscapeTextures.end();
}

bool LandscapeTextures::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "LandscapeTextures::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  LandscapeTextureListIterator iter = m_LandscapeTextures.begin();
  const LandscapeTextureListIterator end_iter = m_LandscapeTextures.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "LandscapeTextures::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void LandscapeTextures::clearAll()
{
  m_LandscapeTextures.clear();
}

int LandscapeTextures::readRecordLTEX(std::ifstream& in_File)
{
  LandscapeTextureRecord temp;
  if (!temp.loadFromStream(in_File))
  {
    std::cout << "Error while reading record LTEX!\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasLandscapeTexture(temp.TextureID))
  {
    if (getLandscapeTexture(temp.TextureID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if activator present
  addLandscapeTexture(temp.TextureID, temp);
  return 1;
} //readRecordACTI
