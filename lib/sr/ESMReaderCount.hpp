/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2025  Dirk Stolle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef SR_ESMREADERCOUNT_HPP
#define SR_ESMREADERCOUNT_HPP

#include "ESMReader.hpp"
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

    /* the map that counts the records. Key is the record name, value is the
       number that indicates how often that record was seen

       remarks:
           If you plan on reading several files with ESMReaderCount and display
           the numbers for each file individually, you have to clear this map
           after each file. Otherwise the numbers will accumulate and represent
           the numbers for all files that were read.
    */
    std::map<uint32_t, unsigned int> RecordCounter;

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
    std::map<uint32_t, unsigned int> GroupCounter;

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
    void showStats(const bool withPercentage = false) const;

  protected:
    /* returns true, if the given group may contains some data that the reader
       wants to read. Returns false otherwise.

       parameters:
           g_data - the group header data

       remarks:
           Returns true for all groups.
    */
    virtual bool needGroup(const GroupData& g_data) const override;

    /* This function's sole purpose is to "notify" the reader that a new group
       was encountered and give the classes derived from ESMReader the
       possibility to update their internal state accordingly. Note that this
       function will only be called for groups where needGroup() returns true.
       Skipped groups will not trigger that function.
       Shall return false only if the read process should be aborted.

       parameters:
           g_data - the group header data
           sub    - if set to true, the new group is a sub-group of another
                    group
    */
    virtual bool nextGroupStarted(const GroupData& g_data, const bool sub) override;

    /* This function's sole purpose is to "notify" the reader that a started
       group has been read (or skipped) completely and give the classes derived
       from ESMReader the possibility to update their internal state
       accordingly. Note that this function will only be called for groups
       where needGroup() returns true. Skipped groups will not trigger that
       function, but it might be possible that all records and subgroups in such
       a group have been skipped, depending on the implementation of
       readNextRecord() and readGroup().
       Shall return false only if the read process should be aborted.

       parameters:
           g_data - the group header data
    */
    virtual bool groupFinished(const GroupData& g_data) override;

    /* tries to read the next record from a file and returns the number of
       relevant records that were read (usually one). If an error occurred,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           input   - the input stream the record shall be read from
           recName - name (header) of the next record
           localized - true, if the data in the stream is localized
           table     - in case of localized data: the string table
    */
    virtual int readNextRecord(std::istream& input, const uint32_t recName, const bool localized, const StringTable& table) override;
}; // class

} // namespace

#endif // SR_ESMREADERCOUNT_HPP
