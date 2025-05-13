/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2022, 2023, 2025  Dirk Stolle

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

#include "DirectoryMetadata.hpp"
#include <iostream>
#include "../../../../lib/sr/ReturnCodes.hpp"
#include "../../../../lib/sr/bsa/BSA.hpp"
#include "ArgumentParsingUtilities.hpp"

namespace SRTP::bsa_cli
{

DirectoryMetadata::DirectoryMetadata()
: bsaFileName(std::string())
{
}

int DirectoryMetadata::parseArguments(int argc, char** argv)
{
  return parseArgumentsBsaFileNameOnly(argc, argv, bsaFileName);
}

int DirectoryMetadata::run()
{
  BSA bsa;
  if (!bsa.open(bsaFileName))
    return SRTP::rcFileError;
  // Some BSA files do not contain information about directories.
  // These are useless for us.
  const auto& header = bsa.getHeader();
  if (!header.hasNamesForDirectories())
  {
    std::cerr << "Error: The file " << bsaFileName << " does not contain "
              << "information about its directory names. The program cannot "
              << "handle such an archive.\n";
    return SRTP::rcDataError;
  }
  if (!bsa.grabAllStructureData())
    return SRTP::rcFileError;

  const auto& directory_records = bsa.getDirectories();
  const auto& directory_blocks = bsa.getDirectoryBlocks();
  const auto dir_length = directory_records.size();
  if (directory_blocks.size() != dir_length)
  {
    std::cerr << "Error: Record and block size for directories do not match.\n"
              << "BSA file may be damaged or incomplete.\n";
    return SRTP::rcDataError;
  }
  std::cout << "hash|offset|file count|name\n"
            << "---------------------------\n";
  for (std::vector<BSADirectoryRecord>::size_type i = 0; i < dir_length; ++i)
  {
    std::cout << "0x" << std::hex;
    // Set width and fill character for hash values.
    const auto prev_width = std::cout.width(16);
    std::cout.fill('0');
    // Print hash to stream.
    std::cout << directory_records[i].nameHash;
    // Reset width and fill character.
    std::cout.width(prev_width);
    std::cout.fill(' ');
    // Display rest of the information.
    std::cout << "|" << std::dec << directory_records[i].offset
              << "|" << directory_records[i].count << "|"
              << directory_blocks[i].name << "\n";
  }

  return 0;
}

std::string DirectoryMetadata::helpShort() const
{
  return "Shows metadata of the directories in the archive.";
}

std::string DirectoryMetadata::helpLong(const std::string_view binaryName) const
{
  #if defined(_WIN32)
  const auto archive = "D:\\Games\\Some-PlugIn\\archive.bsa";
  #else
  const auto archive = "/opt/games/some-plugin/archive.bsa";
  #endif
  return std::string(binaryName).append(" directory-metadata\n")
      .append("Shows metadata of the directories in an archive.\n\n")
      .append("Usage:\n    ")
      .append(binaryName).append(" directory-metadata BSA_FILE\n\n")
      .append("Options:\n    BSA_FILE - Set path to the BSA file to operate on to BSA_FILE.\n")
      .append("               The BSA_FILE must be given.\n\n")
      .append("Example:\n")
      .append("    To show the metadata of all directories in the archive\n    ")
      .append(archive).append(" type:\n\n    ").append(binaryName)
      .append(" directory-metadata ").append(archive).append("\n");
}

} // namespace
