/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

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

#ifndef ESMREADER_H
#define ESMREADER_H

#include <string>
#include <fstream>
#include "DepFiles.h"

namespace MWTP
{

/* ESMReader class

   This is the basic class for reading master (.esm) and plugin (.esp) files of
   Morrowind. The ESMReader class reads the header of the file; all subsequent
   records will be read by processNextRecord() function. In this class, the
   function just skips all records and reads no real data.
   To change that behaviour and actually read some data, you have to derive a
   new class from ESMReader and reimplement the protected processNextRecord()
   function, which has to take care that the required data is read and not
   skipped.
*/

class ESMReader
{
  public:
    /* constructor */
    ESMReader();

    /* destructor */
    virtual ~ESMReader();

    /* skips a record and returns 0 on success, and -1 on error */
    static int skipRecord(std::ifstream& in_File);

    /* tries to read an .esm/.esp file and returns the number of relevant
       records that were read. If an error occured, -1 is returned.

       parameters:
           FileName - name of the .esm/.esp file
           deps     - the list that will be used to store the dependency files
           verbose  - if true, some additional information will be written to
                      standard output
    */
    int readESM(const std::string& FileName, DepFileList& deps, const bool verbose);

  protected:
    /* tries to read the next record from a file and returns the number of
       relevant records that were read (usually one). If an error occured,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           in_File  - the input file stream the record shall be read from

       remarks:
           If you actually want to read some data, you have to derive a class
           from ESMReader and set its processNextRecord() function up in a way
           that does not just skip all data records - because that is what the
           implementation here in ESMReader basically does.
           So naturally, this function will never return values larger than
           zero.
    */
    virtual int processNextRecord(std::ifstream& in_File);
};//class

} //namespace

#endif // ESMREADER_H
