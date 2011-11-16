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

#ifndef SR_ESMREADERWEAPONS_H
#define SR_ESMREADERWEAPONS_H

#include "ESMReader.h"

namespace SRTP
{

/* This descendant of the ESMReader class tries to read all weapon records from
   the given .esm/.esp file.
*/
class ESMReaderWeapons: public ESMReader
{
  public:
    /* constructor */
    ESMReaderWeapons();

    /* destructor */
    virtual ~ESMReaderWeapons();
  protected:
    /* returns true, if the given group may contains some data that the reader
       wants to read. Returns false otherwise.

       parameters:
           g_date - the group header data

       remarks:
           Returns true for all weapon groups, false for any other group.
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
    */
    virtual int readGroup(std::ifstream& in_File, const GroupData& g_data);

    /* tries to read the next record from a file and returns the number of
       relevant records that were read (usually one). If an error occured,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           in_File  - the input file stream the record shall be read from
    */
    virtual int readNextRecord(std::ifstream& in_File);
};//class

} //namespace


#endif // SR_ESMREADERWEAPONS_H
