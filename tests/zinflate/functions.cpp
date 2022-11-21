/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for the Skyrim Tools Project.
    Copyright (C) 2021  Dirk Stolle

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

#include "functions.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../../base/CompressionFunctions.hpp"
#include "codes.hpp"

int getCompressedData(const std::string& fileName, std::unique_ptr<uint8_t[]>& data, uint32_t& dataSize, uint32_t& decompressedSize)
{
  if (!std::filesystem::exists(fileName))
  {
    std::cerr << "Error: File '" << fileName << "' does not exist!\n";
    return rcIO;
  }

  std::ifstream source(fileName, std::ios::binary | std::ios::in);
  if (!source.is_open() || !source.good())
  {
    std::cerr << "Error: File '" << fileName << "' cannot be opened!\n";
    return rcIO;
  }
  uint32_t expectedDecompressedSize = 0;
  source.read(reinterpret_cast<char*>(&expectedDecompressedSize), 4);
  if (!source.good())
  {
    std::cerr << "Error: Cannot read decompressed size from " << fileName
              << "!\n";
    return rcIO;
  }
  source.seekg(0, std::ios::end);
  if (!source.good())
  {
    std::cerr << "Error: Cannot jump to end of file " << fileName << "!\n";
    return rcIO;
  }
  const auto endPosition = source.tellg();
  if (endPosition <= 4)
  {
    std::cerr << "Error: File " << fileName << " is too short!\n";
    return rcIO;
  }
  source.seekg(4, std::ios::beg);
  if (!source.good())
  {
    std::cerr << "Error: Cannot jump to offset 4 of file " << fileName << "!\n";
    return rcIO;
  }
  // In theory, endPosition could also be a 64 bit integer and the following
  // conversion could reduce precision. However, I have not seen any ESM in
  // Skyrim that takes more than 2 GB, so this should be safe.
  const uint32_t readableSize = static_cast<uint32_t>(endPosition) - 4;
  std::unique_ptr<uint8_t[]> readBuffer(new uint8_t[readableSize]);
  source.read(reinterpret_cast<char*>(readBuffer.get()), readableSize);
  if (!source.good())
  {
    std::cerr << "Error: Failed to read compressed data from " << fileName
              << "!\n";
    return rcIO;
  }
  source.close();

  data = std::move(readBuffer);
  dataSize = readableSize;
  decompressedSize = expectedDecompressedSize;
  return 0;
}

int decompress(const std::unique_ptr<uint8_t[]>& data, const uint32_t dataSize, std::unique_ptr<uint8_t[]>& decompressedData, const uint32_t decompressedSize)
{
  if (data == nullptr)
  {
    std::cerr << "Error: Compressed data pointer is null!" << std::endl;
    return rcDecompression;
  }
  if (dataSize < 2)
  {
    std::cerr << "Error: Compressed data is too small for real data!" << std::endl;
    return rcDecompression;
  }
  std::unique_ptr<uint8_t[]> decompressed(new uint8_t [decompressedSize]);
  if (!MWTP::zlibDecompress(data.get(), dataSize, decompressed.get(), decompressedSize))
  {
    std::cerr << "Error while executing decompression function!\n";
    return rcDecompression;
  }

  decompressedData = std::move(decompressed);
  return 0;
}

std::optional<std::string> getDestinationFileName(const std::string& sourceFileName)
{
  std::string destinationFileName = sourceFileName + ".decompressed";
  unsigned int i = 0;
  while (std::filesystem::exists(destinationFileName))
  {
    ++i;
    std::stringstream str;
    str << i;
    destinationFileName = sourceFileName + ".decompressed." + str.str();
    if (i > 100)
    {
      std::cerr << "Error: Could not find suitable file name. Clean up some "
                << "temporary files (" << sourceFileName + ".decompressed.X)."
                << std::endl;
      return std::nullopt;
    }
  }

  return destinationFileName;
}

int writeBufferToFile(const std::string& fileName, const std::unique_ptr<uint8_t[]>& data, const uint32_t dataSize)
{
  std::ofstream destination(fileName, std::ios::binary | std::ios::out);
  if (!destination.is_open() || !destination.good())
  {
    std::cerr << "Error: File " << fileName << " cannot be opened!\n";
    return rcIO;
  }

  destination.write(reinterpret_cast<const char*>(data.get()), dataSize);
  destination.close();
  if (!destination.good())
  {
    std::cerr << "Error: Could not write decompressed data to "
              << fileName << "!" << std::endl;
    return rcIO;
  }

  return 0;
}
