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

#ifndef SR_STRINGTABLE_H_INCLUDED
#define SR_STRINGTABLE_H_INCLUDED

#include <string>
#include <map>
#include <stdint.h>

namespace SRTP
{

class StringTable
{
  public:
    /* constructor */
    StringTable();

    /* destructor */
    ~StringTable();

    enum DataType {sdUnknown, sdNULterminated, sdPascalStyle};

    //iterator type
    typedef std::map<uint32_t, std::string>::const_iterator TableIterator;

    /* adds a string with the given ID and content to the string table. If a
       string with the same ID is already present, that string will be replaced.
       If the string ID is zero, no string will be added or changed.

       parameters:
           stringID - the ID of the string
           content  - the (new) content of the string
    */
    void addString(const uint32_t stringID, const std::string& content);

    /* returns true, if the table contains a string with the given ID */
    bool hasString(const uint32_t stringID) const;

    /* returns the string for a given ID. If no string with that ID is present,
       the function will throw an exception. Use hasString() to check the
       presence of a string first.

       parameters:
           stringID - the ID of the string
    */
    const std::string& getString(const uint32_t stringID) const;

    /* tries to remove the string with the given ID from the string table and
       returns true, if such a string was removed. Returns false otherwise.

       parameters:
           stringID - the ID of the string
    */
    bool deleteString(const uint32_t stringID);

    /* removes all entries from the string table */
    void tabulaRasa();

    /* tries to read a string table from the given file and returns true in case
       of success, false on failure

       parameters:
           FileName   - name of the file containing the string table
           stringType - expected type of string data in table
    */
    bool readTable(const std::string& FileName, DataType stringType);

    /* tries to write a string table to the given file and returns true in case
       of success, false on failure

       parameters:
           FileName   - name of the output file for the string table
           stringType - desired type of string data in table
    */
    bool writeTable(const std::string& FileName, DataType stringType) const;

    /* merges the contents of the other table into this table. If there are
       strings with the same string ID in both tables, the strings from the
       other table take precedence.

       parameters:
           other - the other string table
    */
    void mergeTables(const StringTable& other);

    /* returns iterator to the start of the table */
    TableIterator getBegin() const;

    /* returns iterator to the end of the table */
    TableIterator getEnd() const;

    /* returns the number of strings in the table */
    uint32_t getNumberOfTableEntries() const;
  private:
    struct DirectoryEntry
    {
      uint32_t stringID;
      uint32_t offset;
    }; //struct

    //internal list to hold the strings
    std::map<uint32_t, std::string> m_Strings;
}; //class

} //namespace

#endif // SR_STRINGTABLE_H_INCLUDED
