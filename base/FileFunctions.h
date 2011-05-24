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

#ifndef FILEFUNCTIONS_H
#define FILEFUNCTIONS_H

#include <string>
#include <stdint.h>

/* returns the file size of file fileName. If the file does not exist or if an
   error occured, -1 will be returned.

  parameters:
      FileName - the file whose size shall be obtained
*/
int64_t getFileSize64(const std::string& fileName);

/* sets the time when file FileName was last modified. If the file does not
   exist or if an error occured, false will be returned.

  parameters:
      FileName - the file whose time of last modification shall be changed
*/
bool setFileModificationTime(const std::string& FileName, const time_t new_mtime);

/* returns the file size of file FileName and the time of its last modification.
   If the file does not exist or if an error occured, false will be returned and
   the values will be set to -1.

  parameters:
      FileName - the file whose size shall be obtained
      FileSize - reference to the 64bit integer that will store the file's size
      FileTime - reference to the time_t that will store the file's mtime
*/
bool getFileSizeAndModificationTime(const std::string& FileName, int64_t& FileSize, time_t& FileTime);

/* returns a human readable, nicer string representing the file size passed
   as fileSize

   parameters:
       fileSize - size of the file in bytes
*/
std::string getSizeString(const int64_t fileSize);

/* Checks for existence of file FileName and returns true, if it exists.

  parameters:
      FileName - the file whose existence shall be determined
*/
bool FileExists(const std::string& FileName);

/* tries to delete the file fileName and returns true, if it could be deleted,
   or false if en error occured.

  parameters:
      fileName - the file that shall be deleted
*/
bool deleteFile(const std::string& fileName);

#endif // FILEFUNCTIONS_H
