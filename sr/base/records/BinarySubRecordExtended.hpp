/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#ifndef SR_BINARYSUBRECORDEXTENDED_HPP
#define SR_BINARYSUBRECORDEXTENDED_HPP

#include <stdint.h>
#include <fstream>

namespace SRTP
{

struct BinarySubRecordExtended
{
  public:
    /* constructor */
    BinarySubRecordExtended();

    /* copy constructor */
    BinarySubRecordExtended(const BinarySubRecordExtended& op);

    /* assignment operator */
    BinarySubRecordExtended& operator=(const BinarySubRecordExtended& other);

    /* comparison operator */
    bool operator==(const BinarySubRecordExtended& other) const;

    /* comparison operator */
    bool operator!=(const BinarySubRecordExtended& other) const;

    /* destructor */
    ~BinarySubRecordExtended();

    /* returns size of the internal data in bytes */
    uint32_t getSize() const;

    /* returns pointer to the internal data */
    const uint8_t* getPointer() const;

    /* returns true, if the presence flag is set */
    bool isPresent() const;

    /* sets the subrecord's presence flag */
    void setPresence(const bool presence_flag);

    #ifndef SR_UNSAVEABLE_RECORDS
    /* writes the sub record to the given output stream and returns true on success

      parameters:
          output    - the output stream
          subHeader - the sub record's header
    */
    bool saveToStream(std::ostream& output, const uint32_t subHeader) const;
    #endif

    /* loads the sub record from the given input stream and returns true on success

      parameters:
          in_File    - the input stream
          subHeader  - the sub record's header
          withHeader - if set to true, it will read the header, too. If set to
                       false, it will begin with the length bytes.
    */
    bool loadFromStream(std::istream& in_File, const uint32_t subHeader, const bool withHeader);

    /* loads the sub record from the given input stream and returns true on success

      parameters:
          in_File    - the input stream
          subHeader  - the sub record's header
          withHeader - if set to true, it will read the header, too. If set to
                       false, it will begin with the length bytes.
          realSize   - the subrecord's real size, as read from the previous XXXX subrecord
    */
    bool loadFromStreamExtended(std::istream& in_File, const uint32_t subHeader, const bool withHeader, const uint32_t realSize);
  protected:
    uint8_t* m_Pointer;
    uint32_t m_Size;
    bool m_Present;
}; //struct

} //namespace

#endif // SR_BINARYSUBRECORDEXTENDED_HPP
