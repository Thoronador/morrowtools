/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2014  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "GameSettings.hpp"
#include <iostream>
#include <stdexcept>

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
  if (!record.getSettingName().empty())
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
  throw std::runtime_error("No eye with the ID \""+ID+"\" is present.");
}

GameSettingListIterator GameSettings::getBegin() const
{
  return m_GameSettings.begin();
}

GameSettingListIterator GameSettings::getEnd() const
{
  return m_GameSettings.end();
}

#ifndef SR_UNSAVEABLE_RECORDS
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
#endif

void GameSettings::clearAll()
{
  m_GameSettings.clear();
}

int GameSettings::readNextRecord(std::ifstream& in_File, const bool localized, const StringTable& table)
{
  GMSTRecord temp;
  if(!temp.loadFromStream(in_File, localized, table))
  {
    std::cout << "GameSettings::readNextRecord: Error while reading GMST record.\n";
    return -1;
  }

  #if !defined(SR_NO_SINGLETON_EQUALITY_CHECK) && !defined(SR_NO_RECORD_EQUALITY)
  //add it to the list, if not present with same data
  if (hasGameSetting(temp.getSettingName()))
  {
    if (getGameSetting(temp.getSettingName()).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if game setting present
  #endif
  addGameSetting(temp);
  return 1;
} //readNextRecord

} //namespace
