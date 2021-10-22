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

#include <iostream>
#include <string>
#include "OperationInfo.hpp"
#include "OperationList.hpp"
#include "../base/ReturnCodes.hpp"
#include "../../base/FileFunctions.hpp"

void showVersion()
{
  std::cout << "BSA Command Line Tool for Skyrim, version 0.2.0, 2021-10-22\n";
}

void showHelp()
{
  std::cout << "bsa-cli [OPTIONS] OPERATION BSA_FILE\n"
            << "\n"
            << "options:\n"
            << "  --help       - displays this help message and quits\n"
            << "  -?           - same as --help\n"
            << "  --version    - displays the version of the program and quits\n"
            << "  -v           - same as --version\n"
            << "  OPERATION    - sets the operation that shall be performed on the BSA\n"
            << "                 file. Allowed operations are:\n"
            << "                     info - show header information\n"
            << "                     list - lists all folders and files in the archive\n"
            << "\n"
            << "                 More operations may be added in the future.\n"
            << "                 The operation must be given in most cases.\n"
            << "  BSA_FILE     - set path to the BSA file to operate on to BSA_FILE.\n"
            << "                 The BSA_FILE must be given in most cases.\n";
}

int main(int argc, char **argv)
{
  std::string operation;
  std::string bsaFileName;

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
        else if (operation.empty())
        {
          if (param != "list" && param != "info")
          {
            std::cerr << "Error: '" << param << "' is not an allowed operation.\n"
                      << "An allowed operation is 'list'.\n" ;
            return SRTP::rcInvalidParameter;
          }
          operation = param;
        }
        else if (bsaFileName.empty())
        {
          if (!FileExists(param))
          {
            std::cerr << "Error: The file " << param << " does not exist!\n";
            return SRTP::rcInvalidParameter;
          }
          bsaFileName = param;
        } // case sensitive
        else
        {
          // unknown or wrong parameter
          std::cerr << "Invalid parameter given: \"" << param << "\".\n"
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
    std::cerr << "You have to specify certain parameters for this program to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return SRTP::rcInvalidParameter;
  }

  if (operation.empty())
  {
    std::cerr << "Error: An operation has to be specified!\n"
              << "Use --help to get a list of valid parameters and operations.\n";
    return SRTP::rcInvalidParameter;
  }
  if (bsaFileName.empty())
  {
    std::cerr << "Error: A BSA file name has to be specified after the "
              << "operation!\n";
    return SRTP::rcInvalidParameter;
  }

  // Currently, only "list" and "info" are implemented as operations.
  if (operation == "list")
    return SRTP::listBsaContent(bsaFileName);
  else
    return SRTP::showBsaInfo(bsaFileName);
}
