/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#ifndef SR_ESMREADERCOUNT_H
#define SR_ESMREADERCOUNT_H

#include "ESMReader.h"
#include <map>

namespace SRTP
{

/* This descendant of the ESMReader class tries to count all groups and records
   in the given .esm/.esp file.
*/
class ESMReaderCount: public ESMReader
{
  public:
    /* constructor */
    ESMReaderCount();

    /* destructor */
    virtual ~ESMReaderCount();

    /* the map that counts the records. Key is the record name, value is the
       number that indicates how often that record was seen

       remarks:
           If you plan on reading several files with ESMReaderCount and display
           the numbers for each file individually, you have to clear this map
           after each file. Otherwise the numbers will accumulate and represent
           the numbers for all files that were read.
    */
    std::map<int32_t, unsigned int> RecordCounter;

    /* integer that holds the overall number of records */
    uint32_t totalRecords;

    /* the map that counts the groups. Key is the group name, value is the
       number that indicates how often that group was seen

       remarks:
           If you plan on reading several files with ESMReaderCount and display
           the numbers for each file individually, you have to clear this map
           after each file. Otherwise the numbers will accumulate and represent
           the numbers for all files that were read.
    */
    std::map<int32_t, unsigned int> GroupCounter;

    /* integer that holds the overall number of groups */
    uint32_t totalGroups;

    /* resets the current statistics to zero */
    void resetStats();

    /* puts the current statistics to standard output

       parameters:
           withPercentage - if set to true, the output will also show the
                            percentage of each record type, measured on the
                            total number of records
    */
    void showStats(const bool withPercentage=false) const;

  protected:
    /* returns true, if the given group may contains some data that the reader
       wants to read. Returns false otherwise.

       parameters:
           g_data - the group header data

       remarks:
           Returns true for all groups.
    */
    virtual bool needGroup(const GroupData& g_data) const;

    /* This function's sole purpose is to "notify" the reader that a new group
       was encountered and give the classes derived from ESMReader the
       possibility to update their internal state accordingly. Note that this
       function will only be called for groups where needGroup() returns true.
       Skipped groups will not trigger that function.

       parameters:
           g_data - the group header data
           sub    - if set to true, the new group is a sub-group of another
                    group
    */
    virtual void nextGroupStarted(const GroupData& g_data, const bool sub);

    /* This function's sole purpose is to "notify" the reader that a started
       group has been read (or skipped) completely and give the classes derived
       from ESMReader the possibility to update their internal state
       accordingly. Note that this function will only be called for groups
       where needGroup() returns true. Skipped groups will not trigger that
       function, but it might be possible that all records and subgroups in such
       a group have been skipped, depending on the implementation of
       readNextRecord() and readGroup().

       parameters:
           g_data - the group header data
    */
    virtual void groupFinished(const GroupData& g_data);

    /* tries to read the next record from a file and returns the number of
       relevant records that were read (usually one). If an error occured,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           in_File - the input file stream the record shall be read from
           recName - name (header) of the next record
    */
    virtual int readNextRecord(std::ifstream& in_File, const int32_t recName);
};//class

} //namespace

#endif // SR_ESMREADERCOUNT_H
