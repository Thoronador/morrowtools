/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for the Morrowind Tools Project.
    Copyright (C) 2022  Dirk Stolle

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
#include "../../lib/base/DirectoryFunctions.hpp"
#include "../../lib/mw/Cells.hpp"
#include "../../lib/mw/ReturnCodes.hpp"
#include "ESMReaderCells.hpp"

void showVersion()
{
  std::cout << "Cell Reader for Morrowind, version 0.1.0, 2022-02-06\n";
}

void showHelp()
{
  std::cout << "\ncell_reader -d DIRECTORY\n"
            << "\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the program and quits\n"
            << "  -v               - same as --version\n"
            << "  -d DIRECTORY     - set path to the Data Files directory of Morrowind to\n"
            << "                     DIRECTORY.\n"
            << "  -dir DIRECTORY   - same as -d\n";
}

int main(int argc, char **argv)
{
  std::string dataDir = ""; //Data Files directory - empty at start

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
        else if (param == "--version")
        {
          showVersion();
          return 0;
        }
        else if ((param == "-d") || (param == "-dir") || (param == "--data-files"))
        {
          // set more than once?
          if (!dataDir.empty())
          {
            std::cerr << "Error: Data Files directory was already set!\n";
            return MWTP::rcInvalidParameter;
          }
          // enough parameters?
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            // Is it long enough to be a directory? (Minimum should be "./".)
            if (std::string(argv[i+1]).size() > 1)
            {
              dataDir = std::string(argv[i+1]);
              ++i; // skip next parameter, because it's used as directory name already
              // Does it have a trailing (back)slash? If not, add one.
              dataDir = slashify(dataDir);
              std::cout << "Data Files directory was set to \""<<dataDir<<"\".\n";
            }
            else
            {
              std::cerr << "Parameter \"" << param << "\" is too"
                        << " short to be a proper directory path.\n";
              return MWTP::rcInvalidParameter;
            }
          }
          else
          {
            std::cerr << "Error: You have to specify a directory name after \""
                      << param << "\".\n";
            return MWTP::rcInvalidParameter;
          }
        }
        else
        {
          // unknown or wrong parameter
          std::cerr << "Invalid parameter given: \"" << param << "\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return MWTP::rcInvalidParameter;
        }
      } // parameter exists
      else
      {
        std::cerr << "Parameter at index " << i << " is NULL.\n";
        return MWTP::rcInvalidParameter;
      }
      ++i; // on to next parameter
    } // while
  } // if arguments are present
  else
  {
    std::cout << "You have to specify certain parameters for this program to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return MWTP::rcInvalidParameter;
  }

  // Has the user specified a data directory?
  if (dataDir.empty())
  {
    std::cerr << "Error: Data files directory of Morrowind was not specified, "
              << "will try to read it from the registry.\n";
     return MWTP::rcInvalidParameter;
  }

  MWTP::ESMReaderCells reader;
  MWTP::TES3Record tes3Header;
  if (reader.readESM(dataDir + "Morrowind.esm", tes3Header) < 0)
  {
    std::cout << "Error while reading file \"" << dataDir << "Morrowind.esm\".\nAborting.\n";
    return MWTP::rcFileError;
  }

  uint32_t minSize = 0xFFFFFFFF;

  // check cells for matches
  std::cout << "\n\nMatching cells:\n";
  {
    auto iter = MWTP::Cells::get().begin();
    const auto end = MWTP::Cells::get().end();
    while (iter != end)
    {
      if (iter->second.ColourRef.has_value())
      {
        // found matching cell record
        const uint32_t current_size = iter->second.getWriteSize();
        if (current_size < minSize)
        {
          std::cout << "        Name: '"<< iter->second.CellID << "'\n";
          if (!iter->second.isInterior())
            std::cout << "        coordinates: [" << iter->second.GridX << "," << iter->second.GridY <<"]\n";
          std::cout << "        Size: "<< current_size << " bytes\n";
          std::cout << "\n";
          minSize = current_size;
        }
      }
      ++iter;
    }
  }

  return 0;
}
