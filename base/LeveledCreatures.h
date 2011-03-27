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

#ifndef LEVELEDCREATURES_H
#define LEVELEDCREATURES_H

#include <string>
#include <map>
#include "records/LeveledCreatureRecord.h"

//iterator type for leveled creature list
typedef std::map<std::string, LeveledCreatureRecord>::const_iterator LeveledCreatureListIterator;

class LeveledCreatures
{
  public:
    /* destructor */
    ~LeveledCreatures();

    /* singleton access method */
    static LeveledCreatures& getSingleton();

    /* adds a leveled creature to the list */
    void addLeveledCreature(const LeveledCreatureRecord& record);

    /* returns true, if a leveled creature with the given ID is present

       parameters:
           ID - the ID of the leveled creature object
    */
    bool hasLeveledCreature(const std::string& ID) const;

    /* returns the number of leveled creatures in the list */
    unsigned int getNumberOfLeveledCreatures() const;

    /* returns a reference to the leveled creature record of the leveled creature with the
       given ID

       parameters:
           ID - the ID of the leveled creature

       remarks:
           If no leveled creature with the given ID is present, the function will throw
           an exception. Use hasLeveledCreature() to determine, if a leveled creature with the
           desired ID is present.
    */
    const LeveledCreatureRecord& getLeveledCreature(const std::string& ID) const;

    /* tries to read a leveled creature record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordLEVC(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    LeveledCreatureListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    LeveledCreatureListIterator getEnd() const;

    /* tries to save all available leveled creatures to the given stream and
       returns true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    leveled creatures
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all leveled creatures from the list */
    void clearAll();
  private:
    /* constructor */
    LeveledCreatures();

    /* empty and private copy constructor (singleton) */
    LeveledCreatures(const LeveledCreatures& op) {}

    /* internal data */
    std::map<std::string, LeveledCreatureRecord> m_LeveledCreatures;
};//class

#endif // LEVELEDCREATURES_H
