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

#ifndef SR_MATERIALOBJECTRECORD_HPP
#define SR_MATERIALOBJECTRECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"

namespace SRTP
{

struct MaterialObjectRecord: public BasicRecord
{
  public:
    /* constructor */
    MaterialObjectRecord();

    /* destructor */
    virtual ~MaterialObjectRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const MaterialObjectRecord& other) const;
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
    std::string modelPath;
    BinarySubRecord unknownMODT;
    std::vector<BinarySubRecord> unknownDNAMs;

    //subrecord DATA
    float falloffScale;
    float falloffBias;
    float noise_UV_Scale;
    float material_UV_Scale;
    float directionalProjectionVectorX;
    float directionalProjectionVectorY;
    float directionalProjectionVectorZ;
    float normalDampener;
    float singlePassColorR; // (*1/255, i.e. "real" value in CK is 255 times that)
    float singlePassColorG; // (*1/255, i.e. "real" value in CK  is 255 times that)
    float singlePassColorB; // (*1/255, i.e. "real" value in CK  is 255 times that)
    bool singlePass; // internally represented as int in ESM files: 0=false, 1=true
    //end of subrecord DATA

    /* returns the length of the DATA subrecord, according to the record version
       given in the record's header
    */
    uint16_t getExpectedDataLength() const;
}; //struct

} //namespace

#endif // SR_MATERIALOBJECTRECORD_HPP
