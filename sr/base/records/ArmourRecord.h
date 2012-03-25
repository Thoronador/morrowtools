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

#ifndef SR_ARMOURRECORD_H
#define SR_ARMOURRECORD_H

#include "BasicRecord.h"
#include "BinarySubRecord.h"
#include <string>
#include <vector>
#include <stdint.h>

namespace SRTP
{

struct ArmourRecord: public BasicRecord
{
  public:
    /* constructor */
    ArmourRecord();

    /* destructor */
    virtual ~ArmourRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const ArmourRecord& other) const;
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
    virtual uint32_t getRecordType() const;

    std::string editorID;
    BinarySubRecord unknownVMAD;
    uint8_t unknownOBND[12];
    bool hasFULL;
    uint32_t nameStringID; //subrecord FULL
    bool hasEITM;
    uint32_t unknownEITM;
    std::string modelPath;
    BinarySubRecord unknownMO2T;
    BinarySubRecord unknownMO2S;
    std::string mod4Path;
    BinarySubRecord unknownMO4T;
    BinarySubRecord unknownMO4S;
    BinarySubRecord unknownBODT;
    bool hasETYP;
    uint32_t unknownETYP;
    bool hasBIDS;
    uint32_t unknownBIDS;
    bool hasBAMT;
    uint32_t unknownBAMT;
    bool hasYNAM;
    uint32_t unknownYNAM;
    bool hasZNAM;
    uint32_t unknownZNAM;
    uint32_t unknownRNAM;
    std::vector<uint32_t> keywordArray;
    bool hasDESC;
    uint32_t descriptionStringID; //subrecord DESC
    std::vector<uint32_t> unknownMODLs;
    uint8_t unknownDATA[8];
    uint32_t unknownDNAM;
    bool hasTNAM;
    uint32_t unknownTNAM;
}; //struct

} //namespace

#endif // SR_ARMOURRECORD_H
