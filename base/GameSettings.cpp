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

#include "GameSettings.h"
#include <iostream>

namespace MWTP
{

GameSettings::GameSettings()
{
  //empty
}

GameSettings::~GameSettings()
{
  m_Settings.clear();
}

GameSettings& GameSettings::getSingleton()
{
  static GameSettings Instance;
  return Instance;
}

void GameSettings::addSetting(const GMSTRecord& value)
{
  m_Settings[value.SettingName] = value;
}

bool GameSettings::hasSetting(const std::string& Name) const
{
  return m_Settings.find(Name)!=m_Settings.end();
}

const GMSTRecord& GameSettings::getSetting(const std::string& Name) const
{
  std::map<std::string, GMSTRecord>::const_iterator iter;
  iter = m_Settings.find(Name);
  if (iter!=m_Settings.end())
  {
    return iter->second;
  }
  std::cout << "Error: setting \""<<Name<<"\" not found.\n";
  throw 42;
}

unsigned int GameSettings::getNumberOfSettings() const
{
  return m_Settings.size();
}

int GameSettings::readRecordGMST(std::ifstream& in_File)
{
  GMSTRecord temp;
  if (temp.loadFromStream(in_File))
  {
    //check for presence of setting
    if (hasSetting(temp.SettingName))
    {
      if (getSetting(temp.SettingName).equals(temp))
      {
        return 0; //return zero, nothing was changed
      }
    }//if
    addSetting(temp);
    return 1;
  }//if
  std::cout << "Error while reading data subrecord of GMST.\n";
  return -1;
}//readRecordGMST (integer version of readGMST)


bool GameSettings::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "GameSettings::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  GMSTListIterator iter = m_Settings.begin();
  const GMSTListIterator end_iter = m_Settings.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "GameSettings::saveAllToStream: Error while writing record.\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

GMSTListIterator GameSettings::getBegin() const
{
  return m_Settings.begin();
}

GMSTListIterator GameSettings::getEnd() const
{
  return m_Settings.end();
}

void GameSettings::clearAll()
{
  m_Settings.clear();
}

} //namespace
