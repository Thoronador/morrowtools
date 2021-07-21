/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef COMPRESSIONFUNCTIONS_HPP
#define COMPRESSIONFUNCTIONS_HPP

#include <stdint.h>

namespace MWTP
{

/* tries to decompress the data pointed to by compressedData and stores the
   decompressed bits in decompressionBuffer. Returns true in case of success,
   or false if an error occurred.

   parameters:
       compressedData - pointer to the buffer containing the compressed data
       compressedSize - length of the buffer in bytes
       decompBuffer   - pre-allocated buffer that will hold the decompressed data
       decompSize     - size of decompBuffer
*/
bool zlibDecompress(uint8_t * compressedData, const uint32_t compressedSize, uint8_t * decompBuffer, const uint32_t decompSize);

} //namespace

#endif // COMPRESSIONFUNCTIONS_HPP
