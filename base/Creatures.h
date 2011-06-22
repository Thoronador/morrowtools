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

#ifndef CREATURES_H
#define CREATURES_H

#include <string>
#include <map>
#include "records/CreatureRecord.h"
#include "ComparisonFunctor.h"

namespace MWTP
{

//iterator type for creature list
typedef std::map<std::string, CreatureRecord, ci_less>::const_iterator CreatureListIterator;

class Creatures
{
  public:
    /* destructor */
    ~Creatures();

    /* singleton access method */
    static Creatures& getSingleton();

    /* adds a creature to the list */
    void addCreature(const CreatureRecord& record);

    /* returns true, if a creature with the given ID is present

       parameters:
           ID - the ID of the creature object
    */
    bool hasCreature(const std::string& ID) const;

    /* returns the number of creatures in the list */
    unsigned int getNumberOfCreatures() const;

    /* returns a reference to the creature record of the creature with the
       given ID

       parameters:
           ID - the ID of the creature

       remarks:
           If no creature with the given ID is present, the function will throw
           an exception. Use hasCreature() to determine, if a creature with the
           desired ID is present.
    */
    const CreatureRecord& getCreature(const std::string& ID) const;

    /* tries to read a creature record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordCREA(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    CreatureListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    CreatureListIterator getEnd() const;

    /* tries to save all available creatures to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    creatures
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all creatures from the list */
    void clearAll();
  private:
    /* constructor */
    Creatures();

    /* empty and private copy constructor (singleton) */
    Creatures(const Creatures& op) {}

    /* internal data */
    std::map<std::string, CreatureRecord, ci_less> m_Creatures;
};//class

} //namespace

#endif // CREATURES_H
