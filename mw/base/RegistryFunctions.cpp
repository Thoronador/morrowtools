/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2012 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "RegistryFunctions.h"
#include <iostream>
#include <cstring>
#if defined(_WIN32)
  //Windows includes
  #include <Windows.h>
#else
  #warning There is no Windows Registry on that OS!
#endif

namespace MWTP
{

bool getMorrowindPathFromRegistry(std::string& pathData)
{
  #ifndef _WIN32
  std::cout << "Info: There is no registry on non-Windows OSes.\n";
  return false;
  #else
  HKEY resultingHKEY;
  LONG res = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Bethesda Softworks\\Morrowind", 0, KEY_READ, &resultingHKEY);
  if (res==ERROR_FILE_NOT_FOUND)
  {
    std::cout << "Error: Registry key was not found!\n";
    return false;
  }
  if (res!=ERROR_SUCCESS)
  {
    std::cout << "Error: Could not open registry key!\n";
    return false;
  }

  DWORD valueType = 0;
  DWORD valueSize = 1023;
  unsigned char * buffer = new unsigned char[1024];
  memset(buffer, 0, 1024);
  res = RegQueryValueExA(resultingHKEY, "Installed Path", NULL, &valueType, buffer, &valueSize);
  if (res==ERROR_MORE_DATA)
  {
    //buffer is too small, allocate the required size
    delete[] buffer;
    buffer = new unsigned char[valueSize+1];
    memset(buffer, 0, valueSize+1);
    //... and do it again!
    res = RegQueryValueExA(resultingHKEY, "Installed Path", NULL, &valueType, buffer, &valueSize);
  }//if
  if (res==ERROR_FILE_NOT_FOUND)
  {
    std::cout << "Error: Registry value for Morrowind's install path was not found!\n";
    RegCloseKey(resultingHKEY);
    delete[] buffer;
    buffer = NULL;
    return false;
  }
  pathData = std::string((char*) buffer);
  RegCloseKey(resultingHKEY);
  delete[] buffer;
  buffer = NULL;
  return true;
  #endif
}//func

} //namespace
