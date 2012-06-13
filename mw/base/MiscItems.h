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

#ifndef MW_MISCITEMS_H
#define MW_MISCITEMS_H

#include <string>
#include <map>
#include "records/MiscItemRecord.h"

namespace MWTP
{

//iterator type for misc. item list
typedef std::map<std::string, MiscItemRecord>::const_iterator MiscItemListIterator;

class MiscItems
{
  public:
    /* destructor */
    ~MiscItems();

    /* singleton access method */
    static MiscItems& getSingleton();

    /* adds a misc. item to the list */
    void addMiscItem(const MiscItemRecord& record);

    /* returns true, if a misc. item with the given ID is present

       parameters:
           ID - the ID of the misc. item object
    */
    bool hasMiscItem(const std::string& ID) const;

    /* returns the number of misc. items in the list */
    unsigned int getNumberOfMiscItems() const;

    /* returns a reference to the misc. item record of the misc. item with the
       given ID

       parameters:
           ID - the ID of the misc. item

       remarks:
           If no misc. item with the given ID is present, the function will throw
           an exception. Use hasMiscItem() to determine, if a misc. item with the
           desired ID is present.
    */
    const MiscItemRecord& getMiscItem(const std::string& ID) const;

    /* tries to read a misc. item record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordMISC(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    MiscItemListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    MiscItemListIterator getEnd() const;

    /* tries to save all available misc. items to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    misc. items
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all misc. items from the list */
    void clearAll();
  private:
    /* constructor */
    MiscItems();

    /* empty and private copy constructor (singleton) */
    MiscItems(const MiscItems& op) {}

    /* internal data */
    std::map<std::string, MiscItemRecord> m_MiscItems;
};//class

} //namespace

#endif // MW_MISCITEMS_H
