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

#ifndef SR_SOULGEMRECORD_H
#define SR_SOULGEMRECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"
#include "BinarySubRecord.h"

namespace SRTP
{

struct SoulGemRecord: public BasicRecord
{
  public:
    /* constructor */
    SoulGemRecord();

    /* destructor */
    virtual ~SoulGemRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const SoulGemRecord& other) const;
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
    uint8_t unknownOBND[12];
    bool hasFULL;
    uint32_t nameStringID; //subrecord FULL
    std::string modelPath;
    BinarySubRecord unknownMODT;
    std::vector<uint32_t> keywordArray;
    //subrecord DATA
    uint32_t value;
    float weight;
    //end of DATA
    uint8_t soulInside; //subrecord SOUL
    uint8_t capacity; //subrecord SLCP
    bool hasNAM0;
    uint32_t unknownNAM0;

    //constants for capacity / filled stuff
    static const uint8_t cCapacityEmpty;
    static const uint8_t cCapacityPetty;
    static const uint8_t cCapacityLesser;
    static const uint8_t cCapacityCommon;
    static const uint8_t cCapacityGreater;
    static const uint8_t cCapacityAzura;
}; //struct

} //namespace

#endif // SR_SOULGEMRECORD_H
