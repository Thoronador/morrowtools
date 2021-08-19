/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2015  Thoronador

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

#ifndef SR_PATHFUNCTIONS_HPP
#define SR_PATHFUNCTIONS_HPP

#include <iostream>
#include <set>
#include "RegistryFunctions.hpp"
#include "../../base/DirectoryFunctions.hpp"
#include "../../base/FileFunctions.hpp"
#include "../../base/UtilityFunctions.hpp"
#include "ReturnCodes.hpp"

namespace SRTP
{
  /* tries to set the path to Skyrim's Data directory by reading the Skyrim
     directory from the Windows registry. If that fails, a default value will
     be used instead.

     parameters:
         dataDir     - the string that will hold the data directory path
         defaultPath - default path that should be used in case of error

     remarks:
         The string given by reference as dataDir will only be updated, if it
         is empty. Non-empty strings will not be touched.
  */
  inline void getDataDir(std::string& dataDir, const std::string& defaultPath="C:\\Program Files\\Steam\\SteamApps\\common\\skyrim\\Data\\")
  {
    //Has the user already specified a data directory?
    if (dataDir.empty())
    {
      //No, so let's search the registry first...
      std::cout << "Warning: Data files directory of Skyrim was not specified, "
                << "will try to read it from the registry.\n";
      if (!SRTP::getSkryrimPathFromRegistry(dataDir))
      {
        std::cerr << "Error: Could not find Skyrim's installation path in registry!\n";
        dataDir.clear();
      }
      else
      {
        if (!dataDir.empty())
        {
          //Does it have a trailing (back)slash? Add one, if not.
          dataDir = slashify(dataDir);
          /*add data dir to path, because installed path points only to Skyrim's
            main directory */
          dataDir = dataDir +"Data" +MWTP::pathDelimiter;
          std::cout << "Data files directory was set to \""<<dataDir<<"\" via registry.\n";
        }
        else
        {
          std::cerr << "Error: Installation path in registry is empty!\n";
        }
      }//else

      //check again, in case registry failed
      if (dataDir.empty())
      {
        //empty, so let's try a default value.
        dataDir = defaultPath;
        std::cout << "Warning: Data files directory of Skyrim was not specified, "
                  << "will use default path \""<<dataDir<<"\". This might not work"
                  << " properly on your machine, use the appropriate programme "
                  << "parameter to specify the proper path.\n";
      }
    }//if no data dir is given
  }//function getDataDir

  /* tries to find the language component of the string file names for a given
     plugin. Returns an error code not equal to zero, if an error occurred.
     Returns zero in case of success.

     parameters:
         dataDir           - path to the data directory of Skyrim
         pluginName        - name of the plugin (without extension),
                             e.g. "Skyrim" for Skyrim.esm
         languageComponent - string that will be used to return the language
                             component of the string file names
         stringTableFiles  - vector that will be used to returns the found file
                             names of the string table files

     remarks:
         The value of languageComponent is changed by this function, even if
         the an error occurs. However, the value is undefined in that case and
         does not represent a proper value.
  */
  inline int getLanguageComponent(const std::string& dataDir, const std::string& pluginName, std::string& languageComponent, std::vector<std::string>& stringTableFiles)
  {
    const std::string lcPluginName = lowerCase(pluginName);
    const std::string::size_type piNameLength = pluginName.length();

    languageComponent = "";
    std::string part_path, part_name, part_ext;

    const std::vector<FileEntry> files = getDirectoryFileList(dataDir+"Strings"+MWTP::pathDelimiter);
    if (files.size()<3)
    {
      std::cerr << "Error: could not find string table files for "<<pluginName<<"!\n";
      return SRTP::rcFileError;
    }

    std::set<std::string> presentStuff;

    unsigned int i;
    for (i=0; i<files.size(); ++i)
    {
      if ((!files[i].isDirectory) and (lowerCase(files[i].fileName.substr(0, piNameLength+1))==lcPluginName+"_"))
      {
        splitPathFileExtension(files[i].fileName, MWTP::pathDelimiter, part_path, part_name, part_ext);
        if ((lowerCaseCompare(part_ext, "dlstrings")==0) or (lowerCaseCompare(part_ext, "strings")==0)
          or (lowerCaseCompare(part_ext, "ilstrings")==0))
        {
          //Do we have a language component yet?
          if (languageComponent.empty())
          {
            languageComponent = part_name.substr(piNameLength+1);
            presentStuff.insert(dataDir+"Strings"+MWTP::pathDelimiter + files[i].fileName);
          }
          else
          {
            if (part_name.substr(piNameLength+1)==languageComponent)
            {
              presentStuff.insert(dataDir+"Strings"+MWTP::pathDelimiter + files[i].fileName);
            }
          }//else
        }//if string file extension
      }//if file name starts with plugin name
    }//for

    if (presentStuff.size()<3)
    {
      std::cerr << "Error: Could not find all three string table files!\n";
      return SRTP::rcFileError;
    }

    if (presentStuff.size()>3)
    {
      //Could possibly happen on file systems with case-insensitive file names.
      std::cerr << "Error: Found more than three string table files!\n";
      return SRTP::rcFileError;
    }

    stringTableFiles.clear();
    std::set<std::string>::const_iterator cIter = presentStuff.begin();
    while (cIter != presentStuff.end())
    {
      stringTableFiles.push_back(*cIter);
      ++cIter;
    } //while

    return 0;
  }//function getLanguageComponent

  /* retrieves the paths of the associated string table files for a given
     .esm/.esp file and returns zero in case of success or non-zero return code
     in case of error

     parameters:
         fileName - name of the .esm/.esp file
         files    - vector that will hold the string table files
  */
  inline int getAssociatedTableFiles(const std::string& fileName, std::vector<std::string>& files)
  {
    std::string part_path, part_name, part_ext;
    splitPathFileExtension(fileName, MWTP::pathDelimiter, part_path, part_name, part_ext);

    int lc_return = getLanguageComponent(part_path, part_name, part_ext, files);
    //If return code is not zero, an error occurred! We should return in that case.
    if (lc_return != 0)
      return lc_return;
    else
      return 0;
  }//function getAssociatedTableFiles

} //namespace

#endif // SR_PATHFUNCTIONS_HPP
