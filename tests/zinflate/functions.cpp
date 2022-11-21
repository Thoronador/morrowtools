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

#include "functions.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../../base/CompressionFunctions.hpp"
#include "../zdeflate/codes.hpp"

namespace zinflate
{

int getRawData(const std::string& fileName, std::unique_ptr<uint8_t[]>& data, uint32_t& dataSize)
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
  source.seekg(0, std::ios::end);
  if (!source.good())
  {
    std::cerr << "Error: Cannot jump to end of file " << fileName << "!\n";
    return rcIO;
  }
  const auto endPosition = source.tellg();
  // In theory, endPosition could also be a 64 bit integer and the following
  // conversion could reduce precision. However, I have not seen any ESM in
  // Skyrim that takes more than 2 GB, so this should be safe.
  const uint32_t readableSize = static_cast<uint32_t>(endPosition);
  source.seekg(0, std::ios::beg);
  if (!source.good())
  {
    std::cerr << "Error: Cannot jump to beginning of file " << fileName << "!\n";
    return rcIO;
  }
  std::unique_ptr<uint8_t[]> readBuffer(new uint8_t[readableSize]);
  source.read(reinterpret_cast<char*>(readBuffer.get()), readableSize);
  if (!source.good())
  {
    std::cerr << "Error: Failed to read raw data from " << fileName << "!\n";
    return rcIO;
  }
  source.close();

  data = std::move(readBuffer);
  dataSize = readableSize;
  return 0;
}

int compress(const std::unique_ptr<uint8_t[]>& data, const uint32_t dataSize, std::unique_ptr<uint8_t[]>& compressedData, uint32_t& compressedSize)
{
  if (data == nullptr)
  {
    std::cerr << "Error: Raw data pointer is null!" << std::endl;
    return rcDecompression;
  }
  uint8_t* compressed = new uint8_t [compressedSize];
  uint32_t usedSize = 0;
  if (!MWTP::zlibCompress(data.get(), dataSize, compressed, compressedSize, usedSize))
  {
    std::cerr << "Error while executing decompression function!\n";
    delete[] compressed;
    return rcDecompression;
  }

  compressedData.reset(compressed);
  compressedSize = usedSize;
  return 0;
}

std::optional<std::string> getDestinationFileName(const std::string& sourceFileName)
{
  std::string destinationFileName = sourceFileName + ".compressed";
  unsigned int i = 0;
  while (std::filesystem::exists(destinationFileName))
  {
    ++i;
    std::stringstream str;
    str << i;
    destinationFileName = sourceFileName + ".compressed." + str.str();
    if (i > 100)
    {
      std::cerr << "Error: Could not find suitable file name. Clean up some "
                << "temporary files (" << sourceFileName + ".compressed.X)."
                << std::endl;
      return std::nullopt;
    }
  }

  return destinationFileName;
}

int writeBufferToFile(const std::string& fileName, const uint32_t rawSize, const std::unique_ptr<uint8_t[]>& data, const uint32_t dataSize)
{
  std::ofstream destination(fileName, std::ios::binary | std::ios::out);
  if (!destination.is_open() || !destination.good())
  {
    std::cerr << "Error: File " << fileName << " cannot be opened!\n";
    return rcIO;
  }

  destination.write(reinterpret_cast<const char*>(&rawSize), 4);
  destination.write(reinterpret_cast<const char*>(data.get()), dataSize);
  destination.close();
  if (!destination.good())
  {
    std::cerr << "Error: Could not write compressed data to "
              << fileName << "!" << std::endl;
    return rcIO;
  }

  return 0;
}

} // namespace
