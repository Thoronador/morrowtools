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

#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <sys/stat.h>
#include "../../../lib/sr/bsa/BSA.hpp"

namespace SRTP::bsafs
{

extern BSA archive; /**< global BSA instance for all file operations */

/** \brief Gets version of the FUSE library.
 *
 * \return Returns a string containing FUSE's version.
 * \remark The string will only contain the major and the minor version number,
 *         because that is the only information provided by FUSE. A patch
 *         version number will not be included, i. e. even if version 3.4.2 of
 *         the library is installed, it will only show version 3.4.
 */
std::string fuseVersion();

int bsa_getattr(const char* pathname, struct stat * attr);

int bsa_readdir(const char* path, void* buffer, fuse_fill_dir_t filler,
                off_t offset, struct fuse_file_info* info);

int bsa_readlink(const char* path, char* buffer, size_t buffer_size);

// All those function change files or create new ones, but the bsafs is a read-
// only file system, so they do nothing.
int bsa_mknod(const char* path, mode_t mode, dev_t device);
int bsa_mkdir(const char* path, mode_t mode);
int bsa_unlink(const char* path);
int bsa_rmdir(const char* path);
int bsa_symlink(const char* target, const char* linkpath);
int bsa_rename(const char* old_path, const char* new_path);
int bsa_link(const char* old_path, const char* new_path);
int bsa_chmod(const char* path, mode_t mode);
int bsa_chown(const char* path, uid_t owner, gid_t group);
int bsa_truncate(const char* path, off_t length);

fuse_operations get_operations();

} // namespace

#endif // SRTP_BSAFS_BSAFS_HPP
