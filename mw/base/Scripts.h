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

#ifndef MW_SCRIPTS_H
#define MW_SCRIPTS_H

#include <string>
#include <map>
#include "records/ScriptRecord.h"

namespace MWTP
{

//iterator type for script list
typedef std::map<std::string, ScriptRecord>::const_iterator ScriptListIterator;

class Scripts
{
  public:
    /* destructor */
    ~Scripts();

    /* singleton access method */
    static Scripts& getSingleton();

    /* adds a script to the list */
    void addScript(const ScriptRecord& record);

    /* returns true, if a script with the given ID is present

       parameters:
           ID - the ID of the script object
    */
    bool hasScript(const std::string& ID) const;

    /* returns the number of scripts in the list */
    unsigned int getNumberOfScripts() const;

    /* returns a reference to the script record of the script with the
       given ID

       parameters:
           ID - the ID of the script

       remarks:
           If no script with the given ID is present, the function will throw
           an exception. Use hasScript() to determine, if a script with the
           desired ID is present.
    */
    const ScriptRecord& getScript(const std::string& ID) const;

    /* tries to read a script record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordSCPT(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    ScriptListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    ScriptListIterator getEnd() const;

    /* tries to save all available scripts to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    scripts
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all scripts from the list */
    void clearAll();
  private:
    /* constructor */
    Scripts();

    /* empty copy constructor */
    Scripts(const Scripts& op) {}

    /* internal data */
    std::map<std::string, ScriptRecord> m_Scripts;
};//class

} //namespace

#endif // MW_SCRIPTS_H
