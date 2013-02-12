/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef SR_FURNITURERECORD_H
#define SR_FURNITURERECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"
#include "BinarySubRecord.h"
#include "DestructionData.h"

namespace SRTP
{

struct FurnitureRecord: public BasicRecord
{
  public:
    /* constructor */
    FurnitureRecord();

    /* destructor */
    virtual ~FurnitureRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const FurnitureRecord& other) const;
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
    BinarySubRecord unknownOBND;
    bool hasFULL;
    uint32_t nameStringID; //subrecord FULL
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMODS;
    DestructionData destruction;
    std::vector<uint32_t> keywords;
    uint32_t unknownPNAM;
    uint16_t unknownFNAM;
    uint32_t interactionKeywordFormID; //subrecord KNAM
    uint32_t unknownMNAM;
    uint16_t unknownWBDT;
    std::vector<uint32_t> unknownENAMs;
    std::vector<uint32_t> unknownNAM0s;
    std::vector<uint32_t> unknownFNMKs;
    std::vector<uint32_t> unknownFNPRs;
    std::string markerModelPath;
}; //struct

} //namespace

#endif // SR_FURNITURERECORD_H
