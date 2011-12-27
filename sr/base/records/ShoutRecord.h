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

    /* returns true, if the other record contains the same data */
    bool equals(const ShoutRecord& other) const;

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

    /* wrapper type for SNAM entries */
    struct SNAMentry
    {
      uint32_t wordFormID; //form ID of one word for that shout
      uint8_t  dataEightBytes[8];

      /* equality operator */
      bool operator==(const SNAMentry& other) const;
    }; //struct

    std::string editorID;
    bool hasFULL;
    uint32_t fullNameStringID;
    bool hasMDOB;
    uint32_t unknownMDOB;
    uint32_t descriptionStringID;
    std::vector<SNAMentry> unknownSNAMs;
}; //struct

} //namespace

#endif // SR_SHOUTRECORD_H
