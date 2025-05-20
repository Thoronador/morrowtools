/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2022, 2025  Dirk Stolle

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

#ifndef SR_BASICRECORD_HPP
#define SR_BASICRECORD_HPP

#include <cstdint>
#include <fstream>
#include <vector>

namespace SRTP
{

// forward declaration for StringTable
class StringTable;

/** Abstract base class for all records in Skyrim's ESM files. */
struct BasicRecord
{
  public:
    /** Constructor. */
    BasicRecord();

    /** Destructor. */
    virtual ~BasicRecord() = default;

    #ifndef SR_UNSAVEABLE_RECORDS
    /** \brief Gets the size in bytes that the record's data would occupy in a stream.
     *
     * \return Returns the size in bytes that the record's data would occupy in
     *         a file stream, including the size of the header data.
     */
    uint32_t getTotalWrittenSize() const;

    /** \brief Writes the record to the given output stream.
     *
     * \param output  the output stream
     * \return Returns true on success (record was written to stream).
     *         Returns false, if an error occurred.
     */
    virtual bool saveToStream(std::ostream& output) const = 0;
    #endif

    /** \brief Loads the record from the given input stream.
     *
     * \param input      the input stream
     * \param localized  whether the file to read from is localized or not
     * \param table      the associated string table for localized files
     * \return Returns true on success (record was loaded from stream).
     *         Returns false, if an error occurred.
     */
    virtual bool loadFromStream(std::istream& input, const bool localized, const StringTable& table) = 0;

    /** \brief Gets the record's type, usually its header.
     *
     * \return Returns the record's type.
     */
    virtual uint32_t getRecordType() const = 0;

    /** \brief Checks whether the record's data is compressed.
     *
     * \return Returns true, if the record's data is compressed, according to
     *         the set flags.
     */
    bool isCompressed() const;

    /** \brief Checks whether the record is deleted.
     *
     * \brief Returns true, if the record is deleted, according to the set flags.
     */
    bool isDeleted() const;

    /** \brief Checks whether the record should be ignored.
     *
     * \return Returns true, if the record should be ignored, according to the
     *         set flags.
     */
    bool isIgnored() const;

    // flag constants
    static const uint32_t cDeletedFlag     = 0x00000020;
    static const uint32_t cIgnoredFlag     = 0x00001000;
    static const uint32_t cCompressionFlag = 0x00040000;

    // partially unknown values - 16 bytes, i.e. 4 x uint32_t
    uint32_t headerFlags;    /**< generic flags of the record */
    uint32_t headerFormID;   /**< form id of the record */
    uint32_t headerRevision;
    uint16_t headerVersion;  /**< version of the record */
    uint16_t headerUnknown5;
  protected:
    void copyBasicMembers(const BasicRecord& other);

    #ifndef SR_NO_RECORD_EQUALITY
    /** Returns true, if the other basic record contains the same data. */
    bool equalsBasic(const BasicRecord& other) const;
    #endif

    /** \brief Loads the record's header data.
     *
     * Loads the first data of a record, the record size and yet unknown other
       values, from the given input stream.
     * \param input        the input stream
     * \param sizeStorage  reference to the variable that will be used to store
     *                     the size
     * \return Returns true on success (data was loaded successfully).
     *         Returns false, if an error occurred.
     */
    bool loadSizeAndUnknownValues(std::istream& input, uint32_t& sizeStorage);

    #ifndef SR_UNSAVEABLE_RECORDS
    /** \brief Writes the record header to the output stream.
     *
     * Writes the first data of a record, the record size and yet unknown other
     * values, to the given output stream.
     *
     * \param output   the output stream
     * \param theSize  size value to be written
     * \return Returns true on success (data was written successfully).
     *         Returns false, if an error occurred.
     */
    bool saveSizeAndUnknownValues(std::ostream& output, const uint32_t theSize) const;

    /** \brief Gets the size in bytes that the record's data would occupy in a file
     *         stream, NOT including the header data.
     *
     * \return Returns the size in bytes that the record would need. Size of the
     *         header is not included.
     * \remarks
     *     This function has to be re-implemented for every new record type.
     */
    virtual uint32_t getWriteSize() const = 0;
    #endif

    /** \brief Tries to load a 4 byte long sub record from the stream.
     *
     * \param input       the input stream
     * \param subHeader   the expected header of that sub record
     * \param target      the uint32_t that will be used to store the read data
     * \param withHeader  if set to true, the header is read, too. Otherwise
     *                    just the sub record's content is read.
     * \return Returns true on success (data was loaded successfully).
     *         Returns false, if an error occurred.
     */
    bool loadUint32SubRecordFromStream(std::istream& input, const uint32_t subHeader, uint32_t& target, const bool withHeader) const;

    /** \brief Tries to load a NUL-terminated string from the stream.
     *
     * \param input       the input stream
     * \param target      the string that will be used to store the read data
     * \param buffer      a pre-allocated array of char that can hold at least 512 bytes
     * \param subHeader   the expected header of that sub record
     * \param withHeader  if set to true, the header is read, too. Otherwise
     *                    just the sub record's content is read.
     * \param bytesRead   the variable that holds the number of bytes read so far
     * \return Returns true on success (data was loaded successfully).
     *         Returns false, if an error occurred.
     */
    bool loadString512FromStream(std::istream& input, std::string& target, char * buffer, const uint32_t subHeader, const bool withHeader, uint32_t& bytesRead) const;

    /** \brief Loads a keyword array (KSIZ/KWDA) from the stream.
     *
     * \param input       the input stream
     * \param keywords    the vector that will be used to store the read keywords
     * \param bytesRead   the variable that holds the number of bytes read so far
     * \remarks This method assumes that the KSIZ header has already been read
     *          when it's called.
     * \return Returns true on success (data was loaded successfully).
     *         Returns false, if an error occurred.
     */
    bool loadKeywords(std::istream& input, std::vector<uint32_t>& keywords, uint32_t& bytesRead) const;

    /** \brief Tries to load a fixed length buffer from the stream.
     *
     * \param input       the input stream
     * \param len         expected length of the buffer in bytes
     * \param target      the pre-allocated buffer that will be used to store the
     *                    read data, has to have a length of at least len bytes
     * \param subHeader   the expected header of that sub record
     * \param withHeader  if set to true, the header is read, too. Otherwise
     *                    just the sub record's content is read.
     * \param bytesRead   the variable that holds the number of bytes read so far
     * \return Returns true on success (data was loaded successfully).
     *         Returns false, if an error occurred.
     */
    bool loadBufferFromStream(std::istream& input, const uint16_t len, uint8_t * target, const uint32_t subHeader, const bool withHeader, uint32_t& bytesRead) const;
}; // struct

} // namespace

#endif // SR_BASICRECORD_HPP
