/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021, 2022, 2023, 2025  Dirk Stolle

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

#include "Directories.hpp"
#include <iostream>
#include "../../../../lib/sr/ReturnCodes.hpp"
#include "../../../../lib/sr/bsa/BSA.hpp"
#include "ArgumentParsingUtilities.hpp"

namespace SRTP::bsa_cli
{

Directories::Directories()
: bsaFileName(std::string())
{
}

int Directories::parseArguments(int argc, char** argv)
{
  return parseArgumentsBsaFileNameOnly(argc, argv, bsaFileName);
}

int Directories::run()
{
  BSA bsa;
  if (!bsa.open(bsaFileName))
    return SRTP::rcFileError;

  // Some BSA files do not contain information about directory and file names.
  // These are useless for us.
  const auto& header = bsa.getHeader();
  if (!header.hasNamesForDirectories())
  {
    std::cerr << "Error: The file " << bsaFileName << " does not contain "
              << "information about its directory names. The program cannot "
              << "handle such an archive.\n";
    return SRTP::rcDataError;
  }
  if (!bsa.grabDirectoryData() || !bsa.grabDirectoryBlocks())
    return SRTP::rcFileError;

  const auto directories = bsa.getDirectoryNames();
  for (const auto& element: directories)
  {
    std::cout << element << "\n";
  }
  return 0;
}

std::string Directories::helpShort() const
{
  return "Lists all directories in the archive.";
}

std::string Directories::helpLong(const std::string_view binaryName) const
{
  #if defined(_WIN32)
  const auto archive = "D:\\my_stuff\\archive.bsa";
  #else
  const auto archive = "/my_stuff/archive.bsa";
  #endif
  return std::string(binaryName).append(" directories\n")
      .append("Lists all directories in the archive.\n\n")
      .append("Usage:\n    ")
      .append(binaryName).append(" directories BSA_FILE\n\n")
      .append("Options:\n    BSA_FILE - Set path to the BSA file to operate on to BSA_FILE.\n")
      .append("               The BSA_FILE must be given.\n\n")
      .append("Example:\n")
      .append("    To list all directories in the archive ")
      .append(archive).append(" type:\n\n    ").append(binaryName)
      .append(" directories ").append(archive).append("\n");
}

} // namespace
