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

#ifndef REGIONS_H
#define REGIONS_H

#include <string>
#include <set>
#include "records/RegionRecord.h"

//iterator type for region list
typedef std::set<RegionRecord>::const_iterator RegionListIterator;

class Regions
{
  public:
    /* destructor */
    ~Regions();

    /* singleton access method */
    static Regions& getSingleton();

    /* adds a region to the list */
    void addRegion(const RegionRecord& record);

    /* returns true, if a region with the given ID is present

       parameters:
           ID - the ID of the region object
    */
    bool hasRegion(const std::string& ID) const;

    /* returns the number of regions in the list */
    unsigned int getNumberOfRegions() const;

    /* returns a reference to the region record of the region with the
       given ID

       parameters:
           ID - the ID of the region

       remarks:
           If no region with the given ID is present, the function will throw
           an exception. Use hasRegion() to determine, if a region with the
           desired ID is present.
    */
    const RegionRecord& getRegion(const std::string& ID) const;

    /* tries to read a region record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordREGN(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    RegionListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    RegionListIterator getEnd() const;

    /* tries to save all available regions to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    regions
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all regions from the list */
    void clearAll();
  private:
    /* constructor */
    Regions();

    /* empty copy constructor */
    Regions(const Regions& op) {}

    /* internal data */
    std::set<RegionRecord> m_Regions;
};//class

#endif // REGIONS_H
