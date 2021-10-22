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

#include "OperationInfo.hpp"
#include <iostream>
#include "../base/FormIDFunctions.hpp"
#include "../base/ReturnCodes.hpp"
#include "../base/bsa/BSA.hpp"

namespace SRTP
{

int showBsaInfo(const std::string& fileName)
{
  BSA bsa;
  if (!bsa.open(fileName))
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

} // namespace
