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

#ifndef SR_BOOKRECORD_H
#define SR_BOOKRECORD_H

#include "BasicRecord.h"
#include "BinarySubRecord.h"
#include <string>
#include <vector>
#include <stdint.h>

namespace SRTP
{

struct BookRecord: public BasicRecord
{
  public:
    /* constructor */
    BookRecord();

    /* destructor */
    virtual ~BookRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const BookRecord& other) const;
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
    BinarySubRecord unknownVMAD;
    uint8_t unknownOBND[12];
    bool hasFULL;
    uint32_t titleStringID; //subrecord FULL
    std::string modelPath;
    BinarySubRecord unknownMODT;
    uint32_t textStringID; //DESC
    bool hasYNAM;
    uint32_t unknownYNAM;
    std::vector<uint32_t> keywordArray;
    //subrecord DATA
    uint32_t bookFlags;
    uint32_t spellOrSkillID;
    uint32_t bookValue;
    float weight;
    //end of subrecord DATA
    bool hasINAM;
    uint32_t unknownINAM;
    uint32_t unknownCNAM;

    //flag constants
    static const uint32_t cSkillBookFlag;
    static const uint32_t cSpellTomeFlag;
    static const uint32_t cNoteOrScrollTypeFlag;

    /* returns true, if the book is a skill book, according to its flags */
    bool isSkillBook() const;

    /* returns true, if the book is a spell tome, according to its flags */
    bool isSpellTome() const;
}; //struct

} //namespace

#endif // SR_BOOKRECORD_H
