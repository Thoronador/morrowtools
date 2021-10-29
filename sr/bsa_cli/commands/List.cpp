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

#include "List.hpp"
#include <iostream>
#include "../../base/ReturnCodes.hpp"
#include "../../base/bsa/BSA.hpp"
#include "ArgumentParsingUtilities.hpp"

namespace SRTP::bsa_cli
{

List::List()
: bsaFileName(std::string())
{
}

int List::parseArguments(int argc, char** argv)
{
  return parseArgumentsBsaFileNameOnly(argc, argv, bsaFileName);
}

int List::run()
{
  BSA bsa;
  if (!bsa.open(bsaFileName))
    return SRTP::rcFileError;
  if (!bsa.grabAllStructureData())
    return SRTP::rcFileError;
  // Some BSA files do not contain information about folder and file names.
  // These are useless for us.
  const auto& header = bsa.getHeader();
  if (!header.hasNamesForFolders() || !header.hasNamesForFiles())
  {
    std::cout << "Info: The file " << bsaFileName << " does not contain "
              << "information about its folder names and file names.\n";
    return 0;
  }
  // List files without compression status (false).
  bsa.listFileNames(false);
  return 0;
}

std::string List::helpShort() const
{
  return "Lists all files in the archive.";
}

std::string List::helpFull(const std::string_view binaryName) const
{
  return std::string(binaryName).append(" list\n")
      .append("Lists all files in the archive.\n\n")
      .append("Usage:\n    ")
      .append(binaryName).append(" list BSA_FILE\n\n")
      .append("Options:\n    BSA_FILE    - set path to the BSA file to operate on to BSA_FILE.\n")
      .append("                  The BSA_FILE must be given.\n");
}

} // namespace
