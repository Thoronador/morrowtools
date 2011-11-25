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

#ifndef SR_GLOBALS_H
#define SR_GLOBALS_H

#include <map>
#include <string>
#include "records/GlobalRecord.h"

namespace SRTP
{

//iterator type for global list
typedef std::map<std::string, GlobalRecord>::const_iterator GlobalListIterator;

class Globals
{
  public:
    /* destructor */
    ~Globals();

    /* singleton access method */
    static Globals& getSingleton();

    /* adds a global to the list */
    void addGlobal(const GlobalRecord& record);

    /* returns true, if a global with the given ID is present

       parameters:
           ID - the ID of the global object
    */
    bool hasGlobal(const std::string& ID) const;

    /* returns the number of globals in the list */
    unsigned int getNumberOfGlobals() const;

    /* returns a reference to the global record of the global with the given ID

       parameters:
           ID - the ID of the global

       remarks:
           If no global with the given ID is present, the function will throw
           an exception. Use hasGlobal() to determine, if a global with the
           desired ID is present.
    */
    const GlobalRecord& getGlobal(const std::string& ID) const;

    /* tries to read a global record from the given input file stream.

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
    GlobalListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    GlobalListIterator getEnd() const;

    /* tries to save all available globals to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    globals
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all globals from the list */
    void clearAll();
  private:
    /* constructor */
    Globals();

    /* empty copy constructor */
    Globals(const Globals& op) {}

    /* internal data */
    std::map<std::string, GlobalRecord> m_Globals;
};//class

} //namespace

#endif // SR_GLOBALS_H