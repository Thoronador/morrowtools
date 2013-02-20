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

#ifndef SR_DEBRISRECORD_H
#define SR_DEBRISRECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"
#include "BinarySubRecord.h"

namespace SRTP
{

struct DebrisRecord: public BasicRecord
{
  public:
    /* constructor */
    DebrisRecord();

    /* destructor */
    virtual ~DebrisRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const DebrisRecord& other) const;
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

    // structure for the DATA subrecord
    struct subrecordDATA
    {
      uint8_t percentage;
      std::string modelPath;
      uint8_t flags;

      /* returns true, if the collision data flag is set */
      bool hasCollisionData() const;

      /* comparison operator */
      bool operator==(const subrecordDATA& other) const;

      /* resets all data members */
      void clear();
    };//struct

    struct DATA_MODT_compound
    {
      subrecordDATA data;
      BinarySubRecord unknownMODT;

      /* comparison operator */
      bool operator==(const DATA_MODT_compound& other) const;
    };//struct

    std::string editorID;
    std::vector<DATA_MODT_compound> unknownDATA_MODTs;
}; //struct

} //namespace

#endif // SR_DEBRISRECORD_H
