/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#ifndef MW_ESMWRITER_HPP
#define MW_ESMWRITER_HPP

#include <string>
#include "DepFiles.hpp"
#include "records/TES3Record.hpp"

/*---------------------------------------------------------------------------

  To do:
  ======
    - enforce proper file extension (.esm or .esp) depending on given params

  --------------------------------------------------------------------------*/

namespace MWTP
{

/* ESMWriter class

   This is the basic class to write ESM (and ESP, too) files to a certain file.
   It just implements the write process of the header - the rest has to be done
   by deriving from this class and implementing the function getTotalRecords()
   and writeRecords(), which do the actual work.
*/
class ESMWriter
{
  public:
    /* constructor */
    ESMWriter();

    /* destructor */
    virtual ~ESMWriter();

    /* tries to write an .esm/.esp file and returns true on success, false on
       failure.

       parameters:
           FileName     - name of the .esm/.esp file
           IsMasterFile - if true, the file will be treated as master file
           headerData   - the TES3Record containing the list of dependency files
                          and the description of the file. Note that this record
                          might get changed slightly during the writing process.
    */
    bool writeESM(const std::string& FileName, const bool IsMasterFile, TES3Record& headerData);

    /* presets the data members of the given TES3Record to match the default
       data that was used in previous versions of ESMWriter's writeESM()
       function. Use of that function should be considered deprecated.

       parameters:
           headerData   - the TES3Record whose data shall be set
           IsMasterFile - if true, the file will be treated as master file
           deps         - the list of dependency files
           Description  - the description of the file that will be placed in the
                          file header

    */
    static void setOldStyleHeaderData(TES3Record& headerData, const bool IsMasterFile, const DepFileList& deps, const std::string& Description);
  protected:
    /* returns the number of records that will be written to the stream */
    virtual int32_t getTotalRecords() const = 0;

    /* tries to write all data records and returns true on success

       parameters:
           output - the output file stream that's used to write the records
    */
    virtual bool writeRecords(std::ofstream& output) const = 0;
};//class

} //namespace

#endif // MW_ESMWRITER_HPP
