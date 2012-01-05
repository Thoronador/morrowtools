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

#ifndef SR_LOCATIONRECORD_H
#define SR_LOCATIONRECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"
#include "BinarySubRecord.h"

namespace SRTP
{

struct LocationRecord: public BasicRecord
{
  public:
    /* constructor */
    LocationRecord();

    /* destructor */
    virtual ~LocationRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const LocationRecord& other) const;

    /* writes the record to the given output stream and returns true on success

      parameters:
          output   - the output file stream
    */
    virtual bool saveToStream(std::ofstream& output) const;

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File);

    /* returns the record's type, usually its header */
    virtual int32_t getRecordType() const;

    /* returns the size in bytes that the record's data would occupy in a file
       stream, NOT including the header data
    */
    virtual uint32_t getWriteSize() const;

    std::string editorID;
    BinarySubRecord unknownLCSR;
    BinarySubRecord unknownLCPR;
    std::vector<BinarySubRecord> unknownLCECs;
    BinarySubRecord unknownLCEP;
    BinarySubRecord unknownLCUN;
    BinarySubRecord unknownLCID;
    bool hasFULL;
    uint32_t nameStringID; //subrecord FULL
    std::vector<uint32_t> keywordArray;
    bool hasPNAM;
    uint32_t unknownPNAM;
    bool hasFNAM;
    uint32_t unknownFNAM;
    bool hasMNAM;
    uint32_t unknownMNAM;
    bool hasRNAM;
    uint32_t unknownRNAM;
    bool hasNAM0;
    uint32_t unknownNAM0;
    bool hasCNAM;
    uint32_t unknownCNAM;
    bool hasNAM1;
    uint32_t unknownNAM1;
}; //struct

} //namespace

#endif // SR_LOCATIONRECORD_H
