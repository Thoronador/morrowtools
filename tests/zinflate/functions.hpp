/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for the Skyrim Tools Project.
    Copyright (C) 2022  Dirk Stolle

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

#ifndef SRTP_ZINFLATE_FUNCTIONS_HPP
#define SRTP_ZINFLATE_FUNCTIONS_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <memory>

namespace zinflate
{

/** \brief Reads raw (uncompressed) data from a file.
 *
 * \param fileName   name of the file to read
 * \param data       (output parameter) pointer to the raw data
 * \param dataSize   (output parameter) stores the size of the read raw data
 * \return Returns zero on success. Returns non-zero exit code on error.
 */
int getRawData(const std::string& fileName, std::unique_ptr<uint8_t[]>& data, uint32_t& dataSize);

/** \brief Performs zlib compression (deflate).
 *
 * \param data              pointer to the raw (uncompressed) data
 * \param dataSize          size of the raw data in bytes
 * \param compressedData  (output parameter) pointer to the compressed data
 * \param compressedSize  (in/out) the size in bytes of the compressedData buffer
 * \return Returns zero on success. Returns non-zero exit code on error.
 */
int compress(const std::unique_ptr<uint8_t[]>& data, const uint32_t dataSize, std::unique_ptr<uint8_t[]>& compressedData, uint32_t& compressedSize);

/** \brief Determines a suitable destination file name.
 *
 * \param sourceFileName   name of the original source file
 * \return Returns a suitable file name on success.
 *         Returns an empty optional on error.
 */
std::optional<std::string> getDestinationFileName(const std::string& sourceFileName);

/** \brief Writes given pointer data to a file, prefixed by uncompressed size value.
 *
 * \param fileName   destination file name to write to
 * \param rawSize    size of the data before compression
 * \param data       pointer to the data that shall be written
 * \param dataSize   size of the data pointed to be @data
 * \return Returns zero on success. Returns non-zero exit code on error.
 */
int writeBufferToFile(const std::string& fileName, const uint32_t rawSize, const std::unique_ptr<uint8_t[]>& data, const uint32_t dataSize);

} // namespace

#endif // SRTP_ZINFLATE_FUNCTIONS_HPP
