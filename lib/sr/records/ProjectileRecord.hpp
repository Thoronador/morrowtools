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

#ifndef SR_PROJECTILERECORD_HPP
#define SR_PROJECTILERECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

struct ProjectileRecord: public BasicRecord
{
  public:
    /* constructor */
    ProjectileRecord();

    /* destructor */
    virtual ~ProjectileRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const ProjectileRecord& other) const;
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

    struct DSTD_DSTF_record
    {
      uint8_t unknownDSTD[20];

      /* equality operator */
      bool operator==(const DSTD_DSTF_record& other) const;
    };//struct

    std::string editorID;
    uint8_t unknownOBND[12];
    LocalizedString name;
    std::string modelPath;
    BinarySubRecord unknownMODT;
    bool hasDEST;
    uint64_t unknownDEST;
    std::vector<DSTD_DSTF_record> unknownDSTD_DSTFs;
    BinarySubRecord unknownDATA;//subrecord DATA
    std::string unknownNAM1;
    BinarySubRecord unknownNAM2;
    uint32_t unknownVNAM;
}; //struct

} //namespace

#endif // SR_PROJECTILERECORD_HPP
