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

#include "OperationList.hpp"
#include <iostream>
#include "../base/ReturnCodes.hpp"
#include "../base/bsa/BSA.hpp"

namespace SRTP
{

int listBsaContent(const std::string& fileName)
{
  BSA bsa;
  if (!bsa.open(fileName))
    return SRTP::rcFileError;
  if (!bsa.grabAllStructureData())
    return SRTP::rcFileError;
  // Some BSA files do not contain information about folder and file names.
  // These are useless for us.
  const auto& header = bsa.getHeader();
  if (!header.hasNamesForFolders() || !header.hasNamesForFiles())
  {
    std::cout << "Info: The file " << fileName << " does not contain "
              << "information about its folder names and file names.\n";
    return 0;
  }
  // List files without compression status (false).
  bsa.listFileNames(false);
  return 0;
}

} // namespace
