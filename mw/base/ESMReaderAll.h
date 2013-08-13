/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

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

#ifndef MW_ESMREADERALL_H
#define MW_ESMREADERALL_H

#include "ESMReader.h"

namespace MWTP
{

/* This descendant of the ESMReader class tries to read all records from the
   given .esm/.esp file and uses the appropriate singleton classes to store the
   read data records.
*/
class ESMReaderAll: public ESMReader
{
  public:
    /* constructor */
    ESMReaderAll();

    /* destructor */
    virtual ~ESMReaderAll();
  protected:
    /* tries to read the next record from a file and returns the number of
       relevant records that were read (usually one). If an error occured,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           in_File  - the input file stream the record shall be read from
    */
    virtual int processNextRecord(std::ifstream& in_File);
};//class

} //namespace

#endif // MW_ESMREADERALL_H
