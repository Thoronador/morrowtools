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

#ifndef SR_ESMREADERCONTENTSBASE_H
#define SR_ESMREADERCONTENTSBASE_H

#include "ESMReader.h"
#include "ESMFileContents.h"

namespace SRTP
{

class ESMReaderContentsBase: public ESMReader
{
  public:
    /* constructor */
    ESMReaderContentsBase();

    /* destructor */
    virtual ~ESMReaderContentsBase();

    //the structure that is used to store the read data
    ESMFileContents contents;
  protected:
    /* returns true, if the given group may contains some data that the reader
       wants to read. Returns false otherwise.

       parameters:
           g_data - the group header data

       remarks:
           Pure virtual.
    */
    virtual bool needGroup(const GroupData& g_data) const = 0;

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

       remarks:
           Pure virtual.
    */
    virtual int readNextRecord(std::ifstream& in_File, const int32_t recName) = 0;
}; //class

}//namespace

#endif // SR_ESMREADERCONTENTSBASE_H
