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

#ifndef SR_GAMESETTINGS_H
#define SR_GAMESETTINGS_H

#include <map>
#include <string>
#include "records/GMSTRecord.h"

namespace SRTP
{

//iterator type for game setting list
typedef std::map<std::string, GMSTRecord>::const_iterator GameSettingListIterator;

class GameSettings
{
  public:
    /* destructor */
    ~GameSettings();

    /* singleton access method */
    static GameSettings& getSingleton();

    /* adds a game setting to the list */
    void addGameSetting(const GMSTRecord& record);

    /* returns true, if a game setting with the given ID is present

       parameters:
           ID - the ID of the game setting object
    */
    bool hasGameSetting(const std::string& ID) const;

    /* returns the number of game settings in the list */
    unsigned int getNumberOfGameSettings() const;

    /* returns a reference to the game setting record of the game setting with
       the given ID

       parameters:
           ID - the ID of the game setting

       remarks:
           If no game setting with the given ID is present, the function will
           throw an exception. Use hasGameSetting() to determine, if a game
           setting with the desired ID is present.
    */
    const GMSTRecord& getGameSetting(const std::string& ID) const;

    /* tries to read a game setting record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readNextRecord(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    GameSettingListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    GameSettingListIterator getEnd() const;

    #ifndef SR_UNSAVEABLE_RECORDS
    /* tries to save all available game settings to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    game settings
    */
    bool saveAllToStream(std::ofstream& output) const;
    #endif

    /* removes all game settings from the list */
    void clearAll();
  private:
    /* constructor */
    GameSettings();

    /* empty copy constructor */
    GameSettings(const GameSettings& op) {}

    /* internal data */
    std::map<std::string, GMSTRecord> m_GameSettings;
};//class

} //namespace

#endif // SR_GAMESETTINGS_H
