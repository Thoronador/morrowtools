/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef MW_ESMREADERCOUNT_HPP
#define MW_ESMREADERCOUNT_HPP

#include "ESMReader.hpp"
#include <map>

namespace MWTP
{

/* This descendant of the ESMReader class tries to skip all records from the
   given .esm/.esp file and just counts how often each record type occurred.
   Hence both processNextRecord() and readESM() will never return values larger
   than zero with this class.
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

    /* puts the current statistics to standard output

       parameters:
           withPercentage - if set to true, the output will also show the
                            percentage of each record type, measured on the
                            total number of records
    */
    void showStats(const bool withPercentage=false) const;

  protected:
    /* tries to read the next record from a stream and returns the number of
       relevant records that were read (usually one). If an error occurred,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           in_File  - the input stream the record shall be read from

       remarks:
           Naturally, this implementation of processNextRecord() will never
           return values larger than zero, because all record data is skipped.
    */
    virtual int processNextRecord(std::istream& in_File) override;
};//class

} //namespace

#endif // MW_ESMREADERCOUNT_HPP
