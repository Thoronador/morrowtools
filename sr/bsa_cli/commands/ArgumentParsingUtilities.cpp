/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021  Thoronador

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

#include "ArgumentParsingUtilities.hpp"
#include <iostream>
#include "../../base/ReturnCodes.hpp"
#include "../../../base/FileFunctions.hpp"

namespace SRTP::bsa_cli
{

int parseArgumentsBsaFileNameOnly(int argc, char** argv, std::string& bsaFileName)
{
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
      std::cerr << "Parameter at index " << i << " is NULL.\n";
      return SRTP::rcInvalidParameter;
    }
  }
  if (bsaFileName.empty())
  {
    std::cerr << "Error: A BSA file name has to be specified after the "
              << "operation!\n";
    return SRTP::rcInvalidParameter;
  }

  return 0;
}

} // namespace
