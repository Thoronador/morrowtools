/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2024  Dirk Stolle

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

#ifndef SRTP_BSAFS_BSAFS_HPP
#define SRTP_BSAFS_BSAFS_HPP

#define FUSE_USE_VERSION 39
#include <fuse3/fuse.h>
#include <sys/stat.h>
#include "../../../lib/sr/bsa/BSA.hpp"

namespace SRTP::bsafs
{

extern BSA archive; /**< global BSA instance for all file operations */

extern std::time_t access_time;        /**< last access time of the archive */
extern std::time_t modification_time;  /**< modification time of the archive */
extern std::time_t status_change_time; /**< last status change of the archive */

/** \brief Sets the values of the three time_t variables above based on the given file.
 *
 * \param bsa_path   path to the BSA file
 * \remarks This function should be called once before the file system is
 *          mounted. Otherwise the time values reported for the files are
 *          incorrect / set to the current time.
 */
void set_time_values(const std::filesystem::path& bsa_path);

/** \brief Gets version of the FUSE library.
 *
 * \return Returns a string containing FUSE's version.
 * \remark The string may only contain the major and the minor version number,
 *         because in some cases that is the only information provided by FUSE.
 *         A patch version number will not be included, i. e. even if version
 *         3.4.2 of the library is installed, it may only show version 3.4.
 */
std::string fuseVersion();

int bsa_getattr(const char* pathname, struct stat * attr, struct fuse_file_info *fi);

int bsa_readdir(const char* path, void* buffer, fuse_fill_dir_t filler,
                off_t offset, struct fuse_file_info* info, enum fuse_readdir_flags flags);

int bsa_read(const char* path, char* buffer, size_t count, off_t offset,
		     struct fuse_file_info* info);

int bsa_readlink(const char* path, char* buffer, size_t buffer_size);

// All those function change files or create new ones, but the bsafs is a read-
// only file system, so they do nothing.
int bsa_mknod(const char* path, mode_t mode, dev_t device);
int bsa_mkdir(const char* path, mode_t mode);
int bsa_unlink(const char* path);
int bsa_rmdir(const char* path);
int bsa_symlink(const char* target, const char* linkpath);
int bsa_rename(const char* old_path, const char* new_path, unsigned int flags);
int bsa_link(const char* old_path, const char* new_path);
int bsa_chmod(const char* path, mode_t mode, struct fuse_file_info *fi);
int bsa_chown(const char* path, uid_t owner, gid_t group, struct fuse_file_info *fi);
int bsa_truncate(const char* path, off_t length, struct fuse_file_info *fi);

fuse_operations get_operations();

} // namespace

#endif // SRTP_BSAFS_BSAFS_HPP
