/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Thoronador

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

#ifndef SR_GENERICRECORD_HPP
#define SR_GENERICRECORD_HPP

#include "BasicRecord.hpp"

namespace SRTP
{

/** Record type that holds the whole record as BLOB. */
struct GenericRecord: public BasicRecord
{
  public:
    /** Constructor, creates an empty record. */
    GenericRecord();

    /** Copy constructor. */
    GenericRecord(const GenericRecord& other);

    /** assignment operator */
    GenericRecord& operator=(const GenericRecord& other);

    /** Destructor. */
    virtual ~GenericRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool equals(const GenericRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /** \brief Gets the size in bytes that the record's data would occupy in a file
     *         stream, NOT including the header data.
     *
     * \return Returns the size in bytes that the record would need. Size of the
     *         header is not included.
     */
    virtual uint32_t getWriteSize() const;

    /** \brief Writes the record to the given output stream.
     *
     * \param output  the output stream
     * \return Returns true on success (record was written to stream).
     *         Returns false, if an error occurred.
     */
    virtual bool saveToStream(std::ostream& output) const;
    #endif

    /** \brief Loads the record from the given input stream.
     *
     * \param in_File    the input stream
     * \param localized  whether the file to read from is localized or not
     * \param table      the associated string table for localized files
     * \return Returns true on success (record was loaded from stream).
     *         Returns false, if an error occurred.
     */
    virtual bool loadFromStream(std::istream& in_File, const bool localized, const StringTable& table);

    /** \brief Returns true, because this is the GenericRecord class.
     *
     * \return Returns true.
     */
    virtual bool isGenericRecord() const;

    /** \brief Gets the record's type, usually its header.
     *
     * \return Returns the record's type.
     */
    virtual uint32_t getRecordType() const;

    /** \brief Gets a pointer to the record data.
     *
     * \return Returns a pointer to the record data.
     * \remark Can be nullptr, if no data was read yet.
     */
    const unsigned char* data() const;

    /** \brief Gets the size of the data in bytes.
     *
     * \return Returns the size of the data in bytes.
     */
    uint32_t size() const;

    uint32_t Header; /**< holds the header for the record */
  protected:
    uint32_t m_Size; /**< size of the data */
    unsigned char* m_Data; /**< data pointer */
}; // struct

} // namespace

#endif // SR_GENERICRECORD_HPP
