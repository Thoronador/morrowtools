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

#ifndef SR_WORLDSPACERECORD_HPP
#define SR_WORLDSPACERECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "BinarySubRecordExtended.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

struct WorldSpaceRecord: public BasicRecord
{
  public:
    /* constructor */
    WorldSpaceRecord();

    /* destructor */
    virtual ~WorldSpaceRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const WorldSpaceRecord& other) const;
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

    std::string editorID;
    std::vector<BinarySubRecord> unknownRNAMs;
    BinarySubRecord unknownMHDT;
    LocalizedString name; //subrecord FULL
    //subrecord WCTR
    bool hasWCTR;
    int16_t centerCellX;
    int16_t centerCellY;
    //end of subrecord WCTR
    uint32_t interiorLightingFormID; //subrecord LTMP
    uint32_t encounterZoneFormID; //subrecord XEZN
    uint32_t climateFormID; //subrecord CNAM
    uint32_t waterFormID; //subrecord NAM2
    uint32_t LODWaterTypeFormID; //subrecord NAM3
    bool hasNAM4;
    float LODWaterHeight; //subrecord NAM4
    bool hasDNAM;
    uint64_t unknownDNAM;
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMNAM;
    uint32_t locationFormID; //subrecord XLCN
    uint32_t parentWorldSpaceFormID; //subrecord WNAM
    bool hasPNAM;
    uint16_t unknownPNAM;
    BinarySubRecord unknownONAM;
    float distantLODMultiplier; //subrecord NAMA
    bool hasDATA;
    uint8_t unknownDATA;
    bool hasNAM0;
    uint64_t unknownNAM0;
    bool hasNAM9;
    uint64_t unknownNAM9;
    uint32_t musicFormID; //subrecord ZNAM
    std::string HD_LOD_DiffuseTexture; //subrecord TNAN
    std::string HD_LOD_NormalTexture; //subrecord UNAM
    std::string unknownXWEM; //subrecord XWEM
    BinarySubRecordExtended unknownOFST;
}; //struct

} //namespace

#endif // SR_WORLDSPACERECORD_HPP
