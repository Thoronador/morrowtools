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

#ifndef SR_SPELLRECORD_H
#define SR_SPELLRECORD_H

#include "BasicRecord.h"
#include "EffectBlock.h"
#include <string>
#include <stdint.h>

namespace SRTP
{

struct SpellRecord: public BasicRecord
{
  public:
    /* constructor */
    SpellRecord();

    /* destructor */
    virtual ~SpellRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const SpellRecord& other) const;

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
    bool hasMDOB;
    uint32_t unknownMDOB;
    uint32_t unknownETYP;
    uint32_t descriptionStringID; //subrecord DESC
    uint8_t unknownSPIT[36];
    std::vector<EffectBlock> effects;
    std::string unknownCIS2;
}; //struct

} //namespace

#endif // SR_SPELLRECORD_H
