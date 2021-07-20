/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013  Thoronador

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
   could be created, false otherwise. If necessary, the required parent
   directories will be created, too.

   parameters:
       dirName - path/name of the directory that shall be created
*/
bool createDirectoryRecursive(const std::string& dirName);

/* adds a slash or backslash (or whatever is the path delimiter on the current
   system) to the given path, if the path is not empty and has no path delimiter
   as the last character yet.

   parameters:
       path - the path that should (possibly) have an (back)slash
*/
std::string slashify(const std::string& path);

#endif // DIRECTORYFUNCTIONS_H
