/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "GameSettings.h"
#include <iostream>

namespace SRTP
{

GameSettings::GameSettings()
{
  //empty
}

GameSettings::~GameSettings()
{
  //empty
}

GameSettings& GameSettings::getSingleton()
{
  static GameSettings Instance;
  return Instance;
}

void GameSettings::addGameSetting(const GMSTRecord& record)
{
  if (record.getSettingName()!="")
  {
    m_GameSettings[record.getSettingName()] = record;
  }
}

bool GameSettings::hasGameSetting(const std::string& ID) const
{
  return (m_GameSettings.find(ID)!=m_GameSettings.end());
}

unsigned int GameSettings::getNumberOfGameSettings() const
{
  return m_GameSettings.size();
}

const GMSTRecord& GameSettings::getGameSetting(const std::string& ID) const
{
  std::map<std::string, GMSTRecord>::const_iterator iter = m_GameSettings.find(ID);
  if (iter!=m_GameSettings.end())
  {
    return iter->second;
  }
  std::cout << "No eye with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

GameSettingListIterator GameSettings::getBegin() const
{
  return m_GameSettings.begin();
}

GameSettingListIterator GameSettings::getEnd() const
{
  return m_GameSettings.end();
}

bool GameSettings::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "GameSettings::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  GameSettingListIterator iter = m_GameSettings.begin();
  const GameSettingListIterator end_iter = m_GameSettings.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "GameSettings::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void GameSettings::clearAll()
{
  m_GameSettings.clear();
}

int GameSettings::readNextRecord(std::ifstream& in_File)
{
  GMSTRecord temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "GameSettings::readNextRecord: Error while reading GMST record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasGameSetting(temp.getSettingName()))
  {
    if (getGameSetting(temp.getSettingName()).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if eye present
  addGameSetting(temp);
  return 1;
} //readNextRecord

} //namespace
