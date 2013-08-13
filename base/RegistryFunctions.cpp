/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#include "RegistryFunctions.h"
#include <iostream>
#include <cstring>
#if defined(_WIN32)
  //Windows includes
  #include <Windows.h>
#else
  #warning There is no Windows Registry on that OS! Registry related functions \
           will always fail here!
#endif

namespace MWTP
{

bool getRegistryStringValueHKLM(std::string& theString, const std::string& subKey, const std::string& valueName)
{
  #ifndef _WIN32
  std::cout << "Info: There is no registry on non-Windows OSes.\n";
  return false;
  #else
  HKEY resultingHKEY;
  LONG res = RegOpenKeyExA(HKEY_LOCAL_MACHINE, subKey.c_str(), 0, KEY_READ, &resultingHKEY);
  if (res==ERROR_FILE_NOT_FOUND)
  {
    std::cout << "Error: Registry key \"HKLM\\"<<subKey<<"\" was not found!\n";
    return false;
  }
  if (res!=ERROR_SUCCESS)
  {
    std::cout << "Error: Could not open registry key \"HKLM\\"<<subKey<<"\"!\n";
    return false;
  }

  DWORD valueType = 0;
  DWORD valueSize = 1023;
  unsigned char * buffer = new unsigned char[1024];
  memset(buffer, 0, 1024);
  res = RegQueryValueExA(resultingHKEY, valueName.c_str(), NULL, &valueType, buffer, &valueSize);
  if (res==ERROR_MORE_DATA)
  {
    //buffer is too small, allocate the required size
    delete[] buffer;
    buffer = new unsigned char[valueSize+1];
    memset(buffer, 0, valueSize+1);
    //... and do it again!
    res = RegQueryValueExA(resultingHKEY, valueName.c_str(), NULL, &valueType, buffer, &valueSize);
  }//if
  if (res==ERROR_FILE_NOT_FOUND)
  {
    std::cout << "Error: Registry value \""<<valueName<<"\" was not found in HKLM!\n";
    RegCloseKey(resultingHKEY);
    delete[] buffer;
    buffer = NULL;
    return false;
  }
  theString = std::string((char*) buffer);
  RegCloseKey(resultingHKEY);
  delete[] buffer;
  buffer = NULL;
  return true;
  #endif
}//func

} //namespace
