/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2024  Dirk Stolle

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

#include <iostream>
#include <vector>
#define FUSE_USE_VERSION 26
#include <fuse.h>
#include "../../../lib/base/lz4Compression.hpp"
#include "../../../lib/base/CompressionFunctions.hpp"
#include "../../../lib/sr/ReturnCodes.hpp"
#include "bsafs.hpp"

void showVersion()
{
  std::cout << "bsafs - FUSE file system for Skyrim BSA archives, version 0.1.2, 2024-01-20\n"
            << "\nLibrary versions:\n"
            << " * fuse: " << SRTP::bsafs::fuseVersion() << "\n"
            << " * lz4:  " << MWTP::lz4Version() << "\n"
            << " * zlib: " << MWTP::zlibVersion() << "\n";
}

void showHelp()
{
  std::cout << "bsafs [OPTIONS] --archive BSA_FILE MOUNT_POINT\n"
            << "\n"
            << "options:\n"
            << "  --help             - Displays this help message and quits.\n"
            << "  -?                 - same as --help\n"
            << "  --version          - Displays the version of the program and quits.\n"
            << "  -v                 - same as --version\n"
            << "  --archive BSA_FILE - Set path to the BSA file to operate on to BSA_FILE.\n"
            << "                       The BSA_FILE must be given.\n"
            << "\n"
            << "Furthermore, some FUSE-specific parameters apply.\n";
}

int main(int argc, char* argv[])
{
  std::unique_ptr<char*[]> fuse_args = std::make_unique<char*[]>(argc + 1);
  int fuse_argc = 0;
  fuse_args[argc] = nullptr;
  if ((argc > 0) && (argv != nullptr))
  {
    fuse_argc = 1;
    fuse_args[0] = argv[0];
  }

  std::string archive_path;

  if ((argc > 1) && (argv != nullptr))
  {
    int i = 1;
    while (i < argc)
    {
      if (argv[i] != nullptr)
      {
        const std::string param = std::string(argv[i]);
        // help parameter
        if ((param == "--help") || (param == "-?") || (param == "/?"))
        {
          showHelp();
          return 0;
        }
        // version information requested?
        else if ((param == "--version") || (param == "-v"))
        {
          showVersion();
          return 0;
        }
        else if (param == "--archive")
        {
          if (!archive_path.empty())
          {
            std::cerr << "Error: Archive path was already specified.\n";
            return SRTP::rcInvalidParameter;
          }
          // enough parameters?
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            archive_path = std::string(argv[i+1]);
            ++i; // skip next parameter, because it's already used as path
          }
          else
          {
            std::cerr << "Error: You have to enter the path to a BSA file after \""
                      << param << "\".\n";
            return SRTP::rcInvalidParameter;
          }
        }
        else
        {
          // unknown parameter, probably fuse-related
          fuse_args[fuse_argc] = argv[i];
          fuse_argc++;
        }
      } // parameter exists
      else
      {
        std::cerr << "Parameter at index " << i << " is NULL.\n";
        return SRTP::rcInvalidParameter;
      }
      ++i; // on to next parameter
    }
  } // if arguments present
  else
  {
    std::cerr << "You have to specify certain parameters for this program to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return SRTP::rcInvalidParameter;
  }

  if (archive_path.empty())
  {
    std::cerr << "You have to specify the path to the archive file via the "
              << "--archive parameter for this program to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return SRTP::rcInvalidParameter;
  }

  if (!SRTP::bsafs::archive.open(archive_path))
  {
    return SRTP::rcFileError;
  }
  // Some BSA files do not contain information about directory and file names.
  // These are useless for us.
  const auto& header = SRTP::bsafs::archive.getHeader();
  if (!header.hasNamesForDirectories() || !header.hasNamesForFiles())
  {
    std::cout << "Info: The file " << archive_path << " does not contain "
              << "information about its directory names and file names."
              << "Therefore, it cannot be mounted as file system.\n";
    return 0;
  }
  if (!SRTP::bsafs::archive.grabAllStructureData())
  {
    return SRTP::rcFileError;
  }
  SRTP::bsafs::set_time_values(archive_path);

  for (int i = fuse_argc + 1; i < argc; ++i)
  {
    fuse_args[i] = nullptr;
  }

  const auto operations = SRTP::bsafs::get_operations();
  return fuse_main(fuse_argc, fuse_args.get(), &operations, nullptr);
}
