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

#ifndef SR_ARMOURRECORD_HPP
#define SR_ARMOURRECORD_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

struct ArmourRecord: public BasicRecord
{
  public:
    /* constructor */
    ArmourRecord();

    /* destructor */
    virtual ~ArmourRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const ArmourRecord& other) const;
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

    std::string editorID;
    BinarySubRecord unknownVMAD;
    uint8_t unknownOBND[12];
    LocalizedString name; //subrecord FULL
    uint32_t enchantingFormID; //subrecord EITM
    std::string modelPath;
    BinarySubRecord unknownMO2T;
    BinarySubRecord unknownMO2S;
    std::string mod4Path;
    BinarySubRecord unknownMO4T;
    BinarySubRecord unknownMO4S;
    BinarySubRecord unknownBODT;
    BinarySubRecord unknownBOD2;
    uint32_t equipTypeFormID; //subrecord ETYP
    uint32_t blockBashImpactDataSetFormID; //subrecord BIDS
    uint32_t alternateBlockMaterialFormID; //subrecord BAMT
    uint32_t pickupSoundFormID; //subrecord YNAM
    uint32_t putdownSoundFormID; //subrecord ZNAM
    uint32_t unknownRNAM;
    std::vector<uint32_t> keywordArray;
    LocalizedString description; //subrecord DESC
    std::vector<uint32_t> models; //subrecords MODL
    //subrecor DATA
    uint32_t value;
    float weight;
    //end of DATA
    uint32_t unknownDNAM;
    uint32_t templateArmorFormID; //subrecord TNAM
}; //struct

} //namespace

#endif // SR_ARMOURRECORD_HPP
