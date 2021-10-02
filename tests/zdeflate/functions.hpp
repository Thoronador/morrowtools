/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021  Thoronador

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

#ifndef SRTP_ZDEFLATE_FUNCTIONS_HPP
#define SRTP_ZDEFLATE_FUNCTIONS_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <memory>

/** \brief Reads compressed data from a file.
 *
 * \param fileName   name of the file to read
 * \param data       (output parameter) pointer to the compressed data
 * \param dataSize   (output parameter) stores the size of the read compressed data
 * \param decompressedSize  (output parameter) stores the expected size of the data when decompressed
 * \return Returns zero on success. Returns non-zero exit code on error.
 */
int getCompressedData(const std::string& fileName, std::unique_ptr<uint8_t[]>& data, uint32_t& dataSize, uint32_t& decompressedSize);

/** \brief Performs zlib decompression (inflate).
 *
 * \param data              pointer to the compressed data
 * \param dataSize          size of the compressed data in bytes
 * \param decompressedData  (output parameter) pointer to the decompressed data
 * \param decompressedSize  the expected size in bytes of the data when decompressed
 * \return Returns zero on success. Returns non-zero exit code on error.
 */
int decompress(const std::unique_ptr<uint8_t[]>& data, const uint32_t dataSize,  std::unique_ptr<uint8_t[]>& decompressedData, const uint32_t decompressedSize);

/** \brief Determines a suitable destination file name.
 *
 * \param sourceFileName   name of the original source file
 * \return Returns a suitable file name on success.
 *         Returns an empty optional on error.
 */
std::optional<std::string> getDestinationFileName(const std::string& sourceFileName);

/** \brief Writes given pointer data to a file.
 *
 * \param fileName   destination file name to write to
 * \param data       pointer to the data that shall be written
 * \param dataSize   size of the data pointed to be @data
 * \return Returns zero on success. Returns non-zero exit code on error.
 */
int writeBufferToFile(const std::string& fileName, const std::unique_ptr<uint8_t[]>& data, const uint32_t dataSize);

#endif // SRTP_ZDEFLATE_FUNCTIONS_HPP
