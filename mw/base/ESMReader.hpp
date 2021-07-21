/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012 Thoronador

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

#ifndef MW_ESMREADER_HPP
#define MW_ESMREADER_HPP

#include <string>
#include <fstream>
#include "DepFiles.hpp"
#include "records/TES3Record.hpp"

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
       records that were read. If an error occurred, -1 is returned. Note that
       this function is deprecated, readESM() should be used instead.

       parameters:
           FileName - name of the .esm/.esp file
           deps     - the list that will be used to store the dependency files
           verbose  - if true, some additional information will be written to
                      standard output
    */
    int oldStyleReadESM(const std::string& FileName, DepFileList& deps, const bool verbose);

    /* tries to read an .esm/.esp file and returns the number of relevant
       records that were read. If an error occurred, -1 is returned.

       parameters:
           FileName - name of the .esm/.esp file
           theHead  - the TES3Record that will be used to store the header data
           verbose  - if true, some additional information will be written to
                      standard output
    */
    int readESM(const std::string& FileName, TES3Record& theHead, const bool verbose);

    /* tries to read the TES3 record of an .esm/.esp file and returns true in
       case of success, false on failure.

       parameters:
           FileName - name of the .esm/.esp file
           theHead  - the TES3Record that will be used to store the header data
    */
    static bool peekESMHeader(const std::string& FileName, TES3Record& theHead);

  protected:
    /* tries to read the next record from a file and returns the number of
       relevant records that were read (usually one). If an error occurred,
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

#endif // MW_ESMREADER_HPP
