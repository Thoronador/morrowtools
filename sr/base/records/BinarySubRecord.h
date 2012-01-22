/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef SR_BINARYSUBRECORD_H
#define SR_BINARYSUBRECORD_H

#include <stdint.h>
#include <fstream>

namespace SRTP
{

struct BinarySubRecord
{
  public:
    /* constructor */
    BinarySubRecord();

    /* copy constructor */
    BinarySubRecord(const BinarySubRecord& op);

    /* assignment operator */
    BinarySubRecord& operator=(const BinarySubRecord& other);

    /* comparison operator */
    bool operator==(const BinarySubRecord& other) const;

    /* comparison operator */
    bool operator!=(const BinarySubRecord& other) const;

    /* destructor */
    ~BinarySubRecord();

    /* returns size of the internal data in bytes */
    uint16_t getSize() const;

    /* returns pointer to the internal data */
    const uint8_t* getPointer() const;

    /* returns true, if the presence flag is set */
    bool isPresent() const;

    /* sets the subrecord's presence flag */
    void setPresence(const bool presence_flag);

    #ifndef SR_UNSAVEABLE_RECORDS
    /* writes the sub record to the given output stream and returns true on success

      parameters:
          output    - the output file stream
          subHeader - the sub record's header
    */
    bool saveToStream(std::ofstream& output, const int32_t subHeader) const;
    #endif

    /* loads the sub record from the given input stream and returns true on success

      parameters:
          in_File    - the input stream
          subHeader  - the sub record's header
          withHeader - if set to true, it will read the header, too. If set to
                       false, it will begin with the length bytes.
    */
    bool loadFromStream(std::istream& in_File, const int32_t subHeader, const bool withHeader);
  protected:
    uint8_t* m_Pointer;
    uint16_t m_Size;
    bool m_Present;
}; //struct

} //namespace

#endif // SR_BINARYSUBRECORD_H
