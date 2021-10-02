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

#include <iostream>
#include <memory>
#include "codes.hpp"
#include "functions.hpp"

void showUsage()
{
  std::cout << "Usage:\n"
            << "  zdeflate source_file" << std::endl;
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    showUsage();
    return rcInvalidParameters;
  }
  const std::string sourceFileName(argv[1]);
  if (sourceFileName == "--help" || sourceFileName == "-?")
  {
    showUsage();
    return 0;
  }

  std::unique_ptr<uint8_t[]> data(nullptr);
  uint32_t dataSize = 0;
  uint32_t decompressedSize = 0;

  // Gather compressed input data from file.
  int rc = getCompressedData(sourceFileName, data, dataSize, decompressedSize);
  if (rc != 0)
  {
    return rc;
  }

  // Decompress the data.
  std::unique_ptr<uint8_t[]> decompressedData(nullptr);
  rc = decompress(data, dataSize, decompressedData, decompressedSize);
  if (rc != 0)
  {
    return rc;
  }

  // Release read buffer, because it's not needed anymore.
  data.reset();

  // Find a suitable file name to save the data.
  const auto destinationFileName = getDestinationFileName(sourceFileName);
  if (!destinationFileName.has_value())
    return rcIO;

  // Write decompressed data to destination.
  rc = writeBufferToFile(destinationFileName.value(), decompressedData, decompressedSize);
  if (rc != 0)
  {
    return rc;
  }

  std::cout << "Decompressed data was written to "
            << destinationFileName.value() << "." << std::endl;
  return 0;
}
