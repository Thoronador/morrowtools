/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2021  Thoronador

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

#ifndef LZ4_COMPRESSIONFUNCTIONS_HPP
#define LZ4_COMPRESSIONFUNCTIONS_HPP

#include <cstdint>
#include <string>

namespace MWTP
{

/** Tries to decompress the data pointed to by compressedData and stores the
 * decompressed bits in decompressionBuffer.
 *
 * \param compressedData   pointer to the buffer containing the compressed data
 * \param compressedSize   length of the compressed data buffer in bytes
 * \param decompBuffer     pre-allocated buffer that will hold the decompressed data
 * \param decompSize       size of decompBuffer in bytes
 * \return Returns true in case of success, or false if an error occurred.
 */
bool lz4Decompress(uint8_t * compressedData, const uint32_t compressedSize, uint8_t * decompBuffer, const uint32_t decompSize);

/** \brief Gets version of the lz4 library.
 *
 * \return Returns a string containing lz4's version.
 */
std::string lz4Version();

} // namespace

#endif // LZ4_COMPRESSIONFUNCTIONS_HPP
