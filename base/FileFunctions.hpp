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

#ifndef FILEFUNCTIONS_HPP
#define FILEFUNCTIONS_HPP

#include <string>
#include <stdint.h>
#include <vector>

/* returns the file size of file fileName. If the file does not exist or if an
   error occurred, -1 will be returned.

  parameters:
      FileName - the file whose size shall be obtained
*/
int64_t getFileSize64(const std::string& fileName);

/* sets the time when file FileName was last modified. If the file does not
   exist or if an error occurred, false will be returned.

  parameters:
      FileName - the file whose time of last modification shall be changed
*/
bool setFileModificationTime(const std::string& FileName, const time_t new_mtime);

/* returns the file size of file FileName and the time of its last modification.
   If the file does not exist or if an error occurred, false will be returned and
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
   or false if en error occurred.

  parameters:
      fileName - the file that shall be deleted
*/
bool deleteFile(const std::string& fileName);

/* structure for file list entries */
struct FileEntry {
    std::string fileName;
    bool isDirectory;

    /* constructor */
    FileEntry();
};//struct

/* returns a list of all files in the given directory as a vector */
std::vector<FileEntry> getDirectoryFileList(const std::string& Directory);

/* splits the file name in fileName into path, file and extension

   parameters:
       fileName      - the full file name
       pathSeperator - the path separator, i.e. '/' or '\\'
       path          - string that will be used to store the path component
       name          - string that will be used to store the name component
       extension     - string that will be used to store the extension

   remarks:
       Calling the function like splitPathFileExtension("/home/user/path/readme.txt", '/', ...)
       will give the following results:
           path: "/home/user/path/" (i.e. includes path separator at the end)
           name: "readme"
           ext.: "txt" (i.e. without dot)
*/
void splitPathFileExtension(const std::string fileName, const char pathSeperator, std::string& path, std::string& name, std::string& extension);

#endif // FILEFUNCTIONS_HPP
