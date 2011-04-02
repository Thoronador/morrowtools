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

#ifndef LOCKPICKS_H
#define LOCKPICKS_H

#include <string>
#include <map>
#include "records/LockpickRecord.h"

//iterator type for lockpick list
typedef std::map<std::string, LockpickRecord>::const_iterator LockpickListIterator;

class Lockpicks
{
  public:
    /* destructor */
    ~Lockpicks();

    /* singleton access method */
    static Lockpicks& getSingleton();

    /* adds a lockpick to the list */
    void addLockpick(const LockpickRecord& record);

    /* returns true, if a lockpick with the given ID is present

       parameters:
           ID - the ID of the lockpick object
    */
    bool hasLockpick(const std::string& ID) const;

    /* returns the number of lockpicks in the list */
    unsigned int getNumberOfLockpicks() const;

    /* returns a reference to the lockpick record of the lockpick with the given ID

       parameters:
           ID - the ID of the lockpick

       remarks:
           If no lockpick with the given ID is present, the function will throw
           an exception. Use hasLockpick() to determine, if a lockpick with the
           desired ID is present.
    */
    const LockpickRecord& getLockpick(const std::string& ID) const;

    /* tries to read a lockpick record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordLOCK(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    LockpickListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    LockpickListIterator getEnd() const;

    /* tries to save all available lockpicks to the given stream and
       returns true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    lockpicks
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all lockpicks from the list */
    void clearAll();
  private:
    /* constructor */
    Lockpicks();

    /* empty and private copy constructor (singleton) */
    Lockpicks(const Lockpicks& op) {}

    /* internal data */
    std::map<std::string, LockpickRecord> m_Lockpicks;
};//class

#endif // LOCKPICKS_H
