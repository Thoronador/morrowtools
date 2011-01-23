/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as published
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

#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <string>
#include <map>

enum GMSTType {gtInteger, gtFloat, gtString};

struct GMSTRecord
{
  GMSTType Type;
  int32_t iVal;
  float fVal;
  std::string sVal;
};

class GameSettings
{
  public:
    ~GameSettings();
    static GameSettings& getSingleton();
    void addSetting(const std::string& Name, const GMSTRecord& value);
    bool hasSetting(const std::string& Name) const;
    const GMSTRecord& getSetting(const std::string& Name) const;
    unsigned int getNumberOfSettings() const;
    void clearAll();
  private:
    GameSettings();
    GameSettings(const GameSettings& op) {}
    std::map<std::string, GMSTRecord> m_Settings;
}; //class

#endif // GAMESETTINGS_H
