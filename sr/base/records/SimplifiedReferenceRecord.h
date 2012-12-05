/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#ifndef SR_SIMPLIFIEDREFERENCERECORD_H
#define SR_SIMPLIFIEDREFERENCERECORD_H

#include "BasicRecord.h"

namespace SRTP
{

struct SimplifiedReferenceRecord: public BasicRecord
{
  public:
    /* constructor */
    SimplifiedReferenceRecord();

    /* destructor */
    virtual ~SimplifiedReferenceRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const SimplifiedReferenceRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /* returns the size in bytes that the record's data would occupy in a file
       stream, NOT including the header data

       remarks:
           This function will always return zero.
           See the remarks on saveToStream() for a further explanation.
    */
    virtual uint32_t getWriteSize() const;

    /* writes the record to the given output stream and returns true on success

      parameters:
          output   - the output file stream

      remarks:
          This function will always return false, because the
           SimplifiedReferenceRecord class cannot be saved to stream. Saving the
           incomplete data that is available in this record would result in an
           unreadable or invalid record, at least for the original Skyrim
           engine. Therefore, saveToStream() does nothing and returns false.
    */
    virtual bool saveToStream(std::ofstream& output) const;
    #endif

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File);

    /* returns the record's type, usually its header */
    virtual uint32_t getRecordType() const;

    uint32_t baseObjectFormID;
}; //struct

} //namespace

#endif // SR_SIMPLIFIEDREFERENCERECORD_H
