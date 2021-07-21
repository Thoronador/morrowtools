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

#ifndef SR_CELLRECORD_HPP
#define SR_CELLRECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

struct CellRecord: public BasicRecord
{
  public:
    /* constructor */
    CellRecord();

    /* destructor */
    virtual ~CellRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const CellRecord& other) const;
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
          in_File   - the input file stream
          localized - whether the file to read from is localized or not
          table     - the associated string table for localized files
    */
    virtual bool loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table);

    /* returns the record's type, usually its header */
    virtual uint32_t getRecordType() const;

    //type for XCLC subrecord
    struct SubrecordXCLC
    {
      bool presence;
      int32_t locationX;
      int32_t locationY;
      uint32_t unknownThird;

      /* constructor */
      SubrecordXCLC();

      /* equality operator */
      bool operator==(const SubrecordXCLC& other) const;
    };

    std::string editorID;
    LocalizedString name; //subrecord FULL
    BinarySubRecord unknownDATA;
    BinarySubRecord unknownTVDT;
    BinarySubRecord unknownMHDT;
    SubrecordXCLC gridLocation;
    BinarySubRecord unknownXCLL;
    uint32_t lightingTemplateFormID; //subrecord LTMP
    bool hasLNAM;
    uint32_t unknownLNAM;
    float    unknownXCLW;
    std::vector<uint32_t> unknownXCLR;
    bool hasXNAM;
    uint8_t unknownXNAM;
    uint32_t locationFormID; //subrecord XLCN
    bool hasXWCN;
    uint32_t unknownXWCN;
    bool hasXWCS;
    uint32_t unknownXWCS;
    BinarySubRecord unknownXWCU;
    uint32_t imageSpaceFormID; //subrecord XCIM
    uint32_t encounterZoneFormID; //subrecord XEZN
    bool hasXCWT;
    uint32_t unknownXCWT;
    uint32_t musicTypeFormID; //subrecord XCMO
    std::string unknownXWEM;
    uint32_t ownerFactionFormID; //subrecord XOWN
    uint32_t lockListFormID; //subrecord XILL
    uint32_t regionFormID; //subrecord XCCM
    uint32_t defaultAcousticSpaceFormID; //subrecord CAS
}; //struct

} //namespace

#endif // SR_CELLRECORD_HPP
