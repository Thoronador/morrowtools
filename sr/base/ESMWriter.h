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

#ifndef SR_ESMWRITER_H
#define SR_ESMWRITER_H

#include <string>
#include <stdint.h>
#include "records/TES4HeaderRecord.h"

namespace SRTP
{

/* ESMWriter class

   This is the basic class to write ESM (and ESP, too) files to a certain file.
   It just implements the write process of the header - the rest has to be done
   by deriving from this class and implementing the functions which do the
   actual work.
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
           FileName  - name of the .esm/.esp file
           theHeader - the header record that contains all relevant header info
                       for the file
    */
    bool writeESM(const std::string& FileName, Tes4HeaderRecord& theHeader);

  protected:
    /* returns the number of records that will be written to the stream, not
       including the header record

       remarks:
           This function has to be implemented by every derived class to match
           the actual number of records.
    */
    virtual uint32_t getTotalNumberOfRecords() const = 0;

    /* returns the number of groups that will be written to the stream

       remarks:
           This function has to be implemented by every derived class to match
           the actual number of groups.
    */
    virtual uint32_t getTotalNumberOfGroups() const = 0;

    /* tries to write all gropus and data records and returns true on success

       parameters:
           output - the output file stream that's used to write the data
    */
    virtual bool writeGroups(std::ofstream& output) const = 0;
};//class

} //namespace

#endif // SR_ESMWRITER_H
