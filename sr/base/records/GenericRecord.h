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

#ifndef SR_GENERICRECORD_H
#define SR_GENERICRECORD_H

#include "BasicRecord.h"

namespace SRTP
{

struct GenericRecord: public BasicRecord
{
  public:
    /* constructor */
    GenericRecord();

    /* copy constructor */
    GenericRecord(const GenericRecord& other);

    /* assignment operator */
    GenericRecord& operator=(const GenericRecord& other);

    /* destructor */
    virtual ~GenericRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const GenericRecord& other) const;
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
          in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File);

    /* returns true, if the record is a generic record (see GenericRecord.h) */
    virtual bool isGenericRecord() const;

    /* returns the record's type, usually its header */
    virtual uint32_t getRecordType() const;

    /* returns a pointer to the record data

       remarks:
           Can be NULL; if no data was read yet.
    */
    const unsigned char* getDataPointer() const;

    /* returns the size of the data in bytes */
    uint32_t getDataSize() const;

    /* holds the header for the record */
    uint32_t Header;
  protected:
    uint32_t m_DataSize;
    unsigned char* m_Data;
}; //struct

} //namespace

#endif // SR_GENERICRECORD_H
