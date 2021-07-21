/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef SR_STRINGTABLECOMPOUND_HPP
#define SR_STRINGTABLECOMPOUND_HPP

#include "StringTable.hpp"

namespace SRTP
{

class StringTableCompound
{
  public:
    /* constructor */
    StringTableCompound();

    /* destructor */
    ~StringTableCompound();

    enum TableType {ttNone, ttDLStrings, ttILStrings, ttstrings};

    /* adds a string with the given ID and content to the given string table in
       the compound. If a string with the same ID is already present in that
       table, that string will be replaced. If the string ID is zero or the
       table type is ttNone, no string will be added or changed.

       parameters:
           stringID - the ID of the string
           content  - the (new) content of the string
           tt       - the table that shall hold the new string
    */
    void addString(const uint32_t stringID, const std::string& content, const TableType tt);

    /* returns true, if the one of the tables contains a string with the given
       ID

       parameters:
           stringID - the ID of the string
    */
    bool hasString(const uint32_t stringID) const;

    /* returns the type of the table that contains the string with the given
       string ID, or ttNone if the string was not found.

       parameters:
           stringID - the ID of the string
    */
    TableType locateString(const uint32_t stringID) const;

    /* returns the string for a given ID. If no string with that ID is present,
       the function will throw an exception. Use hasString() to check the
       presence of a string first.

       parameters:
           stringID - the ID of the string
    */
    const std::string& getString(const uint32_t stringID) const;

    /* returns a reference to the specified string table. If the type is ttNone,
       the function will throw an exception.

       parameters:
           tt - the requested table
    */
    const StringTable& getStringTable(const TableType tt) const;

    /* tries to read three string tables from the given file path and returns
       true in case of success, false on failure

       parameters:
           BaseFileName - file name suffix for all three string tables

       remarks:
           The BaseFileName parameter has to be the full path to the tables
           without their respective extension. For example, if you want to
           read the three string table from the files
             C:\Program Files\Steam\SteamApps\common\skyrim\Data\Strings\Skyrim_English.DLStrings
             C:\Program Files\Steam\SteamApps\common\skyrim\Data\Strings\Skyrim_English.ILStrings
             C:\Program Files\Steam\SteamApps\common\skyrim\Data\Strings\Skyrim_English.strings
           then the BaseFileName has to be
             "C:\Program Files\Steam\SteamApps\common\skyrim\Data\Strings\Skyrim_English"
             (without the quotes of course).
    */
    bool readCompound(const std::string& BaseFileName);

    /* tries to write three string tables to the given file path and returns
       true in case of success, false on failure

       parameters:
           BaseFileName - file name suffix for all three string tables

       remarks:
           For further explanation about the parameters, see the remarks of the
           readCompound() function above.
    */
    bool writeCompound(const std::string& BaseFileName) const;

    /* returns the number of strings in the table compound */
    uint32_t getNumberOfCompoundEntries() const;
  private:
    StringTable m_DLStrings;
    StringTable m_ILStrings;
    StringTable m_strings;
}; //class

} //namespace

#endif // SR_STRINGTABLECOMPOUND_HPP
