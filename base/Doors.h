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

#ifndef DOORS_H
#define DOORS_H

#include <string>
#include <map>
#include "records/DoorRecord.h"

//iterator type for door list
typedef std::map<std::string, DoorRecord>::const_iterator DoorListIterator;

class Doors
{
  public:
    /* destructor */
    ~Doors();

    /* singleton access method */
    static Doors& getSingleton();

    /* adds a door to the list */
    void addDoor(const DoorRecord& record);

    /* returns true, if a door with the given ID is present

       parameters:
           ID - the ID of the door object
    */
    bool hasDoor(const std::string& ID) const;

    /* returns the number of doors in the list */
    unsigned int getNumberOfDoors() const;

    /* returns a reference to the door record of the door with the
       given ID

       parameters:
           ID - the ID of the door

       remarks:
           If no door with the given ID is present, the function will throw
           an exception. Use hasDoor() to determine, if a door with the
           desired ID is present.
    */
    const DoorRecord& getDoor(const std::string& ID) const;

    /* tries to read a door record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordDOOR(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    DoorListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    DoorListIterator getEnd() const;

    /* tries to save all available doors to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    doors
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all doors from the list */
    void clearAll();
  private:
    /* constructor - private due to singleton pattern*/
    Doors();

    /* empty copy constructor */
    Doors(const Doors& op) {}

    /* internal data */
    std::map<std::string, DoorRecord> m_Doors;
};//class

#endif // DOORS_H
