/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2023  Dirk Stolle

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
#include <filesystem>
#include <string>
#include "../../../lib/base/DirectoryFunctions.hpp"
#include "../../../lib/mw/NPCs.hpp"
#include "../../../lib/mw/Races.hpp"
#include "../../../lib/mw/RegistryFunctions.hpp"
#include "../../../lib/mw/ReturnCodes.hpp"
#include "../../../lib/mw/records/TES3Record.hpp"
#include "ESMReaderNameGen.hpp"
#include "NameGenFunctions.hpp"
#include "generators/Factory.hpp"

void showHelp()
{
  std::cout << "\nname_generator_mw [-d DIRECTORY]\n"
            << "\n"
            << "options:\n"
            << "  --help             - displays this help message and quits\n"
            << "  -?                 - same as --help\n"
            << "  --version          - displays the version of the program and quits\n"
            << "  -d DIRECTORY       - set path to the Data Files directory of Morrowind to\n"
            << "                       DIRECTORY. If omitted, the path will be read from the\n"
            << "                       Windows registry or a default value will be used.\n"
            << "  -dir DIRECTORY     - same as -d\n"
            << "  --male             - generate male names\n"
            << "  --female           - generate female names\n"
            << "  --both             - generate both male and female names\n"
            << "                       If none of --male, --female or --both is given, then the\n"
            << "                       program will prompt the user to select one of those\n"
            << "                       options.\n"
            << " --count N           - generate N names.\n"
            << "                       If this option is not set, then the program will prompt\n"
            << "                       the user to enter the number of names to generate.\n";
}

void showVersion()
{
  std::cout << "Name Generator for Morrowind, version 0.4.1, 2023-12-30\n";
}

int main(int argc, char **argv)
{
  std::string dataDir = "";
  std::optional<MWTP::Gender> opt_gender;
  std::optional<uint_least16_t> opt_count;

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
        else if ((param == "-v") || (param == "--version"))
        {
          showVersion();
          return 0;
        }
        else if ((param == "-d") || (param == "-dir") || (param == "--data-files"))
        {
          // set more than once?
          if (!dataDir.empty())
          {
            std::cout << "Error: Data Files directory was already set!\n";
            return MWTP::rcInvalidParameter;
          }
          // enough parameters?
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            // Is it long enough to be a directory? (Minimum should be "./".)
            if (std::string(argv[i+1]).size()>1)
            {
              dataDir = std::string(argv[i+1]);
              ++i; // skip next parameter, because it's used as directory name already
              // Does it have a trailing (back)slash? If not, add one.
              dataDir = slashify(dataDir);
              std::cout << "Data Files directory was set to \"" << dataDir << "\".\n";
            }
            else
            {
              std::cout << "Parameter \"" << param << "\" is too"
                        << " short to be a proper directory path.\n";
              return MWTP::rcInvalidParameter;
            }
          }
          else
          {
            std::cout << "Error: You have to specify a directory name after \""
                      << param << "\".\n";
            return MWTP::rcInvalidParameter;
          }
        } // data files directory
        else if ((param == "--male") || (param == "-m"))
        {
          if (opt_gender.has_value())
          {
            std::cout << "Error: Gender of names to generate was already set!\n";
            return MWTP::rcInvalidParameter;
          }
          opt_gender = MWTP::Gender::Male;
        }
        else if ((param == "--female") || (param == "-f"))
        {
          if (opt_gender.has_value())
          {
            std::cout << "Error: Gender of names to generate was already set!\n";
            return MWTP::rcInvalidParameter;
          }
          opt_gender = MWTP::Gender::Female;
        }
        else if ((param == "--both") || (param == "-b"))
        {
          if (opt_gender.has_value())
          {
            std::cout << "Error: Gender of names to generate was already set!\n";
            return MWTP::rcInvalidParameter;
          }
          opt_gender = MWTP::Gender::Both;
        }
        else if ((param == "-c") || (param == "--count"))
        {
          // set more than once?
          if (opt_count.has_value())
          {
            std::cout << "Error: Count was already set!\n";
            return MWTP::rcInvalidParameter;
          }
          // enough parameters?
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            int16_t count = -1;
            if (!stringToShort(argv[i+1], count))
            {
              std::cout << "Error: \"" << argv[i+1] << "\" is not a valid number or is outside of the allowed range." << std::endl;
              return MWTP::rcInvalidParameter;
            }
            if (count < 1)
            {
              std::cout << "Error: Count must be larger than zero.\n";
              return MWTP::rcInvalidParameter;
            }
            opt_count = static_cast<uint_least16_t>(count);
            ++i; // skip next parameter, because that is the number
          }
          else
          {
            std::cout << "Error: You have to specify a number after \""
                      << param << "\".\n";
            return MWTP::rcInvalidParameter;
          }
        }
        else
        {
          // unknown or wrong parameter
          std::cout << "Invalid parameter given: \"" << param << "\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return MWTP::rcInvalidParameter;
        }
      } // parameter exists
      else
      {
        std::cout << "Parameter at index " << i << " is NULL.\n";
        return MWTP::rcInvalidParameter;
      }
      ++i; // on to next parameter
    } // while
  } // if arguments present

  // Has the user specified a data directory?
  if (dataDir.empty())
  {
    // No, so let's search the registry first...
    std::cout << "Warning: Data files directory of Morrowind was not specified, "
              << "will try to read it from the registry.\n";
    if (!MWTP::getMorrowindPathFromRegistry(dataDir))
    {
      std::cout << "Error: Could not find Morrowind's installation path in registry!\n";
      dataDir.clear();
    }
    else
    {
      if (!dataDir.empty())
      {
        //Does it have a trailing (back)slash? If not, add one.
        dataDir = slashify(dataDir);
        /*add Data Files dir to path, because installed path points only to
          Morrowinds's main directory */
        dataDir = dataDir + "Data Files" + MWTP::pathDelimiter;
        std::cout << "Data Files directory was set to \"" << dataDir << "\" via registry.\n";
      }
      else
      {
        std::cout << "Error: Installation path in registry is empty!\n";
      }
    }

    // check again, in case registry failed
    if (dataDir.empty())
    {
      // empty, so let's try a default value.
      dataDir = "C:\\Program Files\\Bethesda Softworks\\Morrowind\\Data Files\\";
      std::cout << "Warning: Data files directory of Morrowind was not specified, "
                << "will use default path \"" << dataDir << "\". This might not work"
                << " properly on your machine, use the parameter -d to specify "
                << "the proper path.\n";
    }
  } // if no data dir is given


  MWTP::TES3Record tes3Header;
  MWTP::ESMReaderNameGen reader;
  std::cout << "Reading Morrowind data files. This may take a while.\n";
  if (!std::filesystem::exists(dataDir + "Morrowind.esm"))
  {
    std::cerr << "Error: The file Morrowind.esm does not exist in the directory "
              << dataDir << ". Please specify the path to the Data Files "
              << "directory of your Morrowind installation via the -d "
              << "parameter.\n";
    return MWTP::rcFileError;
  }
  const auto files = { "Morrowind.esm", "Tribunal.esm", "Bloodmoon.esm" };
  for (const auto& file: files)
  {
    if (!std::filesystem::exists(dataDir + file))
    {
      continue;
    }
    std::cout << "Reading file " << file << "...\n";
    if (reader.readESM(dataDir + file, tes3Header) < 0)
    {
      std::cout << "Error while reading file \"" << dataDir + file <<"\".\nAborting.\n";
      return MWTP::rcFileError;
    }
  }

  MWTP::removeNonPlayableRaces();

  std::string raceId;
  while (raceId.empty())
  {
    raceId = MWTP::selectRace();
  }

  const auto& race = MWTP::Races::get().getRecord(raceId);
  std::cout << "Selected race is " << race.Name << " (ID \"" << raceId << "\").\n";

  while (!opt_gender.has_value())
  {
    opt_gender = MWTP::selectGender();
  }
  const auto gender = opt_gender.value();
  std::cout << "Selected gender is " << to_string(gender) << ".\n";

  const auto generator = MWTP::Factory::create(raceId, gender);
  if (generator == nullptr)
  {
    std::cout << "Note: Only Argonian, Breton, Dark Elf, High Elf, Imperial, "
              << "Khajiit, Nord, Orc, Redguard or Wood Elf names can be generated.\n";
    return 0;
  }

  // Remove records with incompatible names like "Dead adventurer".
  for (const auto& id: generator->purge())
  {
    MWTP::NPCs::get().removeRecord(id);
  }

  while (!opt_count.has_value())
  {
    opt_count = MWTP::selectCount();
  }

  const auto names = generator->generate(opt_count.value());
  for (const auto& name: names)
  {
    std::cout << name << '\n';
  }

  return 0;
}
