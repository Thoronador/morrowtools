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

#ifndef SR_ALCHEMYPOTIONRECORD_H
#define SR_ALCHEMYPOTIONRECORD_H

#include "BasicRecord.h"
#include "BinarySubRecord.h"
#include "EffectBlock.h"
#include <string>
#include <vector>
#include <stdint.h>

namespace SRTP
{

struct AlchemyPotionRecord: public BasicRecord
{
  public:
    /* constructor */
    AlchemyPotionRecord();

    /* destructor */
    virtual ~AlchemyPotionRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const AlchemyPotionRecord& other) const;

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
    uint8_t unknownOBND[12];
    bool hasFULL;
    uint32_t nameStringID; //subrecord FULL
    std::vector<uint32_t> keywordArray;
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMODS;
    bool hasYNAM;
    uint32_t unknownYNAM;
    bool hasZNAM;
    uint32_t unknownZNAM;
    uint32_t unknownDATA;
    uint8_t unknownENIT[20];
    std::vector<EffectBlock> effects;
}; //struct

} //namespace

#endif // SR_ALCHEMYPOTIONRECORD_H
