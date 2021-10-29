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

#include "Info.hpp"
#include <iostream>
#include "../../base/FormIDFunctions.hpp"
#include "../../base/ReturnCodes.hpp"
#include "../../base/bsa/BSA.hpp"
#include "ArgumentParsingUtilities.hpp"

namespace SRTP::bsa_cli
{

Info::Info()
: bsaFileName(std::string())
{
}

int Info::parseArguments(int argc, char** argv)
{
  return parseArgumentsBsaFileNameOnly(argc, argv, bsaFileName);
}

int Info::run()
{
  BSA bsa;
  if (!bsa.open(bsaFileName))
    return SRTP::rcFileError;
  const auto& header = bsa.getHeader();
  std::cout
    << "BSA version:             " << header.version << "\n"
    << "Archive flags:           0x" << SRTP::getFormIDAsString(header.archiveFlags) << "\n"
    << "  has folder names:      " << (header.hasNamesForFolders() ? "yes" : "no") << "\n"
    << "  has file names:        " << (header.hasNamesForFiles() ? "yes" : "no") << "\n"
    << "  compressed by default: " << (header.filesCompressedByDefault() ? "yes" : "no") << "\n"
    << "Number of folders:       " << header.folderCount << "\n"
    << "Number of files:         " << header.fileCount << "\n"
    << "Folder name length:      " << header.totalFolderNameLength << "\n"
    << "File name length:        " << header.totalFileNameLength << "\n"
    << "File flags:              0x" << SRTP::getFormIDAsString(header.fileFlags) << "\n";
  return 0;
}

std::string Info::helpShort() const
{
  return "Shows header information of the archive.";
}

std::string Info::helpFull(const std::string_view binaryName) const
{
  return std::string(binaryName).append(" info\n")
      .append("Shows header information of an archive.\n\n")
      .append("Usage:\n    ")
      .append(binaryName).append(" info BSA_FILE\n\n")
      .append("Options:\n    BSA_FILE    - set path to the BSA file to operate on to BSA_FILE.\n")
      .append("                  The BSA_FILE must be given.\n");
}

} // namespace
