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

#ifndef SR_ESMWRITERCONTENTS_H
#define SR_ESMWRITERCONTENTS_H

#include "ESMWriter.h"
#include "ESMFileContents.h"

namespace SRTP
{

class ESMWriterContents: public ESMWriter
{
  public:
    /* constructor */
    ESMWriterContents();

    /* destructor */
    virtual ~ESMWriterContents();

    /*the structure that is used to store the data that will be written - the
      application has to fill it before calling writeESM() or the produced ESM
      won't contain any records besides the header */
    ESMFileContents contents;
  protected:
    /* returns the number of records that will be written to the stream, not
       including the header record

       remarks:
           This function has to be implemented by every derived class to match
           the actual number of records.
    */
    virtual uint32_t getTotalNumberOfRecords() const;

    /* returns the number of groups that will be written to the stream

       remarks:
           This function has to be implemented by every derived class to match
           the actual number of groups.
    */
    virtual uint32_t getTotalNumberOfGroups() const;

    /* tries to write all gropus and data records and returns true on success

       parameters:
           output - the output file stream that's used to write the data
    */
    virtual bool writeGroups(std::ofstream& output) const;
};//class

} //namespace

#endif // SR_ESMWRITERCONTENTS_H
