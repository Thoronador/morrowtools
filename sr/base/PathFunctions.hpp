/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2015, 2021  Thoronador

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
#include "RegistryFunctions.hpp"
#include "../../base/DirectoryFunctions.hpp"

namespace SRTP
{
  /** Tries to set the path to Skyrim's Data directory by reading the Skyrim
   * directory from the Windows registry. If that fails, a default value will
   * be used instead.
   *
   * \param dataDir      the string that will hold the data directory path
   * \param edition      the expected edition of Skyrim
   * \param defaultPath  default path that should be used in case of error
   *
   * \remarks The string given by reference as dataDir will only be updated, if
   *          it is empty. Non-empty strings will not be touched.
   */
  inline void getDataDir(std::string& dataDir, const std::optional<Edition> edition, const std::string& defaultPath = "C:\\Program Files\\Steam\\SteamApps\\common\\Skyrim\\Data\\")
  {
    // Has the user already specified a data directory?
    if (dataDir.empty())
    {
      // No, so let's search the registry first...
      std::cout << "Warning: Data files directory of Skyrim was not specified, "
                << "will try to read it from the registry.\n";
      if (!SRTP::getSkryrimPathFromRegistry(dataDir, edition))
      {
        std::cerr << "Error: Could not find Skyrim's installation path in registry!\n";
        dataDir.clear();
      }
      else
      {
        if (!dataDir.empty())
        {
          // Does it have a trailing (back)slash? Add one, if not.
          dataDir = slashify(dataDir);
          /* add data dir to path, because installed path points only to Skyrim's
             main directory */
          dataDir = dataDir + "Data" + MWTP::pathDelimiter;
          std::cout << "Data files directory was set to \"" << dataDir << "\" via registry.\n";
        }
        else
        {
          std::cerr << "Error: Installation path in registry is empty!\n";
        }
      }

      // check again, in case registry failed
      if (dataDir.empty())
      {
        // empty, so let's try a default value.
        dataDir = defaultPath;
        std::cout << "Warning: Data files directory of Skyrim was not specified, "
                  << "will use default path \"" << dataDir << "\". This might not work"
                  << " properly on your machine, use the appropriate program "
                  << "parameter to specify the proper path.\n";
      }
    } // if no data directory is given
  }
} // namespace

#endif // SR_PATHFUNCTIONS_HPP
