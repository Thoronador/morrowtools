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

#ifndef SR_BASICRECORD_H
#define SR_BASICRECORD_H

#include <fstream>
#include <stdint.h>

namespace SRTP
{

struct BasicRecord
{
  public:
    /* constructor */
    BasicRecord();

    /* destructor */
    virtual ~BasicRecord();

    #ifndef SR_UNSAVEABLE_RECORDS
    /* returns the size in bytes that the record's data would occupy in a file
       stream, including the size of the header data
    */
    uint32_t getTotalWrittenSize() const;

    /* writes the record to the given output stream and returns true on success

       parameters:
           output   - the output file stream
    */
    virtual bool saveToStream(std::ofstream& output) const = 0;
    #endif

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File) = 0;

    /* returns true, if the record is a generic record (see GenericRecord.h) */
    virtual bool isGenericRecord() const;

    /* returns the record's type, usually its header */
    virtual int32_t getRecordType() const = 0;

    /* returns true, if the record's data is compressed, according to the set
       flags
    */
    bool isCompressed() const;

    /* returns true, if the record is deleted, according to the set flags */
    bool isDeleted() const;

    //flag constants
    static const uint32_t cDeletedFlag     = 0x00000020;
    static const uint32_t cCompressionFlag = 0x00040000;

    //partially unknown values - 16 bytes, i.e. 4 x uint32_t
    uint32_t headerFlags;
    uint32_t headerFormID;
    uint32_t headerRevision;
    uint16_t headerVersion;
    uint16_t headerUnknown5;
  protected:
    //util. func.
    void copyBasicMembers(const BasicRecord& other);

    /* returns true, if the other basic record contains the same data */
    bool equalsBasic(const BasicRecord& other) const;

    /* loads the first data of a record, the record size and yet unknown other
       values, from the given input stream and returns true on success

       parameters:
           in_File     - the input file stream
           sizeStorage - reference to the variable that will be used to store the size
    */
    bool loadSizeAndUnknownValues(std::ifstream& in_File, uint32_t& sizeStorage);

    #ifndef SR_UNSAVEABLE_RECORDS
    /* writes the first data of a record, the record size and yet unknown other
       values, to the given output stream and returns true on success

       parameters:
           output  - the output file stream
           theSize - size to be written
    */
    bool saveSizeAndUnknownValues(std::ofstream& output, const uint32_t theSize) const;

    /* returns the size in bytes that the record's data would occupy in a file
       stream, NOT including the header data

       parameters:
           none

       remarks:
           This function has to be reimplemented for every new record type
    */
    virtual uint32_t getWriteSize() const = 0;
    #endif

    /* tries to load a 4 byte long subrecord from the stream and returns true
       in case of success

       parameters:
           in_File   - the input stream
           subHeader - the expected header of that subrecord
           target    - the uint32_t that will be used to store the read data
           withHeader - if set to true, the header is read, too. Otherwise just
                        the subrecord's content is read
    */
    bool loadUint32SubRecordFromStream(std::istream& in_File, const int32_t subHeader, uint32_t& target, const bool withHeader=true) const;

    /* tries to load a NUL-terminated string from the stream and returns true
       in case of success

       parameters:
           in_File    - the input stream
           target     - the uint32_t that will be used to store the read data
           buffer     - a pre-allocated array of char that can hold at least 512 bytes
           subHeader  - the expected header of that subrecord
           withHeader - if set to true, the header is read, too. Otherwise just
                        the subrecord's content is read
           bytesRead  - the variable that holds the number of bytes read so far
    */
    bool loadString512FromStream(std::istream& in_File, std::string& target, char * buffer, const int32_t subHeader, const bool withHeader, uint32_t& bytesRead) const;
}; //struct

} //namespace

#endif // SR_BASICRECORD_H
