/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2022  Dirk Stolle

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

#ifdef MWTP_ZLIB_COMPRESS
bool zlibCompress(uint8_t * rawData, const uint32_t rawSize, uint8_t*& compBuffer, uint32_t& compSize, uint32_t& usedSize, const int level)
{
  if ((rawData == nullptr) || (rawSize == 0) || (compBuffer == nullptr) || (compSize == 0))
  {
    usedSize = 0;
    std::cerr << "zlibCompress: Error: Invalid buffer values given!\n";
    return false;
  }

  z_stream streamZlib;

  /* allocate deflate state */
  streamZlib.zalloc = Z_NULL;
  streamZlib.zfree = Z_NULL;
  streamZlib.opaque = Z_NULL;
  streamZlib.avail_in = 0;
  streamZlib.next_in = Z_NULL;
  int z_return = deflateInit(&streamZlib, level);
  if (z_return != Z_OK)
  {
    switch (z_return)
    {
      case Z_MEM_ERROR:
           std::cerr << "zlibCompress: Error: Not enough memory to initialize z_stream!\n";
           break;
      case Z_VERSION_ERROR:
           std::cerr << "zlibCompress: Error: Incompatible library version!\n";
           break;
      case Z_STREAM_ERROR:
           std::cerr << "zlibCompress: Error: " << level << " is not a valid compression level!\n";
           break;
      default:
           std::cerr << "zlibCompress: Error: Could not initialize z_stream!\n";
           break;
    }
    return false;
  }

  const uLong bound = deflateBound(&streamZlib, rawSize);
  if (compSize < bound)
  {
    // re-allocate buffer
    delete[] compBuffer;
    compBuffer = new uint8_t[bound];
    compSize = bound;
  }

  streamZlib.avail_in = rawSize;
  streamZlib.next_in = rawData;

  streamZlib.avail_out = compSize;
  streamZlib.next_out = compBuffer;

  /* compress */
  z_return = deflate(&streamZlib, Z_FINISH);
  switch (z_return)
  {
    case Z_OK: // not enough output buffer
         usedSize = 0;
         (void) deflateEnd(&streamZlib);
         std::cerr << "zlibCompress: Output buffer is too small for deflate(), available output buffer size is "
                   << streamZlib.avail_out << " bytes!\n";
         return false;
    case Z_STREAM_END: // finished
         usedSize = compSize - streamZlib.avail_out;
         (void) deflateEnd(&streamZlib);
         return true;
    default:
         std::cerr << "zlibCompress: unknown error (code=" << z_return << ")!\n";
         (void) deflateEnd(&streamZlib);
         return false;
  }
}
#endif // MWTP_ZLIB_COMPRESS

std::string zlibVersion()
{
  return std::string(::zlibVersion());
}

} // namespace
