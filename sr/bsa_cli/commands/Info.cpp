/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021, 2022, 2023  Dirk Stolle

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
#include "../../../lib/sr/FormIDFunctions.hpp"
#include "../../../lib/sr/ReturnCodes.hpp"
#include "../../../lib/sr/bsa/BSA.hpp"
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
    << "  has directory names:   " << (header.hasNamesForDirectories() ? "yes" : "no") << "\n"
    << "  has file names:        " << (header.hasNamesForFiles() ? "yes" : "no") << "\n"
    << "  compressed by default: ";
  if (header.filesCompressedByDefault())
  {
    std::cout << "yes";
    switch (header.version)
    {
      case 103:
      case 104:
           std::cout << " (zlib)";
           break;
      case 105:
           std::cout << " (lz4)";
           break;
    }
  }
  else
  {
    std::cout << "no";
  }
  std::cout << "\n"
    << "  Xbox archive:          " << (header.isXboxArchive() ? "yes" : "no") << "\n"
    << "  embedded file names:   " << (header.hasEmbeddedFileNames() ? "yes" : "no") << "\n"
    << "  uses XMem compression: " << (header.usesXMemCodec() ? "yes" : "no") << "\n"
    << "Number of directories:   " << header.directoryCount << "\n"
    << "Number of files:         " << header.fileCount << "\n"
    << "Directory name length:   " << header.totalDirectoryNameLength << "\n"
    << "File name length:        " << header.totalFileNameLength << "\n"
    << "File flags:              0x" << SRTP::getFormIDAsString(header.fileFlags) << "\n"
    << "  Meshes:                " << (header.contains(ContentType::Meshes) ? "yes" : "no") << "\n"
    << "  Textures:              " << (header.contains(ContentType::Textures) ? "yes" : "no") << "\n"
    << "  Menus:                 " << (header.contains(ContentType::Menus) ? "yes" : "no") << "\n"
    << "  Sounds:                " << (header.contains(ContentType::Sounds) ? "yes" : "no") << "\n"
    << "  Voices:                " << (header.contains(ContentType::Voices) ? "yes" : "no") << "\n"
    << "  Shaders:               " << (header.contains(ContentType::Shaders) ? "yes" : "no") << "\n"
    << "  Fonts:                 " << (header.contains(ContentType::Fonts) ? "yes" : "no") << "\n"
    << "  Miscellaneous:         " << (header.contains(ContentType::Misc) ? "yes" : "no") << "\n"
    << "(Note: File flags are only a rough indication for the actual archive content.)\n";
  return 0;
}

std::string Info::helpShort() const
{
  return "Shows header information of the archive.";
}

std::string Info::helpLong(const std::string_view binaryName) const
{
  #if defined(_WIN32)
  const auto archive = "D:\\Games\\Some-PlugIn\\archive.bsa";
  #else
  const auto archive = "/opt/games/some-plugin/archive.bsa";
  #endif
  return std::string(binaryName).append(" info\n")
      .append("Shows header information of an archive.\n\n")
      .append("Usage:\n    ")
      .append(binaryName).append(" info BSA_FILE\n\n")
      .append("Options:\n    BSA_FILE - Set path of the BSA file to operate on to BSA_FILE.\n")
      .append("               The BSA_FILE must be given.\n\n")
      .append("Example:\n")
      .append("    To show the header information of the archive\n")
      .append("    ").append(archive).append(" type:\n\n    ")
      .append(binaryName).append(" info ").append(archive).append("\n");
}

} // namespace
