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

#ifndef SR_WATERTYPERECORD_H
#define SR_WATERTYPERECORD_H

#include "BasicRecord.h"
#include "BinarySubRecord.h"
#include <string>
#include <vector>
#include <stdint.h>

namespace SRTP
{

struct WaterTypeRecord: public BasicRecord
{
  public:
    /* constructor */
    WaterTypeRecord();

    /* destructor */
    virtual ~WaterTypeRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const WaterTypeRecord& other) const;

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
    bool hasFULL;
    uint32_t unknownFULL; //subrecord FULL
    std::vector<std::string> unknownNNAMs;
    uint8_t unknownANAM;
    uint8_t unknownFNAM;
    bool hasMNAM;
    uint8_t unknownMNAM;
    bool hasSNAM;
    uint32_t unknownSNAM;
    bool hasTNAM;
    uint32_t unknownTNAM;
    uint16_t unknownDATA;
    BinarySubRecord unknownDNAM;
    uint8_t unknownGNAM[12];
    bool hasNAM0;
    uint8_t unknownNAM0[12];
    bool hasNAM1;
    uint8_t unknownNAM1[12];
}; //struct

} //namespace

#endif // SR_WATERTYPERECORD_H
