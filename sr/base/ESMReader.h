/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef SR_ESMREADER_H
#define SR_ESMREADER_H

#include <fstream>
#include "records/TES4HeaderRecord.h"
#include "records/GroupData.h"

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
    static int skipRecord(std::ifstream& in_File);

    /* skips a group and returns 0 on success, and -1 on error */
    static int skipGroup(std::ifstream& in_File, const GroupData& g_data);

    /* tries to read an .esm/.esp file and returns the number of relevant
       records that were read. If an error occured, -1 is returned.

       parameters:
           FileName - name of the .esm/.esp file
           head     - the record that will be used to store the header information
    */
    int readESM(const std::string& FileName, Tes4HeaderRecord& head);

  protected:
    /* tries to read the next group from a file and returns the number of
       relevant groups that were read (usually one). If an error occured,
       -1 is returned. If the group was skipped or contained no relevant data,
       zero is returned.

       parameters:
           in_File  - the input file stream the group shall be read from
    */
    int processGroup(std::ifstream& in_File);

    /* returns true, if the given group may contains some data that the reader
       wants to read. Returns false otherwise.

       parameters:
           g_date - the group header data

       remarks:
           If you actually want to read some data, you have to derive a class
           from ESMReader and set its needGroup() function up in a way that it
           does not just always return false and causes processGroup() to skip
           all groups and data records - because that is what the implementation
           here in ESMReader basically does.
    */
    virtual bool needGroup(const GroupData& g_data) const;

    /* tries to read the next group from a file and returns the number of
       relevant groups that were read (usually one). If an error occured,
       -1 is returned. If the group was skipped or contained no relevant data,
       zero is returned.
       This function is guaranteed to be only called for needed groups (see the
       function needGroup() for details).

       parameters:
           in_File - the input file stream used to read the group
           g_data  - group's data header

       remarks:
           If you actually want to read some data, you have to derive a class
           from ESMReader and set its readGroup() function up in a way
           that does not just skip all groups - because that is what the
           implementation here in ESMReader does.
           So naturally, this function will never return values larger than
           zero.
    */
    virtual int readGroup(std::ifstream& in_File, const GroupData& g_data);

    /* tries to read the next record from a file and returns the number of
       relevant records that were read (usually one). If an error occured,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           in_File - the input file stream the record shall be read from
           recName - name (header) of the next record

       remarks:
           If you actually want to read some data, you have to derive a class
           from ESMReader and set its readNextRecord() function up in a way
           that does read the data records you want to have.
    */
    virtual int readNextRecord(std::ifstream& in_File, const int32_t recName) = 0;
};//class

} //namespace

#endif // SR_ESMREADER_H
