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

#ifndef SR_CELLRECORD_H
#define SR_CELLRECORD_H

#include "BasicRecord.h"
#include "BinarySubRecord.h"
#include <string>
#include <vector>

namespace SRTP
{

struct CellRecord: public BasicRecord
{
  public:
    /* constructor */
    CellRecord();

    /* destructor */
    virtual ~CellRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const CellRecord& other) const;
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

    //type for XCLC subrecord
    struct SubrecordXCLC
    {
      bool presence;
      int32_t locationX;
      int32_t locationY;
      uint32_t unknownThird;

      /* equality operator */
      bool operator==(const SubrecordXCLC& other) const;
    };

    std::string editorID;
    bool hasFULL;
    uint32_t nameStringID; //subrecord FULL
    BinarySubRecord unknownDATA;
    BinarySubRecord unknownTVDT;
    BinarySubRecord unknownMHDT;
    SubrecordXCLC gridLocation;
    BinarySubRecord unknownXCLL;
    uint32_t unknownLTMP;
    bool hasLNAM;
    uint32_t unknownLNAM;
    float    unknownXCLW;
    std::vector<uint32_t> unknownXCLR;
    bool hasXNAM;
    uint8_t unknownXNAM;
    bool hasXLCN;
    uint32_t unknownXLCN;
    bool hasXWCN;
    uint32_t unknownXWCN;
    bool hasXWCS;
    uint32_t unknownXWCS;
    BinarySubRecord unknownXWCU;
    bool hasXCIM;
    uint32_t unknownXCIM;
    bool hasXEZN;
    uint32_t encounterZoneFormID; //subrecord XEZN
    bool hasXCWT;
    uint32_t unknownXCWT;
    bool hasXCMO;
    uint32_t unknownXCMO;
    std::string unknownXWEM;
    bool hasXOWN;
    uint32_t unknownXOWN;
    bool hasXILL;
    uint32_t unknownXILL;
    bool hasXCCM;
    uint32_t unknownXCCM;
    bool hasXCAS;
    uint32_t unknownXCAS;
}; //struct

} //namespace

#endif // SR_CELLRECORD_H
