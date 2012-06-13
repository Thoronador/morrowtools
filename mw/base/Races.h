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

#ifndef MW_RACES_H
#define MW_RACES_H

#include <string>
#include <set>
#include "records/RaceRecord.h"

namespace MWTP
{

//iterator type for race list
typedef std::set<RaceRecord>::const_iterator RaceListIterator;

class Races
{
  public:
    /* destructor */
    ~Races();

    /* singleton access method */
    static Races& getSingleton();

    /* adds a race to the list */
    void addRace(const RaceRecord& record);

    /* returns true, if a race with the given ID is present

       parameters:
           ID - the ID of the race object
    */
    bool hasRace(const std::string& ID) const;

    /* returns the number of races in the list */
    unsigned int getNumberOfRaces() const;

    /* returns a reference to the race record of the race with the
       given ID

       parameters:
           ID - the ID of the race

       remarks:
           If no race with the given ID is present, the function will throw
           an exception. Use hasRace() to determine, if a race with the
           desired ID is present.
    */
    const RaceRecord& getRace(const std::string& ID) const;

    /* tries to read a race record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordRACE(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    RaceListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    RaceListIterator getEnd() const;

    /* tries to save all available races to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    races
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all races from the list */
    void clearAll();
  private:
    /* constructor */
    Races();

    /* empty copy constructor */
    Races(const Races& op) {}

    /* internal data */
    std::set<RaceRecord> m_Races;
};//class

} //namespace

#endif // MW_RACES_H
