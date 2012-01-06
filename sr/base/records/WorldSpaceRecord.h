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

#ifndef SR_WORLDSPACERECORD_H
#define SR_WORLDSPACERECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"
#include "BinarySubRecord.h"

namespace SRTP
{

struct WorldSpaceRecord: public BasicRecord
{
  public:
    /* constructor */
    WorldSpaceRecord();

    /* destructor */
    virtual ~WorldSpaceRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const WorldSpaceRecord& other) const;

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
    std::vector<BinarySubRecord> unknownRNAMs;
    BinarySubRecord unknownMHDT;
    bool hasFULL;
    uint32_t nameStringID; //subrecord FULL
    bool hasWCTR;
    uint32_t unknownWCTR;
    bool hasLTMP;
    uint32_t unknownLTMP;
    bool hasXEZN;
    uint32_t unknownXEZN;
    bool hasCNAM;
    uint32_t unknownCNAM;
    bool hasNAM2;
    uint32_t unknownNAM2;
    bool hasNAM3;
    uint32_t unknownNAM3;
    bool hasNAM4;
    uint32_t unknownNAM4;
    bool hasDNAM;
    uint64_t unknownDNAM;
    BinarySubRecord unknownMNAM;
    bool hasXLCN;
    uint32_t unknownXLCN;
    bool hasWNAM;
    uint32_t unknownWNAM;
    bool hasPNAM;
    uint16_t unknownPNAM;
    BinarySubRecord unknownONAM;
    uint32_t unknownNAMA;
    bool hasDATA;
    uint8_t unknownDATA;
    bool hasNAM0;
    uint64_t unknownNAM0;
    bool hasNAM9;
    uint64_t unknownNAM9;
    bool hasZNAM;
    uint32_t unknownZNAM;
    std::string pathTNAM;
    std::string pathUNAM;
    bool hasXXXX;
    uint32_t unknownXXXX;
    BinarySubRecord unknownOFST;
}; //struct

} //namespace

#endif // SR_WORLDSPACERECORD_H
