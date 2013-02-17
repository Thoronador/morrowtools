/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#ifndef SR_PATHFUNCTIONS_H
#define SR_PATHFUNCTIONS_H

#include <iostream>
#include <set>
#include "RegistryFunctions.h"
#include "../../base/FileFunctions.h"
#include "../../base/UtilityFunctions.h"
#include "ReturnCodes.h"

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
        std::cout << "Error: Could not find Skyrim's installation path in registry!\n";
        dataDir.clear();
      }
      else
      {
        if (!dataDir.empty())
        {
          //Does it have a trailing (back)slash?
          if (dataDir.at(dataDir.length()-1)!='\\')
          {
            dataDir = dataDir + "\\";
          }//if not backslash
          /*add data dir to path, because installed path points only to Skyrim's
            main direkctory */
          dataDir = dataDir +"Data\\";
          std::cout << "Data files directory was set to \""<<dataDir<<"\" via registry.\n";
        }
        else
        {
          std::cout << "Error: Installation path in registry is empty!\n";
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
     plugin. Returns an error code not equal to zero, if an error occured.
     Returns zero in case of success.

     parameters:
         dataDir           - path to the data directory of Skyrim
         pluginName        - name of the plugin (without extension),
                             e.g. "Skyrim" for Skyrim.esm
         languageComponent - string that will be used to return the language
                             component of the string file names

     remarks:
         The value of languageComponent is changed by this function, even if
         the an error occurs. However, the value is undefined in that case and
         does not represent a proper value.
  */
  int getLanguageComponent(const std::string& dataDir, std::string pluginName, std::string& languageComponent)
  {
    pluginName = lowerCase(pluginName);
    const std::string::size_type piNameLength = pluginName.length();

    languageComponent = "";
    std::string part_path, part_name, part_ext;

    std::vector<FileEntry> files = getDirectoryFileList(dataDir+"Strings\\");
    if (files.size()<3)
    {
      std::cout << "Error: could not find string table files for "<<pluginName<<"!\n";
      return SRTP::rcFileError;
    }

    std::set<std::string> presentStuff;

    unsigned int i;
    for (i=0; i<files.size(); ++i)
    {
      if ((!files[i].isDirectory) and (lowerCase(files[i].fileName.substr(0, piNameLength+1))==pluginName+"_"))
      {
        splitPathFileExtension(files[i].fileName, '\\', part_path, part_name, part_ext);
        if ((lowerCaseCompare(part_ext, "dlstrings")==0) or (lowerCaseCompare(part_ext, "strings")==0)
          or (lowerCaseCompare(part_ext, "ilstrings")==0))
        {
          //Do we have a language component yet?
          if (languageComponent.empty())
          {
            languageComponent = part_name.substr(piNameLength+1);
            presentStuff.insert(lowerCase(part_ext));
          }
          else
          {
            if (part_name.substr(piNameLength+1)==languageComponent)
            {
              presentStuff.insert(lowerCase(part_ext));
            }
          }//else
        }//if string file extension
      }//if file name starts with plugin name
    }//for

    if (presentStuff.size()<3)
    {
      std::cout << "Error: Could not find string table files!\n";
      return SRTP::rcFileError;
    }

    if ((!FileExists(dataDir+"Strings\\"+pluginName+"_"+languageComponent+".dlstrings"))
       or (!FileExists(dataDir+"Strings\\"+pluginName+"_"+languageComponent+".ilstrings"))
       or (!FileExists(dataDir+"Strings\\"+pluginName+"_"+languageComponent+".strings")))
    {
      std::cout << "Error: At least one string table file is missing!\n";
      return SRTP::rcFileError;
    }
    return 0;
  }//function getLanguageComponent

  /* retrieves the paths of the associated string table files for a given
     .esm/.esp file and returns zero in case of success or non-zero return code
     in case of error

     parameters:
         fileName - name of the .esm/.esp file
         files    - vector that will hold the string table files
  */
  int getAssociatedTableFiles(const std::string& fileName, std::vector<std::string>& files)
  {
    std::string part_path, part_name, part_ext;
    splitPathFileExtension(fileName, '\\', part_path, part_name, part_ext);

    int lc_return = getLanguageComponent(part_path, part_name, part_ext);
    //If return code is not zero, an error occured! We should return in that case.
    if (lc_return!=0)
      return lc_return;
    files.clear();
    files_push_back(part_path+"Strings\\"+part_name+"_"+part_ext+".dlstrings");
    files_push_back(part_path+"Strings\\"+part_name+"_"+part_ext+".ilstrings");
    files_push_back(part_path+"Strings\\"+part_name+"_"+part_ext+".strings");
    return 0;
  }//function getAssociatedTableFiles

} //namespace

#endif // SR_PATHFUNCTIONS_H
