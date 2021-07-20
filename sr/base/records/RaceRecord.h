/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#ifndef SR_RACERECORD_H
#define SR_RACERECORD_H

#include <string>
#include <vector>
#include <stdint.h>
#include "BasicRecord.h"
#include "BinarySubRecord.h"
#include "LocalizedString.h"
#include "SubBlock.h"

namespace SRTP
{

struct RaceRecord: public BasicRecord
{
  public:
    /* constructor */
    RaceRecord();

    /* destructor */
    virtual ~RaceRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const RaceRecord& other) const;
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

    /* returns the length of the DATA record in bytes */
    uint32_t getDataLength() const;

    //type for data subrecord
    struct RaceData
    {
      uint8_t unknown16[16];
      float heightMale;
      float heightFemale;
      float weightMale;
      float weightFemale;
      uint8_t unknown96[96];
      bool has36;
      uint8_t unknown36[36];

      /* constructor */
      RaceData();

      /* comparison operator */
      bool operator==(const RaceData& other) const;

      /* initialisation/ reset data */
      void clear();
    };//struct

    std::string editorID;
    LocalizedString name; //subrecord FULL
    LocalizedString description; //subrecord DESC
    std::vector<uint32_t> spellFormIDs;
    bool hasWNAM;
    uint32_t unknownWNAM;
    BinarySubRecord unknownBODT;
    BinarySubRecord unknownBOD2;
    std::vector<uint32_t> keywordArray;
    RaceData data;
    std::vector<SubBlock> subBlocks;
}; //struct

} //namespace

#endif // SR_RACERECORD_H
