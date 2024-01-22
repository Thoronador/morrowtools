/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013, 2021, 2024  Dirk Stolle

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

#include <cstdint>
#include <ctime>
#include <optional>
#include <string>
#include <vector>

/** \brief Get the file size of file @fileName.
 *
 * \param fileName  the file whose size shall be obtained
 * \return Returns the file size of file @fileName in bytes. If the file does
 *         not exist or if an error occurred, -1 will be returned.
 */
int64_t getFileSize64(const std::string& fileName);

/** \brief Sets the time when file @FileName was last modified.
 *
 * \param FileName  the file whose time of last modification shall be changed
 * \return Returns true, if modification time was changed.
 * If the file does not exist or if an error occurred, false will be returned.
 */
bool setFileModificationTime(const std::string& FileName, const time_t new_mtime);

/** \brief Gets the file size of file @FileName and the time of its last modification.
 *
 * \param FileName  the file whose size shall be obtained
 * \param FileSize  reference to the 64 bit integer that will store the file's size
 * \param FileTime  reference to the time_t that will store the file's mtime
 * \return Returns true, if the data could be retrieved. In that case the file
 *         size will be stored in @FileSize and the modification time in @FileTime.
 *         If the file does not exist or if an error occurred, false will be
 *         returned and the values @FileSize and @FileTime will be set to -1.
 */
bool getFileSizeAndModificationTime(const std::string& FileName, int64_t& FileSize, time_t& FileTime);

/** \brief Gets a human readable, nicer string representing the file size.
 *
 * \param fileSize   size of the file in bytes
 * \return Returns a human readable, nicer string representing the file size
 *         passed as @fileSize.
 */
std::string getSizeString(const int64_t fileSize);

/** \brief Checks for existence of file @FileName and returns true, if it exists.
 *
 * \param FileName  the file whose existence shall be determined
 * \return Returns true, if the file @FileName exists.
 *         Returns false otherwise and in case of error.
 */
bool FileExists(const std::string& FileName);

/** \brief Tries to delete a file.
 *
 * \param fileName  the file that shall be deleted (can also be a directory)
 * \return Returns true, if the file could be deleted.
 *         Returns false if an error occurred.
 */
bool deleteFile(const std::string& fileName);

/** structure for file list entries */
struct FileEntry {
    std::string fileName; /**< base name of the file */
    bool isDirectory;     /**< whether it is a directory */

    /** constructor */
    FileEntry();
}; // struct

/** \brief Gets a list of all files in the given directory.
 *
 * \param Directory   the directory to search for files
 * \return Returns a vector containing one entry for each file.
 *         If an error occurred, an empty optional is returned instead.
 * \remark The function does no recursive directory traversal. It only gets the
 *         files and directories in the given directory itself, not the files in
 *         its subordinate directories.
 */
std::optional<std::vector<FileEntry> > getDirectoryFileList(const std::string& Directory);

/** \brief Splits a file name into path, file and extension.
 *
 * \param fileName       the full file name
 * \param pathSeperator  the path separator, i.e. '/' or '\\'
 * \param path           string that will be used to store the path component
 * \param name           string that will be used to store the name component
 * \param extension      string that will be used to store the extension
 *
 * \remarks
       Calling the function like splitPathFileExtension("/home/user/path/readme.txt", '/', ...)
       will give the following results:
           path: "/home/user/path/" (i.e. includes path separator at the end)
           name: "readme"
           ext.: "txt" (i.e. without dot)
*/
void splitPathFileExtension(const std::string fileName, const char pathSeperator, std::string& path, std::string& name, std::string& extension);

#endif // FILEFUNCTIONS_HPP
