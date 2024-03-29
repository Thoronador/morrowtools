/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013  Dirk Stolle

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

#include "IniFunctions.hpp"
#include <cstring>
#include <iostream>
#include <fstream>
#include "../base/DirectoryFunctions.hpp"
#include "../base/FileFunctions.hpp"
#include "ReturnCodes.hpp"

namespace MWTP
{

int getFilesFromMorrowindIni(const std::string& baseDir, DepFileList& files)
{
  //check for Morrowind.ini
  if (!FileExists(baseDir+".."+MWTP::pathDelimiter+"Morrowind.ini"))
  {
    std::cout << "Couldn't find Morrowind.ini!\n";
    return rcFileError;
  }
  //now read the files from the .ini
  std::ifstream iniFile;
  iniFile.open((baseDir+".."+MWTP::pathDelimiter+"Morrowind.ini").c_str(), std::ios::in | std::ios::binary);
  if (!iniFile)
  {
    std::cout << "Could not open Morrowind.ini!\n";
    return rcFileError;
  }
  const size_t bufferSize = 512;
  char Buffer[bufferSize];
  memset(Buffer, 0, bufferSize);
  std::string ini_line = "";
  bool in_gf_section = false;
  while (iniFile.getline(Buffer, bufferSize-1))
  {
    ini_line = std::string(Buffer);
    //skip empty lines
    if (ini_line.length()>0)
    {
      //clear carriage return at end of line, if present
      if (ini_line.at(ini_line.length()-1)=='\r')
      {
        ini_line.erase(ini_line.length()-1);
      }

      if (!in_gf_section)
      {
        in_gf_section = (ini_line == "[Game Files]");
      }
      else
      {
        if (ini_line.length()>10)
        {
          if (ini_line.substr(0,8) == "GameFile")
          {
            //we've got one line
            const size_t pos = ini_line.find('=');
            if ((pos>8) &&  (pos!=std::string::npos))
            {
              files.push_back(DepFile(ini_line.substr(pos + 1, std::string::npos)));
            }
          }//if is GameFile line
        }//required minimum length?
      }//else
    }//if line not empty
    memset(Buffer, 0, bufferSize);
  }//while
  iniFile.close();
  return 0;
}//function getFilesFromMorrowindIni

} //namespace
