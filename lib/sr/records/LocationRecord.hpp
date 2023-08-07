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

#ifndef SR_LOCATIONRECORD_HPP
#define SR_LOCATIONRECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

struct LocationRecord: public BasicRecord
{
  public:
    /* constructor */
    LocationRecord();

    /* destructor */
    virtual ~LocationRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const LocationRecord& other) const;
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

    /* returns true, if the record version uses ACxx subrecords instead of
       LCxx subrecords
    */
    bool useACxx() const;

    std::string editorID;
    BinarySubRecord unknownLCSR;
    BinarySubRecord unknownLCPR;
    std::vector<BinarySubRecord> unknownLCECs;
    BinarySubRecord unknownLCEP;
    BinarySubRecord unknownLCUN;
    BinarySubRecord unknownLCID;
    LocalizedString name; //subrecord FULL
    std::vector<uint32_t> keywordArray;
    bool hasPNAM;
    uint32_t unknownPNAM;
    bool hasFNAM;
    uint32_t unknownFNAM;
    bool hasMNAM;
    uint32_t unknownMNAM;
    bool hasRNAM;
    uint32_t unknownRNAM;
    bool hasNAM0;
    uint32_t unknownNAM0;
    bool hasCNAM;
    uint32_t unknownCNAM;
    bool hasNAM1;
    uint32_t unknownNAM1;
}; //struct

} //namespace

#endif // SR_LOCATIONRECORD_HPP
