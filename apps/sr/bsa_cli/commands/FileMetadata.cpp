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

#include "FileMetadata.hpp"
#include <iostream>
#include "../../../../lib/sr/ReturnCodes.hpp"
#include "../../../../lib/sr/bsa/BSA.hpp"
#include "ArgumentParsingUtilities.hpp"

namespace SRTP::bsa_cli
{

FileMetadata::FileMetadata()
: bsaFileName(std::string())
{
}

int FileMetadata::parseArguments(int argc, char** argv)
{
  return parseArgumentsBsaFileNameOnly(argc, argv, bsaFileName);
}

int FileMetadata::run()
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

  std::cout << "hash|offset|block size|compression toggled|file size|file name\n"
            << "--------------------------------------------------------------\n";
  const auto& directories = bsa.getDirectoryBlocks();
  const auto dir_size = directories.size();
  for (std::vector<BSADirectoryBlock>::size_type directory_idx = 0; directory_idx < dir_size; ++directory_idx)
  {
    const auto& directory = directories[directory_idx];
    const auto file_count = directory.files.size();
    for (std::vector<BSAFileRecord>::size_type file_idx = 0; file_idx < file_count; ++file_idx)
    {
      std::cout << "0x" << std::hex;
      // Set width and fill character for hash values.
      const auto prev_width = std::cout.width(16);
      std::cout.fill('0');
      // Print hash to stream.
      const BSAFileRecord& file = directory.files[file_idx];
      std::cout << file.nameHash;
      // Reset width and fill character.
      std::cout.width(prev_width);
      std::cout.fill(' ');
      // Display rest of the information.
      std::cout << "|" << std::dec << file.offset
                << "|" << file.getRealFileBlockSize() << "|"
                << (file.isCompressionToggled() ? "yes" : "no") << "|";
      const auto extracted = bsa.getExtractedFileSize(directory_idx, file_idx);
      if (extracted.has_value())
      {
        std::cout << extracted.value();
      }
      else
      {
        std::cout << "unknown";
      }
      std::cout << "|" << directory.name << '\\' << file.fileName << "\n";
    }
  }

  return 0;
}

std::string FileMetadata::helpShort() const
{
  return "Shows metadata of the files in the archive.";
}

std::string FileMetadata::helpLong(const std::string_view binaryName) const
{
  #if defined(_WIN32)
  const auto archive = "D:\\Games\\Some-PlugIn\\archive.bsa";
  #else
  const auto archive = "/opt/games/some-plugin/archive.bsa";
  #endif
  return std::string(binaryName).append(" file-metadata\n")
      .append("Shows metadata of the files in an archive.\n\n")
      .append("Usage:\n    ")
      .append(binaryName).append(" file-metadata BSA_FILE\n\n")
      .append("Options:\n    BSA_FILE - Set path to the BSA file to operate on to BSA_FILE.\n")
      .append("               The BSA_FILE must be given.\n\n")
      .append("Example:\n")
      .append("    To show the metadata of all files in the archive\n    ")
      .append(archive).append(" type:\n\n    ").append(binaryName)
      .append(" file-metadata ").append(archive).append("\n");
}

} // namespace
