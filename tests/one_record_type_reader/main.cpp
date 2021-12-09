/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021  Dirk Stolle

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
#include <sstream>
#include <string>
#include "../../base/FileFunctions.hpp"
#include "../../base/DirectoryFunctions.hpp"
#include "../../base/UtilityFunctions.hpp"
#include "../../sr/base/Cells.hpp"
#include "../../sr/base/FormIDFunctions.hpp"
#include "../../sr/base/ReturnCodes.hpp"
#include "../../sr/base/records/TES4HeaderRecord.hpp"
#include "ESMReaderOneRecordType.hpp"

void showVersion()
{
  std::cout << "One Record Type Reader for Skyrim, version 0.1.0, 2021-10-07\n";
}

void showHelp()
{
  std::cout << "\none_record_type_reader -d DIRECTORY\n"
            << "\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the programme and quits\n"
            << "  -v               - same as --version\n"
            << "  -d DIRECTORY     - set path to the Data Files directory of Skyrim to\n"
            << "                     DIRECTORY\n"
            << "  -dir DIRECTORY   - same as -d\n"
            << "  -p TEXT          - set the search pattern to be TEXT.\n";
}

/* the main functions, obviously */
int main(int argc, char **argv)
{
  // data files directory
  std::string dataDir = "";

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
        else if ((param == "-d") || (param == "-dir") || (param == "--data-files"))
        {
          // Set more than once?
          if (!dataDir.empty())
          {
            std::cerr << "Error: Data directory was already set!\n";
            return SRTP::rcInvalidParameter;
          }
          // enough parameters?
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            // Is it long enough to be a directory? (Minimum should be "./".)
            if (std::string(argv[i+1]).size() > 1)
            {
              dataDir = std::string(argv[i+1]);
              ++i; //skip next parameter, because it's used as directory name already
              // Does it have a trailing (back)slash? If not, add it.
              dataDir = slashify(dataDir);
              std::cout << "Data files directory was set to \""<<dataDir<<"\".\n";
            }
            else
            {
              std::cerr << "Parameter \""<<std::string(argv[i+1])<<"\" is too"
                        << " short to be a proper directory path.\n";
              return SRTP::rcInvalidParameter;
            }
          }
          else
          {
            std::cerr << "Error: You have to specify a directory name after \""
                      << param<<"\".\n";
            return SRTP::rcInvalidParameter;
          }
        } // data files directory
        else
        {
          // unknown or wrong parameter
          std::cout << "Invalid parameter given: \"" << param << "\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return SRTP::rcInvalidParameter;
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
    std::cout << "You have to specify certain parameters for this programme to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return SRTP::rcInvalidParameter;
  }

  // Has the user specified a data directory?
  if (dataDir.empty())
  {
    std::cerr << "Error: No data directory given!\n";
    return SRTP::rcInvalidParameter;
  }

  std::vector<std::string> esmNames;
  esmNames.push_back("Skyrim.esm");
  if (FileExists(dataDir + "Update.esm"))
    esmNames.push_back("Update.esm");
  if (FileExists(dataDir + "Dawnguard.esm"))
    esmNames.push_back("Dawnguard.esm");
  if (FileExists(dataDir + "HearthFires.esm"))
    esmNames.push_back("HearthFires.esm");
  if (FileExists(dataDir + "Dragonborn.esm"))
    esmNames.push_back("Dragonborn.esm");

  SRTP::ESMReaderOneRecordType reader;
  SRTP::Tes4HeaderRecord tes4rec;

  // read the usual stuff
  for (const auto& esm: esmNames)
  {
    std::cout << "Reading " << esm << " ...\n";
    if (reader.readESM(dataDir + esm, tes4rec, std::nullopt) < 0)
    {
      std::cerr << "Error while reading "<< dataDir + esm << "!\n";
      return SRTP::rcFileError;
    }
    std::cout << "Cell records: " << SRTP::Cells::get().getNumberOfRecords() << "\n";
  }

  unsigned int totalMatches = 0;

  // check cells for matches
  std::cout << "\n\nMatching cells:\n";
  {
    SRTP::Cells::ListIterator iter = SRTP::Cells::get().begin();
    const auto end = SRTP::Cells::get().end();
    while (iter != end)
    {
      if (iter->second.unknownXNAM.has_value())
      {
        // found matching cell record
        std::cout << "        form ID "<< SRTP::getFormIDAsString(iter->second.headerFormID) << "\n";
        if (!iter->second.editorID.empty())
          std::cout << "        editor ID \"" << iter->second.editorID<<"\"\n";
        std::cout << "\n";
        ++totalMatches;
      }
      ++iter;
    }
  }

  std::cout << "\nTotal matching objects found: " << totalMatches << "\n";

  return 0;
}
