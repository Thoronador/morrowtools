/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012  Thoronador

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

#ifndef SR_LEVELEDCHARACTERRECORD_H
#define SR_LEVELEDCHARACTERRECORD_H

#include <string>
#include <vector>
#include "LeveledListBaseRecord.h"
#include "BinarySubRecord.h"

namespace SRTP
{

struct LeveledCharacterRecord: public LeveledListBaseRecord
{
  public:
    /* constructor */
    LeveledCharacterRecord();

    /* destructor */
    virtual ~LeveledCharacterRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const LeveledCharacterRecord& other) const;
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
    uint8_t unknownOBND[12];
    uint8_t chanceNone; //subrecord LVLD
    uint8_t flags; //subrecord LVLF
    //uint8_t entryCount; //subrecord LLCT
    //std::vector<LeveledListEntry> entries; //subrecords LVLO -> already declared in base class
    std::string modelPath;
    BinarySubRecord unknownMODT;
}; //struct

} //namespace

#endif // SR_LEVELEDCHARACTERRECORD_H
