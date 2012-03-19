/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#ifndef SR_LOCATIONREFERENCETYPERECORD_H
#define SR_LOCATIONREFERENCETYPERECORD_H

#include "BasicRecord.h"
#include "BinarySubRecord.h"
#include <string>
#include <vector>

namespace SRTP
{

struct LocationReferenceTypeRecord: public BasicRecord
{
  public:
    /* constructor */
    LocationReferenceTypeRecord();

    /* destructor */
    virtual ~LocationReferenceTypeRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const LocationReferenceTypeRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /* returns the size in bytes that the record's data would occupy in a file
       stream, NOT including the header data
    */
    virtual uint32_t getWriteSize() const;

    /* writes the record to the given output stream and returns true on success

      parameters:
          output   - the output file stream
    */
    virtual bool saveToStream(std::ofstream& output) const;
    #endif

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File);

    /* returns the record's type, usually its header */
    virtual int32_t getRecordType() const;

    std::string editorID;
    //subrecord CNAM
    bool hasCNAM;
    uint8_t colourRed;
    uint8_t colourGreen;
    uint8_t colourBlue;
    uint8_t unused;
    //end of CNAM
}; //struct

} //namespace

#endif // SR_LOCATIONREFERENCETYPERECORD_H
