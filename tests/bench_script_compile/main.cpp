/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
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
#include "../../base/DirectoryFunctions.hpp"
#include "../../mw/base/ESMReaderScriptCompiler.hpp"
#include "../../mw/base/RegistryFunctions.hpp"
#include "../../mw/base/ReturnCodes.hpp"
#include "CompileStatistics.hpp"

void showHelp()
{
  std::cout << "\nbench_script_compile -d DIRECTORY\n"
            << "\n"
            << "options:\n"
            << "  --help             - displays this help message and quits\n"
            << "  -?                 - same as --help\n"
            << "  --version          - displays the version of the programme and quits\n"
            << "  -d DIRECTORY       - set path to the Data Files directory of Morrowind to\n"
            << "                       DIRECTORY. If omitted, the path will be read from the\n"
            << "                       registry or a default value will be used.\n"
            << "  -dir DIRECTORY     - same as -d\n"
            << " --analyze ScriptID  - compiles the script ScriptID and writes the original and\n"
            << "                       the compiled version to separate files for analysis.\n";
}

void showVersion()
{
  std::cout << "Benchmark for script compiler for Morrowind, version 0.2.0, 2021-12-29\n";
}

int main(int argc, char **argv)
{
  std::string dataDir;
  std::string analyzeThis;

  if ((argc > 1) && (argv != nullptr))
  {
    int i = 1;
    while (i < argc)
    {
      if (argv[i] == nullptr)
      {
        std::cout << "Parameter at index " << i << " is NULL.\n";
        return MWTP::rcInvalidParameter;
      }

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
        if ((i + 1 < argc) && (argv[i + 1] != nullptr))
        {
          // Is it long enough to be a directory? (Minimum should be "./".)
          if (std::string(argv[i+1]).size() > 1)
          {
            dataDir = std::string(argv[i+1]);
            ++i; // skip next parameter, because it's used as directory name already
            // Does it have a trailing (back)slash? If not, add one.
            dataDir = slashify(dataDir);
            std::cout << "Data Files directory was set to \"" << dataDir << "\".\n";
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
      else if ((param == "-a") || (param == "--analyze"))
      {
        // set more than once?
        if (!analyzeThis.empty())
        {
          std::cerr << "Error: Script ID for analysis was already set!\n";
          return MWTP::rcInvalidParameter;
        }
        // enough parameters?
        if ((i + 1 < argc) && (argv[i + 1] != nullptr))
        {
          analyzeThis = std::string(argv[i+1]);
          ++i; // skip next parameter, because it's used as script ID already
          std::cout << "Script ID for analysis was set to \"" << analyzeThis
                    << "\".\n";
        }
        else
        {
          std::cerr << "Error: You have to specify a script ID after \""
                    << param << "\".\n";
          return MWTP::rcInvalidParameter;
        }
      }
      else
      {
        // unknown or wrong parameter
        std::cout << "Invalid parameter given: \""<<param<<"\".\n"
                  << "Use --help to get a list of valid parameters.\n";
        return MWTP::rcInvalidParameter;
      }
      ++i; // on to next parameter
    } // while
  } // if arguments present
  else
  {
    std::cout << "You have to specify certain parameters for this programme to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return MWTP::rcInvalidParameter;
  }

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
        // Does it have a trailing (back)slash? If not, add one.
        dataDir = slashify(dataDir);
        /*add Data Files dir to path, because installed path points only to
          Morrowinds's main direkctory */
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
  } // if no data directory is given

  MWTP::ESMReaderScriptCompiler reader;
  MWTP::TES3Record header;
  std::cout << "Reading Morrowind.esm. This may take a while.\n";
  if (reader.readESM(dataDir + "Morrowind.esm", header) < 0)
  {
    std::cout << "Error while reading file \"" << dataDir << "Morrowind.esm\".\nAborting.\n";
    return MWTP::rcFileError;
  }

  if (analyzeThis.empty())
    MWTP::displayCompileStatistics();
  else
    MWTP::compileSingleScript(analyzeThis);

  return 0;
}
