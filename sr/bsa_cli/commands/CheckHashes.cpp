/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2022, 2023  Dirk Stolle

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

#include "CheckHashes.hpp"
#include <iostream>
#include "../../base/ReturnCodes.hpp"
#include "../../base/bsa/BSA.hpp"
#include "ArgumentParsingUtilities.hpp"

namespace SRTP::bsa_cli
{

void hashToStream(std::ostream& os, const BSAHash hash)
{
  os << "0x" << std::hex;
  // Set width and fill character for hash values.
  const auto prev_width = os.width(16);
  os.fill('0');
  // Print hash to stream.
  os << hash;
  // Reset width and fill character.
  os.width(prev_width);
  os.fill(' ');
}

CheckHashes::CheckHashes()
: bsaFileName(std::string())
{
}

int CheckHashes::parseArguments(int argc, char** argv)
{
  return parseArgumentsBsaFileNameOnly(argc, argv, bsaFileName);
}

int CheckHashes::run()
{
  BSA bsa;
  if (!bsa.open(bsaFileName))
    return SRTP::rcFileError;
  // Some BSA files do not contain information about directory and file names.
  // These are useless for us.
  const auto& header = bsa.getHeader();
  if (!header.hasNamesForDirectories() || !header.hasNamesForFiles())
  {
    std::cout << "Info: The file " << bsaFileName << " does not contain "
              << "information about its directory names and file names.\n";
    return 0;
  }
  if (!bsa.grabAllStructureData())
    return SRTP::rcFileError;

  bool all_hashes_good = true;
  const auto& directory_blocks = bsa.getDirectoryBlocks();
  const auto& directory_records = bsa.getDirectories();
  const auto directory_size = directory_blocks.size();
  for (std::vector<BSADirectoryBlock>::size_type i = 0; i < directory_size; ++i)
  {
    const auto& directory = directory_blocks[i];
    // Check hash of directory.
    const auto expected_dir_hash = calculateDirectoryHash(directory.name);
    if (directory_records[i].nameHash != expected_dir_hash)
    {
      all_hashes_good = false;
      std::cout << "Directory " << directory.name
                << " has wrong hash. Hash should be ";
      hashToStream(std::cout, expected_dir_hash);
      std::cout << ", but hash in archive is ";
      hashToStream(std::cout, directory_records[i].nameHash);
      std::cout << " instead.\n";
    }

    // Check hashes of all files in the directory.
    for (const BSAFileRecord& file: directory.files)
    {
      const auto expected = calculateHash(file.fileName);
      if (file.nameHash != expected)
      {
        all_hashes_good = false;
        std::cout << "File " << directory.name << '\\' << file.fileName
                  << " has wrong hash. Hash should be ";
        hashToStream(std::cout, expected);
        std::cout << ", but hash in archive is ";
        hashToStream(std::cout, file.nameHash);
        std::cout << " instead.\n";
      }
    }
  }

  if (all_hashes_good)
  {
    std::cout << "All hashes are correct.\n";
  }

  return 0;
}

std::string CheckHashes::helpShort() const
{
  return "Checks hashes of files and directories in the archive.";
}

std::string CheckHashes::helpLong(const std::string_view binaryName) const
{
  #if defined(_WIN32)
  const auto archive = "D:\\path-of\\some-file.bsa";
  #else
  const auto archive = "/path-of/some-file.bsa";
  #endif
  return std::string(binaryName).append(" check-hashes\n")
      .append("Checks hashes of files and directories in the archive.\n\n")
      .append("Usage:\n    ")
      .append(binaryName).append(" check-hashes BSA_FILE\n\n")
      .append("Options:\n")
      .append("    BSA_FILE    - Set path to the BSA file to operate on to BSA_FILE.\n")
      .append("                  The BSA_FILE must be given.\n\n")
      .append("Example:\n")
      .append("    The following command would check the hash values of the file\n")
      .append("    ").append(archive).append(":\n\n    ")
      .append(binaryName).append(" check-hashes ").append(archive)
      .append("\n\nRemarks:\n    This command is considered experimental.\n")
      .append("    Reason for that is the fact that the hash function is not ")
      .append("completely known\n    but has been reverse-engineered from ")
      .append("known hashes in official BSA files.\n    While it is accurate")
      .append(" for many files, it cannot be guaranteed to be correct\n    ")
      .append("for all BSA files.\n");
}

} // namespace
