/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013, 2021  Dirk Stolle

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

/** Record type that holds the record data as BLOB. */
struct GenericRecord: public BasicRecord
{
  public:
    /** Creates an empty record. */
    GenericRecord();

    GenericRecord(const GenericRecord& op);
    GenericRecord& operator=(const GenericRecord& other);

    virtual ~GenericRecord();

    #ifndef MW_UNSAVEABLE_RECORDS
    /** \brief Writes the record to the given output stream.
     *
     * \param output  the output stream
     * \return Returns true on success (record was written to stream).
     *         Returns false, if an error occurred.
     */
    bool saveToStream(std::ostream& output) const override;
    #endif

    /** \brief Loads the record from the given input stream.
     *
     * \param input    the input stream
     * \return Returns true on success (record was loaded from stream).
     *         Returns false, if an error occurred.
     */
    bool loadFromStream(std::istream& input) override;

    /** \brief Gets a pointer to the record data.
     *
     * \return Returns a pointer to the record data.
     * \remark Can be nullptr, if no data was read yet.
     */
    const uint8_t* data() const;

    /** \brief Gets the size of the data in bytes.
     *
     * \return Returns the size of the data in bytes.
     */
    uint32_t size() const;

    uint32_t Header; /**< holds the header for the record */
  protected:
     uint32_t m_DataSize;
     uint8_t* m_Data;
}; // struct

} // namespace

#endif // MW_GENERICRECORD_HPP
