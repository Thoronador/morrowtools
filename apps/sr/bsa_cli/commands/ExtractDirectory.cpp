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

#include "ExtractDirectory.hpp"
#include <iostream>
#include "../../../../lib/base/FileFunctions.hpp"
#include "../../../../lib/sr/ReturnCodes.hpp"
#include "../../../../lib/sr/bsa/BSA.hpp"

namespace SRTP::bsa_cli
{

ExtractDirectory::ExtractDirectory()
: bsaFileName(std::string()),
  archiveDirectoryName(std::string()),
  outputDirectoryName(std::string())
{
}

int ExtractDirectory::parseArguments(int argc, char** argv)
{
  if (argv == nullptr)
  {
    std::cerr << "Error: Parameter array pointer is NULL.\n";
    return SRTP::rcInvalidParameter;
  }

  for(int i = 2; i < argc; ++i)
  {
    if (argv[i] != nullptr)
    {
      const std::string param = std::string(argv[i]);
      if (bsaFileName.empty())
      {
        if (!FileExists(param))
        {
          std::cerr << "Error: The file " << param << " does not exist!\n";
          return SRTP::rcInvalidParameter;
        }
        bsaFileName = param;
      }
      else if (archiveDirectoryName.empty())
      {
        archiveDirectoryName = param;
      }
      else if (outputDirectoryName.empty())
      {
        if (FileExists(param))
        {
          std::cerr << "Error: The destination " << param << " already exists!\n";
          return SRTP::rcInvalidParameter;
        }
        outputDirectoryName = param;
      }
      else
      {
        // unknown or wrong parameter
        std::cerr << "Invalid parameter given: \"" << param << "\".\n"
                  << "Use --help to get a list of valid parameters.\n";
        return SRTP::rcInvalidParameter;
      }
    }
    else
    {
      std::cerr << "Error: Parameter at index " << i << " is NULL.\n";
      return SRTP::rcInvalidParameter;
    }
  }
  if (bsaFileName.empty())
  {
    std::cerr << "Error: A BSA file name has to be specified after the "
              << "command!\n";
    return SRTP::rcInvalidParameter;
  }
  if (archiveDirectoryName.empty())
  {
    std::cerr << "Error: The name of the directory to extract from the archive "
              << "has to be specified after the BSA file name!\n";
    return SRTP::rcInvalidParameter;
  }
  if (outputDirectoryName.empty())
  {
    std::cerr << "Error: A destination directory name has to be specified for the extraction!\n";
    return SRTP::rcInvalidParameter;
  }

  return 0;
}

int ExtractDirectory::run()
{
  BSA bsa;
  if (!bsa.open(bsaFileName))
    return SRTP::rcFileError;
  // Some BSA files do not contain information about directory and file names.
  // These are useless for us.
  const auto& header = bsa.getHeader();
  if (!header.hasNamesForDirectories() || !header.hasNamesForFiles())
  {
    std::cerr << "Error: The file " << bsaFileName << " does not contain "
              << "information about its directory names and file names. The "
              << "program cannot handle such an archive.\n";
    return SRTP::rcDataError;
  }
  if (!bsa.grabAllStructureData())
    return SRTP::rcFileError;
  uint32_t extractedFiles = 0;
  if (bsa.extractDirectory(archiveDirectoryName, outputDirectoryName, extractedFiles))
    return 0;
  return SRTP::rcFileError;
}

std::string ExtractDirectory::helpShort() const
{
  return "Extracts a single directory from the archive.";
}

std::string ExtractDirectory::helpLong(const std::string_view binaryName) const
{
  const std::string dir = "meshes\\armor\\glass";
  #if defined(_WIN32)
  const auto archive = "D:\\Games\\Some-PlugIn\\archive.bsa";
  const auto destination = "D:\\MyStuff\\unpacked-data";
  #else
  const auto archive = "/opt/some-plugin/archive.bsa";
  const auto destination = "/home/user/unpacked-data";
  #endif
  return std::string(binaryName).append(" extract-directory\n")
      .append("Extracts a single directory from an archive.\n\n")
      .append("Usage:\n    ")
      .append(binaryName).append(" extract-directory BSA_FILE ARCHIVE_DIRECTORY EXTRACT_DESTINATION\n\n")
      .append("Options:\n    BSA_FILE            - Set path to the BSA file to operate on to BSA_FILE.\n")
      .append("                          The BSA_FILE must be given.\n")
      .append("    ARCHIVE_DIRECTORY   - full path of the directory from the archive to\n")
      .append("                          extract. This option must be present.\n")
      .append("    EXTRACT_DESTINATION - destination directory name for extraction. This\n")
      .append("                          option must be present. The destination directory\n")
      .append("                          must not exist yet. Otherwise the extraction will not\n")
      .append("                          be performed.\n\n")
      .append("Example:\n")
      .append("    To extract all files from the directory " + dir)
      .append(" of the archive\n    ")
      .append(archive).append(" to the directory\n    ").append(destination)
      .append(" type:\n\n    ").append(binaryName)
      .append(" extract-directory ").append(archive)
      #if defined(_WIN32)
      .append(" ").append(dir).append(" ")
      #else
      .append(" '").append(dir).append("' ")
      #endif
      .append(destination).append("\n");
}

} // namespace
