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

#ifndef LEVELEDITEMS_H
#define LEVELEDITEMS_H

#include <string>
#include <map>
#include "records/LeveledItemRecord.h"

//iterator type for leveled item list
typedef std::map<std::string, LeveledItemRecord>::const_iterator LeveledItemListIterator;

class LeveledItems
{
  public:
    /* destructor */
    ~LeveledItems();

    /* singleton access method */
    static LeveledItems& getSingleton();

    /* adds a leveled item to the list */
    void addLeveledItem(const LeveledItemRecord& record);

    /* returns true, if a leveled item with the given ID is present

       parameters:
           ID - the ID of the leveled item object
    */
    bool hasLeveledItem(const std::string& ID) const;

    /* returns the number of leveled items in the list */
    unsigned int getNumberOfLeveledItems() const;

    /* returns a reference to the leveled item record of the leveled item with the
       given ID

       parameters:
           ID - the ID of the leveled item

       remarks:
           If no leveled item with the given ID is present, the function will throw
           an exception. Use hasLeveledItem() to determine, if a leveled item with the
           desired ID is present.
    */
    const LeveledItemRecord& getLeveledItem(const std::string& ID) const;

    /* tries to read a leveled item record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordLEVI(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    LeveledItemListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    LeveledItemListIterator getEnd() const;

    /* tries to save all available leveled items to the given stream and
       returns true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    leveled items
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all leveled items from the list */
    void clearAll();
  private:
    /* constructor */
    LeveledItems();

    /* empty and private copy constructor (singleton) */
    LeveledItems(const LeveledItems& op) {}

    /* internal data */
    std::map<std::string, LeveledItemRecord> m_LeveledItems;
};//class

#endif // LEVELEDITEMS_H
