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

#ifndef SR_SOULGEMRECORD_HPP
#define SR_SOULGEMRECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

struct SoulGemRecord: public BasicRecord
{
  public:
    /* constructor */
    SoulGemRecord();

    /* destructor */
    virtual ~SoulGemRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const SoulGemRecord& other) const;
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

    /* constants for capacity / filled stuff */
    static const uint8_t cCapacityEmpty;
    static const uint8_t cCapacityPetty;
    static const uint8_t cCapacityLesser;
    static const uint8_t cCapacityCommon;
    static const uint8_t cCapacityGreater;
    static const uint8_t cCapacityAzura;

    std::string editorID;
    uint8_t unknownOBND[12];
    LocalizedString name; //subrecord FULL
    std::string modelPath;
    BinarySubRecord unknownMODT;
    std::vector<uint32_t> keywordArray;
    //subrecord DATA
    uint32_t value;
    float weight;
    //end of DATA
    uint8_t soulInside; //subrecord SOUL
    uint8_t capacity; //subrecord SLCP
    uint32_t linkedToFormID; //subrecord NAM0
}; //struct

} //namespace

#endif // SR_SOULGEMRECORD_HPP