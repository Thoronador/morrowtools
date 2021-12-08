/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Thoronador

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
#include <string>
#include "ESMReaderCleaner.hpp"
#include "../base/ReturnCodes.hpp"
#include "../base/IniFunctions.hpp"
#include "../../base/DirectoryFunctions.hpp"
#include "../../base/FileFunctions.hpp"
#include "DirectoryTraversal.hpp"

void showHelp()
{
  std::cout << "\ndata_cleaner -d DIRECTORY [-f PLUGINFILE]\n"
            << "\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the program and quits\n"
            << "  -d DIRECTORY     - path to the Data Files directory of Morrowind\n"
            << "  -dir DIRECTORY   - same as -d\n"
            << "  --all            - tries to load all master and plugin files from the given\n"
            << "                     Data Files directory (default).\n"
            << "  --all-data-files - same as --all\n"
            << "  --explicit       - sets explicit mode where every data file that has to be\n"
            << "                     scanned has to be given explicitly. This option is\n"
            << "                     mutually exclusive with --all.\n"
            << "  -e               - same as --explicit\n"
            << "  -f PLUGINFILE    - adds the plugin PLUGINFILE to the list of files that will\n"
            << "                     be searched for data files (only in explicit mode)\n"
            << "  --ini            - tries to load list of plugin files from Morrowind.ini\n"
            << "                     (only allowed in explicit mode)\n"
            << "  -i               - same as --ini\n";
}

void showGPLNotice()
{
  std::cout << "Data Files Cleaner for Morrowind\n"
            << "  This program is part of the Morrowind Tools Project.\n"
            << "  Copyright (C) 2011, 2021  Thoronador\n"
            << "\n"
            << "  This program is free software: you can redistribute it and/or modify\n"
            << "  it under the terms of the GNU General Public License as published by\n"
            << "  the Free Software Foundation, either version 3 of the License, or\n"
            << "  (at your option) any later version.\n"
            << "\n"
            << "  This program is distributed in the hope that it will be useful,\n"
            << "  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            << "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
            << "  GNU General Public License for more details.\n"
            << "\n"
            << "  You should have received a copy of the GNU General Public License\n"
            << "  along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"
            << "\n";
}

void showVersion()
{
  std::cout << "Data Files Cleaner for Morrowind, version 0.2, 2021-12-08\n";
}

int main(int argc, char **argv)
{
  showGPLNotice();
  std::string baseDir = "C:\\Program Files\\Bethesda Softworks\\Morrowind\\Data Files\\";
  bool doIni = false;
  bool doAllPlugins = true;
  bool specifiedMode = false;

  // list of .esp files to consider for scan
  MWTP::DepFileList files;
  files.clear();

  if (argc > 1 && argv != nullptr)
  {
    int i = 1;
    while (i < argc)
    {
      if (argv[i] != nullptr)
      {
        const std::string param = std::string(argv[i]);
        // help parameter
        if (param == "--help" || param == "-?" || param == "/?")
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
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            // Is it long enough to be a directory? (Minimum should be "./".)
            if (std::string(argv[i+1]).size() > 1)
            {
              baseDir = std::string(argv[i+1]);
              ++i; // skip next parameter, because it's used as directory name already
              // Does it have a trailing (back)slash? If not, add one.
              baseDir = slashify(baseDir);
              std::cout << "Data files directory was set to \"" << baseDir
                        << "\".\n";
            }
            else
            {
              std::cerr << "Parameter \"" << std::string(argv[i+1]) << "\" is too"
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
        } // data files directory
        //add plugin file to list
        else if ((param == "-f") || (param == "--add-file"))
        {
          if (!doAllPlugins)
          {
            if ((i+1 < argc) && (argv[i+1] != nullptr))
            {
              const std::string pluginFileName = std::string(argv[i+1]);
              ++i; // skip next parameter, because it's used as file name already
              files.push_back(MWTP::DepFile(pluginFileName));
              std::cout << "Plugin file \"" << pluginFileName << "\" was added.\n";
            }
            else
            {
              std::cerr << "Error: You have to specify a file name after \""
                        << param << "\".\n";
              return MWTP::rcInvalidParameter;
            }
          }
          else
          {
            std::cerr << "Error: Parameter \"" << param << "\" can only be "
                      << "specified in explicit mode.\n";
            return MWTP::rcInvalidParameter;
          }
        } // plugin file
        // try to read from Morrowind.ini?
        else if ((param == "-i") || (param == "--ini"))
        {
          if (!doAllPlugins)
          {
            if (!doIni)
            {
              doIni = true;
            }
            else
            {
              std::cerr << "Error: Parameter \"" << param << "\" has been "
                        << "specified more than once.\n";
              return MWTP::rcInvalidParameter;
            }
          }
          else
          {
            std::cerr << "Error: Parameter \"" << param << "\" can only be "
                      << "specified in explicit mode.\n";
            return MWTP::rcInvalidParameter;
          }
        }
        // try to get all plugin and master files from Data Files directory
        else if ((param == "--all-data-files") || (param == "--all"))
        {
          if (!specifiedMode)
          {
            doAllPlugins = true;
            specifiedMode = true;
          }
          else
          {
            std::cerr << "Error: Parameter \"" << param << "\" is not allowed "
                      << "here, because mode of operation has already been "
                      << "specified.\n";
            return MWTP::rcInvalidParameter;
          }
        }
        // sets explicit mode
        else if ((param == "--explicit") || (param == "-e"))
        {
          if (!specifiedMode)
          {
            doAllPlugins = false;
            specifiedMode = true;
          }
          else
          {
            std::cerr << "Error: Parameter \"" << param << "\" is not allowed "
                      << "here, because mode of operation has already been "
                      << "specified.\n";
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
        std::cerr << "Parameter at index " << i << " is NULL.\n";
        return MWTP::rcInvalidParameter;
      }
      ++i; // on to next parameter
    }
  }

  if (doIni)
  {
    const int iniReturnCode = MWTP::getFilesFromMorrowindIni(baseDir, files);
    if (iniReturnCode != 0)
    {
      return iniReturnCode;
    }
  }
  if (doAllPlugins)
  {
    MWTP::getAllDataFiles(baseDir, files);
  }

  // Let's add master files, if necessary.
  if (!files.hasDepFile("Bloodmoon.esm") && FileExists(baseDir + "Bloodmoon.esm"))
  {
    files.push_front(MWTP::DepFile("Bloodmoon.esm"));
  }
  if (!files.hasDepFile("Tribunal.esm") && FileExists(baseDir + "Tribunal.esm"))
  {
    files.push_front(MWTP::DepFile("Tribunal.esm"));
  }
  if (!files.hasDepFile("Morrowind.esm"))
  {
    files.push_front(MWTP::DepFile("Morrowind.esm"));
  }

  // try to get file information
  for (unsigned int i = 0; i < files.getSize(); ++i)
  {
    getFileSizeAndModificationTime(baseDir + files.at(i).name, files.at(i).size,
                                   files.at(i).modified);
  }

  // sort files according to Morrowind's load order
  files.sort();
  // remove duplicate entries in list
  const unsigned int duplicates = files.removeDuplicates();
  if (duplicates != 0)
  {
    if (duplicates == 1)
    {
      std::cout << "One duplicate file has been removed from the list.\n";
    }
    else
    {
      std::cout << duplicates << " duplicates files have been removed from the list.\n";
    }
  }

  std::cout << "List of active files:\n";
  files.writeDeps();

  // read all files
  MWTP::ESMReaderCleaner reader(baseDir);
  std::cout << "Reading files, this may take a while.\n";
  unsigned int i = 0;
  while (i < files.getSize())
  {
    MWTP::TES3Record DummyHead; // It's not actually used after the read function,
                                // but readESM() needs one as parameter.
    const int read_result = reader.readESM(baseDir + files.at(i).name, DummyHead);
    if (read_result < 0)
    {
      std::cerr << "Error while reading file \"" << baseDir + files.at(i).name
                << "\".\nAborting.\n";
      return MWTP::rcFileError;
    }
    // something was read and file was not removed from list, so increase counter
    ++i;
  }

  std::cout << "Done reading. Checking for unused files...\n";

  std::set<std::string> DeletableMeshes;
  std::set<std::string> DeletableIcons;

  // get mesh files
  MWTP::getDeletableMeshes(baseDir + "Meshes\\", reader.MeshSet, DeletableMeshes);
  MWTP::getDeletableIcons(baseDir + "Icons\\", reader.IconSet, DeletableIcons);

  if (!DeletableIcons.empty() || !DeletableMeshes.empty())
  {
    std::cout << "There are " << DeletableMeshes.size() << " meshes and "
              << DeletableIcons.size() << " icons that could be deleted.\n";

    // Try to get the cumulative size of all files in question.
    int64_t sizeSum = 0;
    // ---- the meshes first
    for (const auto& mesh: DeletableMeshes)
    {
      const int64_t fs = getFileSize64(mesh);
      if (fs == -1)
      {
        sizeSum = -1;
        break;
      }
      else
      {
        sizeSum += fs;
      }
    }
    // ---- and the icons follow
    if (sizeSum != -1)
    {
      for (const auto& icon: DeletableIcons)
      {
        const int64_t fs = getFileSize64(icon);
        if (fs == -1)
        {
          sizeSum = -1;
          break;
        }
        else
        {
          sizeSum += fs;
        }
      }
      if (sizeSum != -1)
      {
        //print the size
        std::cout << "This would free " << getSizeString(sizeSum);
        if (sizeSum > 1024)
        {
          std::cout << " (" << sizeSum << " bytes)";
        }
        std::cout << ".\n";
      }
    } // if no error occurred

    std::string input;
    do
    {
      std::cout << "Do you want to delete those files (y/n)?\n";
      std::cin >> input;
      trim(input);
      input = lowerCase(input);
    } while ((input != "y") && (input != "yes") && (input != "j") && (input != "ja")
         && (input != "n") && (input != "no") && (input != "nein"));
    // Does the user want to delete the stuff?
    if ((input == "y") || (input == "yes") || (input == "j") || (input == "ja"))
    {
      std::cout << "Deleting files...\n";
      // delete meshes
      for (const auto& mesh: DeletableMeshes)
      {
        if (!deleteFile(mesh))
        {
          std::cout << "Error: Could not delete file \"" << mesh << "\".\n";
        }
      }
      // delete icons
      for (const auto& icon: DeletableIcons)
      {
        if (!deleteFile(icon))
        {
          std::cout << "Error: Could not delete file \"" << icon << "\".\n";
        }
      }
      std::cout << "Done.\n";
    }
    else
    {
      std::cout << "You chose not to delete the unused files.\n";
    }
  } // if there are any files that could be deleted
  else
  {
    std::cout << "There are no unused icons or meshes that could be deleted.\n";
  }
  return 0;
}
