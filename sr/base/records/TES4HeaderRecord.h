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

#ifndef SR_TES4HEADERRECORD_H
#define SR_TES4HEADERRECORD_H

#include <vector>
#include "BasicRecord.h"

namespace SRTP
{

struct Tes4HeaderRecord: public BasicRecord
{
  public:
    /* constructor */
    Tes4HeaderRecord();

    /* destructor */
    virtual ~Tes4HeaderRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const Tes4HeaderRecord& other) const;
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

    /* returns true, if the file is an ESM file, according to the set flags */
    bool isMasterFile() const;

    /* returns true, if the file is localized, according to the set flags */
    bool isLocalized() const;

    //flag constants
    static const uint32_t cMasterFlag    = 0x00000001;
    static const uint32_t cLocalizedFlag = 0x00000080;

    //structure for file dependencies
    struct MasterFile
    {
      std::string fileName;
      int64_t data;

      /* equality operator */
      bool operator==(const MasterFile& other) const;
    };//struct

    //subrecord HEDR
    float version;
    uint32_t numRecordsAndGroups;
    uint32_t nextObjectID;
    //end of HEDR
    std::string authorName; //subrecord CNAM
    std::string summary; //subrecord SNAM
    std::vector<MasterFile> dependencies; //subrecords MAST+DATA
    std::vector<uint32_t> unknownONAM; //subrecord ONAM
    uint32_t unknownIntValue; //subrecord INTV
    bool hasINCC;
    uint32_t unknownINCC;
}; //struct

} //namespace

#endif // SR_TES4HEADERRECORD_H
