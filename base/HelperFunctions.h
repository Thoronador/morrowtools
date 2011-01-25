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

#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <string>

/* returns the file size of file FileName. If the file does not exist or if an
   error occured, -1 will be returned.

  parameters:
      FileName - the file whose size shall be obtained
*/
int64_t getFileSize64(const std::string& FileName);

/* Checks for existence of file FileName and returns true, if it exists.

  parameters:
      FileName - the file whose existence shall be determined
*/
bool FileExists(const std::string& FileName);

#endif // HELPERFUNCTIONS_H
