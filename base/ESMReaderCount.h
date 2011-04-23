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

#ifndef ESMREADERCOUNT_H
#define ESMREADERCOUNT_H

#include "ESMReader.h"
#include <map>

/* This descendant of the ESMReader class tries to skip all records from the
   given .esm/.esp file and just counts how often each record type occured.
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
    /* tries to read the next record from a file and returns the number of
       relevant records that were read (usually one). If an error occured,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           in_File  - the input file stream the record shall be read from

       remarks:
           Naturally, this implementation of processNextRecord() will never
           return values larger than zero, because all record data is skipped.
    */
    virtual int processNextRecord(std::ifstream& in_File);
};//class

#endif // ESMREADERCOUNT_H
