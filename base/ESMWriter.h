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

#ifndef ESMWRITER_H
#define ESMWRITER_H

#include <string>
#include "DepFiles.h"

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
           deps         - the list of dependency files
           Description  - the description of the file that will be placed in the
                          file header
    */
    bool writeESM(const std::string& FileName, const bool IsMasterFile, const DepFileList& deps, const std::string& Description);

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

#endif // ESMWRITER_H
