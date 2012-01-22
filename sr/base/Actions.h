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

#ifndef SR_ACTIONS_H
#define SR_ACTIONS_H

#include <map>
#include <string>
#include "records/ActionRecord.h"

namespace SRTP
{

//iterator type for action list
typedef std::map<std::string, ActionRecord>::const_iterator ActionListIterator;

class Actions
{
  public:
    /* destructor */
    ~Actions();

    /* singleton access method */
    static Actions& getSingleton();

    /* adds a action to the list */
    void addAction(const ActionRecord& record);

    /* returns true, if a action with the given ID is present

       parameters:
           ID - the ID of the action object
    */
    bool hasAction(const std::string& ID) const;

    /* returns the number of actions in the list */
    unsigned int getNumberOfActions() const;

    /* returns a reference to the action record of the action with the given ID

       parameters:
           ID - the ID of the action

       remarks:
           If no action with the given ID is present, the function will throw
           an exception. Use hasAction() to determine, if a action with the
           desired ID is present.
    */
    const ActionRecord& getAction(const std::string& ID) const;

    /* tries to read a action record from the given input file stream.

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
    ActionListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    ActionListIterator getEnd() const;

    #ifndef SR_UNSAVEABLE_RECORDS
    /* tries to save all available actions to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    actions
    */
    bool saveAllToStream(std::ofstream& output) const;
    #endif

    /* removes all actions from the list */
    void clearAll();
  private:
    /* constructor */
    Actions();

    /* empty copy constructor */
    Actions(const Actions& op) {}

    /* internal data */
    std::map<std::string, ActionRecord> m_Actions;
};//class

} //namespace

#endif // SR_ACTIONS_H
