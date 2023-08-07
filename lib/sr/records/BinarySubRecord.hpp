/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2021  Thoronador

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

#ifndef SR_BINARYSUBRECORD_HPP
#define SR_BINARYSUBRECORD_HPP

#include <cstdint>
#include <fstream>

namespace SRTP
{

/** Holds data of a subrecord as BLOB. */
struct BinarySubRecord
{
  public:
    /** Constructor, creates an empty record. */
    BinarySubRecord();

    /** Copy constructor.
     *
     * \param op  the other record to copy from
     */
    BinarySubRecord(const BinarySubRecord& op);

    /** Assignment operator. */
    BinarySubRecord& operator=(const BinarySubRecord& other);

    /** \brief Compares two BinarySubRecord instances for equality.
     *
     * \param other   the other BinarySubRecord to compare to
     * \return Returns true, if @other is equal to this instance.
     *         Returns false otherwise.
     */
    bool operator==(const BinarySubRecord& other) const;

    /** \brief Checks two BinarySubRecord instances for inequality.
     *
     * \param other   the other BinarySubRecord to compare to
     * \return Returns true, if @other is not equal to this instance.
     *         Returns false otherwise.
     */
    bool operator!=(const BinarySubRecord& other) const;

    /** Destructor. */
    ~BinarySubRecord();

    /** \brief Gets the size of the internal data in bytes.
     *
     * \return Returns size of the internal data in bytes.
     *         This may be zero, if no internal data is present.
     */
    uint16_t size() const;

    /** \brief Gets a pointer to the internal data.
     *
     * \return Returns a pointer to the internal data.
     *         Note that this pointer may be a null pointer if there is no data.
     *         Use isPresent() to check whether pointer is valid first.
     */
    const uint8_t* data() const;

    /** \brief Indicates whether the record is present and has data.
     *
     * \return Returns true, if the record is present and has data.
     *         Returns false otherwise.
     * \remarks If this returns false, then the functions getting the size and
     *          the internal data pointer may return unexpected values like zero
     *          or null pointers. In that case do not rely on their values.
     */
    bool isPresent() const;

    /** \brief Sets the subrecord's presence flag.
     *
     * \param presence_flag  new value of the flag (true == present, false == absent)
     */
    void setPresence(const bool presence_flag);

    #ifndef SR_UNSAVEABLE_RECORDS
    /** \brief Writes the sub record to the given output stream.
     *
     * \param output       the output stream
     * \param subHeader    the sub record's header
     * \return Returns true on success.
     *         Returns false, if an error occurred.
     * \remarks If the subrecord's presence flag is false, this method does not
     *          write any data to the stream.
     */
    bool saveToStream(std::ostream& output, const uint32_t subHeader) const;
    #endif

    /** \brief Loads the sub record from the given input stream.
     *
     * \param in_File       the input stream
     * \param subHeader     the sub record's header
     * \param withHeader    whether the sub record's header has to be read, too;
     *                      If set to true, the method will read the header,
     *                      too. If set to false, it will begin with the length
     *                      bytes of the sub record.
     * \return Returns true on success (sub record was loaded from stream).
     *         Returns false, if an error occurred.
     */
    bool loadFromStream(std::istream& in_File, const uint32_t subHeader, const bool withHeader);
  private:
    uint8_t* m_Data; /**< pointer to internal BLOB, may be nullptr */
    uint16_t m_Size; /**< size of the internal data, may be zero if there's no data */
    bool m_Present;  /**< flag to indicate whether subrecord has a value */
}; // struct

} // namespace

#endif // SR_BINARYSUBRECORD_HPP
