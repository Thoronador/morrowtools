/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013 Thoronador

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

#ifndef SR_SHOUTRECORD_H
#define SR_SHOUTRECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"

namespace SRTP
{

struct ShoutRecord: public BasicRecord
{
  public:
    /* constructor */
    ShoutRecord();

    /* destructor */
    virtual ~ShoutRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const ShoutRecord& other) const;
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

    /* wrapper type for SNAM entries */
    struct WordEntry
    {
      uint32_t wordFormID; //form ID of one word for that shout
      uint32_t spellFormID; //form ID of the spell effect for that shout
      float recharge; //recharge time in seconds

      /* equality operator */
      bool operator==(const WordEntry& other) const;
    }; //struct

    std::string editorID;
    bool hasFULL;
    uint32_t fullNameStringID;
    uint32_t menuDisplayObjectFormID; //subrecord MDOB
    uint32_t descriptionStringID;
    std::vector<WordEntry> words; //subrecords SNAM
}; //struct

} //namespace

#endif // SR_SHOUTRECORD_H
