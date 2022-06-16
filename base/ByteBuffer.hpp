/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2021, 2022  Dirk Stolle

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

#ifndef MWTP_BYTEBUFFER_HPP
#define MWTP_BYTEBUFFER_HPP

#include <cstddef>
#include <cstdint>

namespace MWTP
{

/** Holds a buffer of bytes. */
class ByteBuffer
{
  public:
    ByteBuffer();
    ByteBuffer(const ByteBuffer& other);
    ByteBuffer(ByteBuffer&& other);
    ByteBuffer& operator=(const ByteBuffer& other);
    ~ByteBuffer();

    bool operator==(const ByteBuffer& op) const;

    /** \brief Get the pointer to the internal buffer.
     *
     * \return Returns the pointer to the internal buffer.
     * \remarks This may be a null pointer.
     */
    const uint8_t* data() const;

    /** \brief Gets the size of the buffer data.
     *
     * \return Returns the size of the buffer data.
     * \remarks If this is zero, the data pointer may be a null pointer.
     */
    std::size_t size() const;

    /** \brief Copies data from a given buffer, replacing the current content.
     *
     * \param buffer   pointer to the data that shall be copied
     * \param buffer_size  size of the data to copy from @buffer
     * \remarks Any pre-existing content of the ByteBuffer instance will be
     *          replaced and therefore will be lost.
     */
    void copy_from(const uint8_t* buffer, const std::size_t buffer_size);

    /** \brief Resets the buffer's contents to empty buffer.
     *
     * \remarks Existing buffer data will be deleted by this method.
     */
    void reset();
  private:
    uint8_t * mData;    /**< pointer to the actual data buffer */
    std::size_t mSize;  /**< size of the pointed-to buffer */
}; // class

} // namespace

#endif // MWTP_BYTEBUFFER_HPP
