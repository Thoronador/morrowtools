/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
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

#include "CompressionFunctions.hpp"
#include <iostream>
#include <zlib.h>

namespace MWTP
{

bool zlibDecompress(uint8_t * compressedData, const uint32_t compressedSize, uint8_t * decompBuffer, const uint32_t decompSize)
{
  if ((compressedData == nullptr) || (compressedSize == 0)
     || (decompBuffer == nullptr) || (decompSize == 0))
  {
    std::cerr << "zlibDecompress: Error: invalid buffer values given!\n";
    return false;
  }

  z_stream streamZlib;

  /* allocate inflate state */
  streamZlib.zalloc = Z_NULL;
  streamZlib.zfree = Z_NULL;
  streamZlib.opaque = Z_NULL;
  streamZlib.avail_in = 0;
  streamZlib.next_in = Z_NULL;
  int z_return = inflateInit(&streamZlib);
  if (z_return != Z_OK)
  {
    switch (z_return)
    {
      case Z_MEM_ERROR:
           std::cerr << "zlibDecompress: Error: Not enough memory to initialize z_stream!\n";
           break;
      case Z_VERSION_ERROR:
           std::cerr << "zlibDecompress: Error: Incompatible library version!\n";
           break;
      case Z_STREAM_ERROR:
           std::cerr << "zlibDecompress: Error: Invalid parameters in z_stream!\n";
           break;
      default:
           std::cerr << "zlibDecompress: Error: Could not initialize z_stream!\n";
           break;
    }
    return false;
  }

  streamZlib.avail_in = compressedSize;
  streamZlib.next_in = compressedData;

  streamZlib.avail_out = decompSize;
  streamZlib.next_out = decompBuffer;

  /* decompress */
  z_return = inflate(&streamZlib, Z_NO_FLUSH);
  switch (z_return)
  {
    case Z_NEED_DICT:
         z_return = Z_DATA_ERROR;     /* and fall through */
    case Z_DATA_ERROR:
    case Z_STREAM_ERROR: //stream state
    case Z_MEM_ERROR:
         (void)inflateEnd(&streamZlib);
         std::cerr << "zlibDecompress: Error while calling inflate()!\n";
         return false;
  }
  const uint32_t have = decompSize - streamZlib.avail_out;
  /* clean up zlib */
  (void) inflateEnd(&streamZlib);
  // check, if size matches expected number of bytes
  if (have != decompSize)
  {
    std::cerr << "zlibDecompress: Error: Having only " << have << " bytes in output"
              << "buffer, but expected size is " << decompSize << " bytes.\n";
    return false;
  }
  // Return value Z_STREAM_END is the right one, if all was successful.
  return (z_return == Z_STREAM_END);
}

std::string zlibVersion()
{
  return std::string(::zlibVersion());
}

} // namespace
