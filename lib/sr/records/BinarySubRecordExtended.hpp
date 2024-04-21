/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2021, 2024  Dirk Stolle

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

#include <cstdint>
#include <fstream>

namespace SRTP
{

/** Holds data of a sub record as BLOB. As a difference to the BinarySubRecord
 *  the BinarySubRecordExtended can read sub records that are larger than
 *  65535 bytes. Since the Skyrim ESM format only allows sub records up to
 *  65535 bytes, it uses a hack: The size is set to zero, but it adds an
 *  additional sub record with XXXX header that contains the real size as a
 *  32 bit integer.
 */
struct BinarySubRecordExtended
{
  public:
    /** Constructor, creates an empty record. */
    BinarySubRecordExtended();

    /** Copy constructor.
     *
     * \param op  the other record to copy from
     */
    BinarySubRecordExtended(const BinarySubRecordExtended& op);

    /** Assignment operator. */
    BinarySubRecordExtended& operator=(const BinarySubRecordExtended& other);

    /** \brief Compares two BinarySubRecordExtended instances for equality.
     *
     * \param other   the other BinarySubRecord to compare to
     * \return Returns true, if @other is equal to this instance.
     *         Returns false otherwise.
     */
    bool operator==(const BinarySubRecordExtended& other) const;

    /** \brief Checks two BinarySubRecord instances for inequality.
     *
     * \param other   the other BinarySubRecordExtended to compare to
     * \return Returns true, if @other is not equal to this instance.
     *         Returns false otherwise.
     */
    bool operator!=(const BinarySubRecordExtended& other) const;

    /** Destructor. */
    ~BinarySubRecordExtended();

    /** \brief Gets the size of the internal data in bytes.
     *
     * \return Returns size of the internal data in bytes.
     *         This may be zero, if no internal data is present.
     */
    uint32_t size() const;

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

    /** \brief Sets the sub record's presence flag.
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
     * \remarks If the sub record's presence flag is false, this method does not
     *          write any data to the stream.
     */
    bool saveToStream(std::ostream& output, const uint32_t subHeader) const;
    #endif

    /** \brief Loads the sub record from the given input stream.
     *
     * \param input         the input stream
     * \param subHeader     the sub record's header
     * \param withHeader    whether the sub record's header has to be read, too;
     *                      If set to true, the method will read the header,
     *                      too. If set to false, it will begin with the length
     *                      bytes of the sub record.
     * \return Returns true on success (sub record was loaded from stream).
     *         Returns false, if an error occurred.
     */
    bool loadFromStream(std::istream& input, const uint32_t subHeader, const bool withHeader);

    /** \brief Loads an extended sub record (>64 KiB) from the given input stream.
     *
     * \param input         the input stream
     * \param subHeader     the sub record's header
     * \param withHeader    whether the sub record's header has to be read, too;
     *                      If set to true, the method will read the header,
     *                      too. If set to false, it will begin with the length
     *                      bytes of the sub record.
     * \param realSize      the sub record's real size, as read from the previous
     *                      XXXX sub record
     */
    bool loadFromStreamExtended(std::istream& input, const uint32_t subHeader, const bool withHeader, const uint32_t realSize);
  protected:
    uint8_t* m_Data; /**< pointer to internal BLOB, may be nullptr */
    uint32_t m_Size; /**< size of the internal data, may be zero if there's no data */
    bool m_Present;  /**< flag to indicate whether sub record has a value */
}; // struct

} // namespace

#endif // SR_BINARYSUBRECORDEXTENDED_HPP
