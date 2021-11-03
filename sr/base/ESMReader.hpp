/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#ifndef SR_ESMREADER_HPP
#define SR_ESMREADER_HPP

#include <fstream>
#include "StringTable.hpp"
#include "records/TES4HeaderRecord.hpp"
#include "records/GroupData.hpp"

namespace SRTP
{

class ESMReader
{
  public:
    /* constructor */
    ESMReader();

    /* destructor */
    virtual ~ESMReader();

    /* skips a record and returns 0 on success, and -1 on error */
    static int skipRecord(std::istream& input);

    /* skips a group and returns 0 on success, and -1 on error */
    static int skipGroup(std::istream& input, const GroupData& g_data);

    /* tries to read an .esm/.esp file and returns the number of relevant
       records that were read. If an error occurred, -1 is returned.

       parameters:
           FileName - name of the .esm/.esp file
           head     - the record that will be used to store the header information
    */
    int readESM(const std::string& FileName, Tes4HeaderRecord& head);

    /* tries to read the TES4 record of an .esm/.esp file and returns true in
       case of success, false on failure.

       parameters:
           FileName - name of the .esm/.esp file
           theHead  - the Tes4HeaderRecord that will be used to store the header data
    */
    static bool peekESMHeader(const std::string& FileName, Tes4HeaderRecord& theHead);

  protected:
    /* tries to read the next group from a file and returns the number of
       relevant groups that were read (usually one). If an error occurred,
       -1 is returned. If the group was skipped or contained no relevant data,
       zero is returned.

       parameters:
           input      - the input stream the group shall be read from
           withHeader - if set to true, the starting four bytes (GRUP) will be
                        read, too. If set to false, the functions expects the
                        data directly.
           localized  - true, if the data in the stream is localized
           table      - in case of localized data: the string table
    */
    int processGroup(std::istream& input, const bool withHeader, const bool localized, const StringTable& table);

    /* returns true, if the given group may contains some data that the reader
       wants to read. Returns false otherwise.

       parameters:
           g_data - the group header data

       remarks:
           If you actually want to read some data, you have to derive a class
           from ESMReader and set its needGroup() function up in a way that it
           does not just always return false and causes processGroup() to skip
           all groups and data records.
    */
    virtual bool needGroup(const GroupData& g_data) const = 0;

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
    virtual bool nextGroupStarted(const GroupData& g_data, const bool sub) = 0;

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
    virtual bool groupFinished(const GroupData& g_data) = 0;

    /* tries to read the next group from a file and returns the number of
       relevant groups that were read (usually one). If an error occurred,
       -1 is returned. If the group was skipped or contained no relevant data,
       zero is returned.
       This function is guaranteed to be only called for needed groups (see the
       function needGroup() for details).

       parameters:
           input     - the input stream used to read the group
           g_data    - group's data header
           localized - true, if the data in the stream is localized
           table     - in case of localized data: the string table

       remarks:
           If you actually want to read some data, you have to derive a class
           from ESMReader and set its readGroup() function up in a way
           that does not just skip all groups - because that is what the
           implementation here in ESMReader does.
           So naturally, this function will never return values larger than
           zero.
    */
    virtual int readGroup(std::istream& input, const GroupData& g_data, const bool localized, const StringTable& table);

    /* tries to read the next record from a file and returns the number of
       relevant records that were read (usually one). If an error occurred,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           input     - the input stream the record shall be read from
           recName   - name (header) of the next record
           localized - true, if the data in the stream is localized
           table     - in case of localized data: the string table

       remarks:
           If you actually want to read some data, you have to derive a class
           from ESMReader and set its readNextRecord() function up in a way
           that does read the data records you want to have.
    */
    virtual int readNextRecord(std::istream& input, const uint32_t recName, const bool localized, const StringTable& table) = 0;

    /* Member variable to hold the current/last TES4 header record that was
       read by the reader.

       Notes:
           This is only valid, if readESM() has been called. Otherwise its
           content is undefined.
    */
    Tes4HeaderRecord currentHead;
}; // class

} // namespace

#endif // SR_ESMREADER_HPP
