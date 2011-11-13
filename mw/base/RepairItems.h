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

#ifndef REPAIRITEMS_H
#define REPAIRITEMS_H

#include <string>
#include <set>
#include "records/RepairItemRecord.h"

namespace MWTP
{

//iterator type for repair item list
typedef std::set<RepairItemRecord>::const_iterator RepairItemListIterator;

class RepairItems
{
  public:
    /* destructor */
    ~RepairItems();

    /* singleton access method */
    static RepairItems& getSingleton();

    /* adds a repair item to the list */
    void addRepairItem(const RepairItemRecord& record);

    /* returns true, if a repair item with the given ID is present

       parameters:
           ID - the ID of the repair item object
    */
    bool hasRepairItem(const std::string& ID) const;

    /* returns the number of repair items in the list */
    unsigned int getNumberOfRepairItems() const;

    /* returns a reference to the repair item record of the repair item with the
       given ID

       parameters:
           ID - the ID of the repair item

       remarks:
           If no repair item with the given ID is present, the function will throw
           an exception. Use hasRepairItem() to determine, if a repair item with the
           desired ID is present.
    */
    const RepairItemRecord& getRepairItem(const std::string& ID) const;

    /* tries to read a repair item record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordREPA(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    RepairItemListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    RepairItemListIterator getEnd() const;

    /* tries to save all available repair items to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    repair items
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all repair items from the list */
    void clearAll();
  private:
    /* constructor */
    RepairItems();

    /* empty copy constructor */
    RepairItems(const RepairItems& op) {}

    /* internal data */
    std::set<RepairItemRecord> m_RepairItems;
};//class

} //namespace

#endif // REPAIRITEMS_H
