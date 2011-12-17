/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef DIRECTORYFUNCTIONS_H
#define DIRECTORYFUNCTIONS_H

#include <string>

namespace MWTP
{
#if defined(_WIN32)
  const char pathDelimiter = '\\';
#elif defined(__linux__) || defined(linux)
  const char pathDelimiter = '/';
#else
  #error "Unknown operating system!"
#endif
} //namespace

/* Checks for existence of directory dirName and returns true, if it exists.

  parameters:
      dirName - the directory whose existence shall be determined
*/
bool directoryExists(const std::string& dirName);

/* tries to create the directory dirName and returns true, if the directory
   could be created, false otherwise.

   parameters:
       dirName - path/name of the directory that shall be created
*/
bool createDirectory(const std::string& dirName);

/* tries to create the directory dirName and returns true, if the directory
   could be created, false otherwise. If neccessary, the required parent
   directories will be created, too.

   parameters:
       dirName - path/name of the directory that shall be created
*/
bool createDirectoryRecursive(const std::string& dirName);

#endif // DIRECTORYFUNCTIONS_H
