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

#ifndef SR_IMAGESPACEMODIFIERRECORD_H
#define SR_IMAGESPACEMODIFIERRECORD_H

#include "BasicRecord.h"
#include <string>
#include <vector>
#include "SubBlock.h"

namespace SRTP
{

struct ImageSpaceModifierRecord: public BasicRecord
{
  public:
    /* constructor */
    ImageSpaceModifierRecord();

    /* destructor */
    virtual ~ImageSpaceModifierRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const ImageSpaceModifierRecord& other) const;
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
    uint8_t unknownDNAM[244];
    BinarySubRecord unknownBNAM;
    BinarySubRecord unknownVNAM;
    BinarySubRecord unknownTNAM;
    BinarySubRecord unknownNAM3;
    BinarySubRecord unknownRNAM;
    BinarySubRecord unknownSNAM;
    BinarySubRecord unknownUNAM;
    BinarySubRecord unknownNAM1;
    BinarySubRecord unknownNAM2;
    BinarySubRecord unknownWNAM;
    BinarySubRecord unknownXNAM;
    BinarySubRecord unknownYNAM;
    BinarySubRecord unknownNAM4;
    BinarySubRecord unknownx00IAD;
    BinarySubRecord unknownx40IAD;
    std::vector<SubBlock> otherIADs;
}; //struct

} //namespace

#endif // SR_IMAGESPACEMODIFIERRECORD_H
