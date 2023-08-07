/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#ifndef SR_HEADPARTRECORD_HPP
#define SR_HEADPARTRECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

struct HeadPartRecord: public BasicRecord
{
  public:
    /* constructor */
    HeadPartRecord();

    /* destructor */
    virtual ~HeadPartRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const HeadPartRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /* returns the size in bytes that the record's data would occupy in a file
       stream, NOT including the header data
    */
    virtual uint32_t getWriteSize() const;

    /* writes the record to the given output stream and returns true on success

      parameters:
          output   - the output stream
    */
    virtual bool saveToStream(std::ostream& output) const;
    #endif

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File   - the input stream
          localized - whether the file to read from is localized or not
          table     - the associated string table for localized files
    */
    virtual bool loadFromStream(std::istream& in_File, const bool localized, const StringTable& table);

    /* returns the record's type, usually its header */
    virtual uint32_t getRecordType() const;

    //structure for NAM0/NAM1 compounds
    struct NAM0_NAM1_compound
    {
      uint32_t unknownNAM0;
      std::string unknownNAM1;

      //comparison operator
      bool operator==(const NAM0_NAM1_compound& other) const;
    };//struct

    std::string editorID;
    LocalizedString fullName;
    std::string modelPath;
    BinarySubRecord unknownMODT;
    uint8_t unknownDATA;
    uint32_t unknownPNAM;
    std::vector<uint32_t> unknownHNAMs;
    std::vector<NAM0_NAM1_compound> unknownNAM0_NAM1s;
    bool hasTNAM;
    uint32_t unknownTNAM;
    bool hasRNAM;
    uint32_t unknownRNAM;
}; //struct

} //namespace

#endif // SR_HEADPARTRECORD_HPP
