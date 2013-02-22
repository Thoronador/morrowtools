/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#ifndef SR_ESMREADERCONVCAMS_H
#define SR_ESMREADERCONVCAMS_H

#include "../base/ESMReaderContentsBase.h"

namespace SRTP
{

/* This ESMReader class reads all records except CAMS and NAVI as generic
   records.
*/
class ESMReaderConvCAMS: public ESMReaderContentsBase
{
  public:
    /* constructor */
    ESMReaderConvCAMS();

    /* destructor */
    virtual ~ESMReaderConvCAMS();
  protected:
    /* returns true, if the given group may contains some data that the reader
       wants to read. Returns false otherwise.

       parameters:
           g_data - the group header data

       remarks:
           Returns true for all groups.
    */
    virtual bool needGroup(const GroupData& g_data) const;

    /* tries to read the next record from a file and returns the number of
       relevant records that were read (usually one). If an error occured,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           in_File   - the input file stream the record shall be read from
           recName   - name (header) of the next record
           localized - true, if the data in the stream is localized
           table     - in case of localized data: the string table
    */
    virtual int readNextRecord(std::ifstream& in_File, const uint32_t recName, const bool localized, const StringTable& table);
}; //class

}//namespace

#endif // SR_ESMREADERCONVCAMS_H
