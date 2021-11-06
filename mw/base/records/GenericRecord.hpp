/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013  Thoronador

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

#ifndef MW_GENERICRECORD_HPP
#define MW_GENERICRECORD_HPP

#include "BasicRecord.hpp"

namespace MWTP
{

struct GenericRecord: public BasicRecord
{
  public:
    /* constructor */
    GenericRecord();

    /* copy constructor */
    GenericRecord(const GenericRecord& op);

    /* assignment operator */
    GenericRecord& operator=(const GenericRecord& other);

    /* destructor */
    virtual ~GenericRecord();

    #ifndef MW_UNSAVEABLE_RECORDS
    /* writes the record to the given output stream and returns true on success

      parameters:
          output - the output stream
    */
    bool saveToStream(std::ostream& output) const override;
    #endif

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input stream
    */
    bool loadFromStream(std::istream& in_File) override;

    /* returns a pointer to the record data

       remarks:
           Can be NULL; if no data was read yet.
    */
    const uint8_t* getDataPointer() const;

    /* returns the size of the data in bytes */
    uint32_t getDataSize() const;

    /* holds the header for the record */
    uint32_t Header;
  protected:
     uint32_t m_DataSize;
     uint8_t* m_Data;
}; //struct

} //namespace

#endif // MW_GENERICRECORD_HPP
