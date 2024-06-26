/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2021, 2022, 2024  Dirk Stolle

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

#include "lz4Compression.hpp"
#include <iostream>
#if !defined(MWTP_NO_LZ4)
#include <lz4.h>
#include <lz4frame.h>
#endif

namespace MWTP
{

#if defined(MWTP_NO_LZ4)
bool lz4Decompress([[maybe_unused]] uint8_t * compressedData,
                   [[maybe_unused]] const uint32_t compressedSize,
                   [[maybe_unused]] uint8_t * decompBuffer,
                   [[maybe_unused]] const uint32_t decompSize)
{
  // This is a build without liblz4, decompression is not available.
  return false;
}

bool lz4Compress([[maybe_unused]] uint8_t * rawData,
                 [[maybe_unused]] const uint32_t rawSize,
                 [[maybe_unused]] uint8_t*& compBuffer,
                 [[maybe_unused]] uint32_t& compSize,
                 [[maybe_unused]] uint32_t& usedSize)
{
  // This is a build without liblz4, compression is not available.
  return false;
}
#else
bool lz4Decompress(uint8_t * compressedData, const uint32_t compressedSize, uint8_t * decompBuffer, const uint32_t decompSize)
{
  if ((compressedData == nullptr) || (compressedSize == 0)
     || (decompBuffer == nullptr) || (decompSize == 0))
  {
    std::cerr << "lz4Decompress: Error: Invalid buffer values given!\n";
    return false;
  }

  #if LZ4_VERSION_NUMBER < 10703
  // Older lz4 versions have a different type for the compression context.
  using LZ4F_dctx = LZ4F_dctx_s;
  #endif

  // Skyrim seems to use the lz4 frame API and not the "simple" API.
  LZ4F_dctx* context = nullptr;
  auto errorCode = LZ4F_createDecompressionContext(&context, LZ4F_VERSION);
  if (LZ4F_isError(errorCode))
  {
    std::cerr << "lz4Decompress: Error: Failed to get decompression context!\n"
              << "Error message: " << LZ4F_getErrorName(errorCode) << "\n";
    return false;
  }

  size_t alreadyConsumed = 0;
  size_t alreadyDecompressed = 0;
  size_t expectedBytes = 0;
  size_t outputSize = 0;
  size_t inputSize = 0;
  do
  {
    outputSize = decompSize - alreadyDecompressed;
    inputSize = compressedSize - alreadyConsumed;
    expectedBytes = LZ4F_decompress(context,
                      decompBuffer + alreadyDecompressed,
                      &outputSize,
                      compressedData + alreadyConsumed,
                      &inputSize, nullptr);
    if (LZ4F_isError(expectedBytes))
    {
      std::cerr << "lz4Decompress: Error: Failed to decompress more data!\n"
                << "Error message: " << LZ4F_getErrorName(expectedBytes) << "\n";
      LZ4F_freeDecompressionContext(context);
      return false;
    }
    alreadyConsumed += inputSize;
    alreadyDecompressed += outputSize;
  } while (
      // If the decompression context expects zero bytes, it is finished.
      (expectedBytes > 0)
      // ... and it should either consume or produce at least one byte per
      // loop iteration. Otherwise it is stale.
      && ((inputSize > 0) || (outputSize > 0))
  );

  errorCode = LZ4F_freeDecompressionContext(context);
  if (LZ4F_isError(errorCode))
  {
    std::cerr << "lz4Decompress: Error: Failed to free decompression context!\n"
              << "Error message: " << LZ4F_getErrorName(errorCode) << "\n";
    return false;
  }
  // Actually processed and produced sizes should match the expectations.
  if (alreadyDecompressed != decompSize)
  {
    std::cerr << "lz4Decompress: Error: Decompression produced "
              << alreadyDecompressed << " bytes, but " << decompSize
              << " bytes were expected!\n";
    return false;
  }
  if (alreadyConsumed != compressedSize)
  {
    std::cerr << "lz4Decompress: Error: Decompression consumed "
              << alreadyConsumed << " bytes, but " << compressedSize
              << " bytes were provided!\n";
    return false;
  }

  return true;
}

bool lz4Compress(uint8_t * rawData, const uint32_t rawSize, uint8_t*& compBuffer, uint32_t& compSize, uint32_t& usedSize)
{
  if ((rawData == nullptr) || (rawSize == 0) || (compBuffer == nullptr) || (compSize == 0))
  {
    usedSize = 0;
    std::cerr << "lz4Compress: Error: Invalid buffer values given!\n";
    return false;
  }

  const auto bound = LZ4F_compressFrameBound(rawSize, nullptr);
  if (bound > compSize)
  {
    // re-allocate buffer
    delete[] compBuffer;
    compBuffer = nullptr;
    compSize = 0;
    try
    {
      compBuffer = new uint8_t[bound];
    }
    catch(...)
    {
      compBuffer = nullptr;
      return false;
    }
    compSize = static_cast<uint32_t>(bound);
  }

  const auto bytesWritten = LZ4F_compressFrame(compBuffer, compSize, rawData, rawSize, nullptr);
  if (LZ4F_isError(bytesWritten))
  {
    std::cerr << "lz4Compress: Error: Failed to compress data into frame!\n"
              << "Error message: " << LZ4F_getErrorName(bytesWritten) << "\n";
    return false;
  }

  usedSize = static_cast<uint32_t>(bytesWritten);
  return true;
}

#endif

std::string lz4Version()
{
  #if defined(MWTP_NO_LZ4)
  // This is a build without liblz4.
  return std::string("none");
  #else
    #if LZ4_VERSION_NUMBER < 10703
    // Predefined macros could be used, too, but that would just get us the
    // compile time version of the library, not the runtime version.
    const int version = LZ4_versionNumber();
    const int major = version / 10000;
    const int minor = (version - (10000 * major)) / 100;
    const int patch = version % 100;
    return std::to_string(major).append(".")
           .append(std::to_string(minor)).append(".")
           .append(std::to_string(patch));
    #else
    // Newer versions of the lz4 library provide a function that returns the
    // library version as a string / char*, so we can use that.
    return std::string(LZ4_versionString());
    #endif // LZ4_VERSION_NUMBER
  #endif
}

} // namespace
