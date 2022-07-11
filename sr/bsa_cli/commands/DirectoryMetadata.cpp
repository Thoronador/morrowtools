/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
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

#include "DirectoryMetadata.hpp"
#include <iostream>
#include "../../base/ReturnCodes.hpp"
#include "../../base/bsa/BSA.hpp"
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
  // Some BSA files do not contain information about folder.
  // These are useless for us.
  const auto& header = bsa.getHeader();
  if (!header.hasNamesForFolders())
  {
    std::cout << "Info: The file " << bsaFileName << " does not contain "
              << "information about its folder names.\n";
    return 0;
  }
  if (!bsa.grabAllStructureData())
    return SRTP::rcFileError;

  std::cout << "hash|file count|offset|name\n"
            << "---------------------------\n";
  const auto& folder_records = bsa.getFolders();
  const auto& folder_blocks = bsa.getFolderBlocks();
  const auto folder_length = folder_records.size();
  if (folder_blocks.size() != folder_length)
  {
    std::cerr << "Error: Record and block size for folders do not match.\n"
              << "BSA file may be damaged or incomplete." << std::endl;
    return SRTP::rcDataError;
  }
  for (std::vector<BSAFolderRecord>::size_type i = 0; i < folder_length; ++i)
  {
    std::cout << "0x" << std::hex;
    // Set width and fill character for hash values.
    const auto prev_width = std::cout.width(16);
    std::cout.fill('0');
    // Print hash to stream.
    std::cout << folder_records[i].nameHash;
    // Reset width and fill character.
    std::cout.width(prev_width);
    std::cout.fill(' ');
    // Display rest of the information.
    std::cout << "|" << std::dec << folder_records[i].count
              << "|" << folder_records[i].offset << "|"
              << folder_blocks[i].folderName << "\n";
  }

  return 0;
}

std::string DirectoryMetadata::helpShort() const
{
  return "Shows metadata of the directories in the archive.";
}

std::string DirectoryMetadata::helpLong(const std::string_view binaryName) const
{
  return std::string(binaryName).append(" metadata-directories\n")
      .append("Shows metadata of the directories in an archive.\n\n")
      .append("Usage:\n    ")
      .append(binaryName).append(" metadata-directories BSA_FILE\n\n")
      .append("Options:\n    BSA_FILE    - set path to the BSA file to operate on to BSA_FILE.\n")
      .append("                  The BSA_FILE must be given.\n");
}

} // namespace
