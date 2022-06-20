/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021, 2022  Dirk Stolle

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

#include "ExtractFile.hpp"
#include <iostream>
#include "../../../base/FileFunctions.hpp"
#include "../../base/ReturnCodes.hpp"
#include "../../base/bsa/BSA.hpp"

namespace SRTP::bsa_cli
{

ExtractFile::ExtractFile()
: bsaFileName(std::string()),
  inArchiveName(std::string()),
  outputFileName(std::string())
{
}

int ExtractFile::parseArguments(int argc, char** argv)
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
      else if (inArchiveName.empty())
      {
        inArchiveName = param;
      }
      else if (outputFileName.empty())
      {
        if (FileExists(param))
        {
          std::cerr << "Error: The destination file " << param << " already exists!\n";
          return SRTP::rcInvalidParameter;
        }
        outputFileName = param;
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
  if (inArchiveName.empty())
  {
    std::cerr << "Error: The name of the file inside the archive has to be "
              << "specified after the BSA file name!\n";
    return SRTP::rcInvalidParameter;
  }
  if (outputFileName.empty())
  {
    std::cerr << "Error: A destination file name has to be specified for the extraction!\n";
    return SRTP::rcInvalidParameter;
  }

  return 0;
}

int ExtractFile::run()
{
  BSA bsa;
  if (!bsa.open(bsaFileName))
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
  if (!bsa.grabAllStructureData())
    return SRTP::rcFileError;
  if (bsa.extractFile(inArchiveName, outputFileName))
    return 0;
  return SRTP::rcFileError;
}

std::string ExtractFile::helpShort() const
{
  return "Extracts a single file from the archive.";
}

std::string ExtractFile::helpLong(const std::string_view binaryName) const
{
  return std::string(binaryName).append(" extract-file\n")
      .append("Extracts a single file from an archive.\n\n")
      .append("Usage:\n    ")
      .append(binaryName).append(" extract-file BSA_FILE ARCHIVE_MEMBER EXTRACT_DESTINATION\n\n")
      .append("Options:\n    BSA_FILE            - set path to the BSA file to operate on to BSA_FILE.\n")
      .append("                          The BSA_FILE must be given.\n")
      .append("    ARCHIVE_MEMBER      - full path and name of the file from the archive to\n")
      .append("                          extract. This option must be present.\n")
      .append("    EXTRACT_DESTINATION - destination file name for extraction. This option\n")
      .append("                          must be present. The destination file must not exist\n")
      .append("                          yet. Otherwise the extraction will not be performed.\n");
}

} // namespace
