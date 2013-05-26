/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2013  Thoronador

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

#ifndef MW_GAMESETTINGS_H
#define MW_GAMESETTINGS_H

#include <string>
#include <map>
#include <fstream>
#include "records/GMSTRecord.h"

namespace MWTP
{

//iterator type for GMST list
typedef std::map<std::string, GMSTRecord>::const_iterator GMSTListIterator;

class GameSettings
{
  public:
    /* destructor */
    ~GameSettings();

    /* singleton access */
    static GameSettings& getSingleton();

    /* adds a setting to the list */
    void addSetting(const GMSTRecord& value);

    /* returns true, if a setting with the given name is present

       parameters:
           Name - the name of the setting

       remarks:
           In the current implementation the names of settings are case-
           sensitive, which means that "sSchoolDestruction" is different from
           setting "sSCHOOLDESTRECUTION". I am aware, that Morrowind itself does
           not distinguish between upper and lower case letters in this case,
           but case insensitivity is not implemented (yet).
    */
    bool hasSetting(const std::string& Name) const;

    /* retrieves the data record of a given setting. This setting must exist,
       or the function will throw an exception.

       parameters:
           Name - the name of the setting

       remarks:
           See remarks for hasSetting() above.
    */
    const GMSTRecord& getSetting(const std::string& Name) const;

    /* returns the current number of present setting */
    unsigned int getNumberOfSettings() const;

    /* tries to read a GMST record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordGMST(std::ifstream& in_File);

    #ifndef MW_UNSAVEABLE_RECORDS
    /* tries to save all available GMSTs to the given stream and returns true
       on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the GMSTs
    */
    bool saveAllToStream(std::ofstream& output) const;
    #endif

    /* returns constant iterator to the beginning of the internal list */
    GMSTListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    GMSTListIterator getEnd() const;

    /* deletes all GMST data */
    void clearAll();
  private:
    /* constructor - private due to singleton pattern */
    GameSettings();

    /* copy constructor - empty due to singleton pattern */
    GameSettings(const GameSettings& op)
    : m_Settings(std::map<std::string, GMSTRecord>()) {}

    /* internal data */
    std::map<std::string, GMSTRecord> m_Settings;
}; //class

} //namespace

#endif // MW_GAMESETTINGS_H
