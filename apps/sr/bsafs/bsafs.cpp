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

#include "bsafs.hpp"
#include <algorithm>
#include <cerrno>
#include <cstring>
#ifdef BSAFS_DEBUG
#include <iostream>
#endif // BSAFS_DEBUG
#include <sys/stat.h>
#include <type_traits>
#include <unistd.h>
#include "../../../lib/base/FileGuard.hpp"
#include "../../../lib/base/SlashFunctions.hpp"
#include "../../../lib/base/RandomFunctions.hpp"

namespace SRTP::bsafs
{

BSA archive;
std::time_t access_time;
std::time_t modification_time;
std::time_t status_change_time;

void set_time_values(const std::filesystem::path& bsa_path)
{
  struct stat statbuf;
  std::memset(&statbuf, 0, sizeof(statbuf));
  if (stat(bsa_path.string().c_str(), &statbuf) == 0)
  {
    access_time = statbuf.st_atime;
    modification_time = statbuf.st_mtime;
    status_change_time = statbuf.st_ctime;
  }
  else
  {
    access_time = time(nullptr);
    modification_time = time(nullptr);
    status_change_time = time(nullptr);
  }
}

std::string fuseVersion()
{
  const int ver = fuse_version();
  const int major = ver / 10;
  const int minor = ver % 10;
  return std::to_string(major).append(".").append(std::to_string(minor));
}

void set_common_attributes(struct stat& attr)
{
  attr.st_uid = getuid();
  attr.st_gid = getgid();
  attr.st_atime = access_time;
  attr.st_mtime = modification_time;
  attr.st_ctime = status_change_time;
}

void set_directory_attributes(struct stat& attr)
{
  set_common_attributes(attr);
  attr.st_nlink = 2;
  attr.st_mode = S_IFDIR | 0755;
  attr.st_size = 4096;
  attr.st_blocks = 8;
}

void set_file_attributes(struct stat& attr)
{
  set_common_attributes(attr);
  attr.st_nlink = 1;
  attr.st_mode = S_IFREG | 0644;
}

int bsa_getattr(const char * pathname, struct stat * attr)
{
  #ifdef BSAFS_DEBUG
  std::clog << "DEBUG: getattr(" << pathname << ", ...)\n";
  #endif // BSAFS_DEBUG
  if (pathname == nullptr)
  {
    return -ENOENT;
  }
  if (pathname == std::string("/"))
  {
    set_directory_attributes(*attr);
    return 0;
  }

  const std::string real_path = MWTP::flipForwardSlashes(pathname).substr(1);
  auto dir_index = archive.getIndexOfDirectory(real_path);
  if (dir_index.has_value() || archive.hasIntermediateDirectory(real_path))
  {
    set_directory_attributes(*attr);
    return 0;
  }

  std::optional<uint32_t> file_index;
  if (archive.getIndexPairForFile(real_path, dir_index, file_index))
  {
    if (dir_index.has_value() && file_index.has_value())
    {
      const auto opt_size = archive.getExtractedFileSize(dir_index.value(), file_index.value());
      if (opt_size.has_value())
      {
        const auto file_size = opt_size.value();
        set_file_attributes(*attr);
        attr->st_size = file_size;
        attr->st_blocks = file_size / 512 + (file_size % 512 != 0);
        return 0;
      }
      else
      {
        return -EIO;
      }
    }
  }

  return -ENOENT;
}

int bsa_readlink([[maybe_unused]] const char* path, [[maybe_unused]] char* buffer, [[maybe_unused]] size_t buffer_size)
{
  // There are no symbolic links in this file system.
  return -EINVAL;
}

int bsa_mknod([[maybe_unused]] const char* path, [[maybe_unused]] mode_t mode, [[maybe_unused]] dev_t device)
{
  // This is a read-only file system.
  return -EROFS;
}

int bsa_readdir(const char* path, void* buffer, fuse_fill_dir_t filler,
                [[maybe_unused]] off_t offset, [[maybe_unused]] struct fuse_file_info* info)
{
  #ifdef BSAFS_DEBUG
  std::clog << "DEBUG: readdir(" << path << ", ..., ..., offset=" << offset << ", ...)\n";
  #endif // BSAFS_DEBUG
  struct stat attr;
  std::memset(&attr, 0, sizeof(attr));
  set_directory_attributes(attr);
  filler(buffer, ".", &attr, 0);
  filler(buffer, "..", std::string(path) == "/" ? nullptr : &attr, 0);

  const std::string real_path = MWTP::flipForwardSlashes(path).substr(1);

  // Get sub-directories, if any.
  const auto directories = archive.getVirtualSubDirectories(real_path);
  for (const auto& entry: directories)
  {
    filler(buffer, entry.c_str(), &attr, 0);
  }

  // List all files in a directory.
  const auto dir_index = archive.getIndexOfDirectory(real_path);
  if (dir_index.has_value())
  {
    const auto& block = archive.getDirectoryBlocks()[dir_index.value()];
    set_file_attributes(attr);
    for (std::vector<BSAFileRecord>::size_type idx = 0; idx < block.files.size(); ++idx)
    {
      const auto size = archive.getExtractedFileSize(dir_index.value(), idx);
      if (size.has_value())
      {
        attr.st_size = size.value();
      }
      else
      {
        return -EIO;
      }
      filler(buffer, block.files[idx].fileName.c_str(), &attr, 0);
    }
  }

  return 0;
}

int bsa_mkdir([[maybe_unused]] const char* path, [[maybe_unused]] mode_t mode)
{
  // This is a read-only file system.
  return -EROFS;
}

int bsa_unlink([[maybe_unused]] const char* path)
{
  // This is a read-only file system.
  return -EROFS;
}

int bsa_rmdir([[maybe_unused]] const char* path)
{
  // This is a read-only file system.
  return -EROFS;
}

int bsa_symlink([[maybe_unused]] const char* target, [[maybe_unused]] const char* linkpath)
{
  // This is a read-only file system.
  return -EROFS;
}

int bsa_rename([[maybe_unused]] const char* old_path, [[maybe_unused]] const char* new_path)
{
  // This is a read-only file system.
  return -EROFS;
}

int bsa_link([[maybe_unused]] const char* old_path, [[maybe_unused]] const char* new_path)
{
  // This is a read-only file system.
  return -EROFS;
}

int bsa_chmod([[maybe_unused]] const char* path, [[maybe_unused]] mode_t mode)
{
  // This is a read-only file system.
  return -EROFS;
}

int bsa_chown([[maybe_unused]] const char* path, [[maybe_unused]] uid_t owner, [[maybe_unused]] gid_t group)
{
  // This is a read-only file system.
  return -EROFS;
}

int bsa_truncate([[maybe_unused]] const char* path, [[maybe_unused]] off_t length)
{
  // This is a read-only file system.
  return -EROFS;
}

std::optional<std::filesystem::path> temporary_file_path(const std::filesystem::path& directory)
{
  namespace fs = std::filesystem;

  fs::path file_path = directory / MWTP::randomAlphaNumericSequence(12).append(".tmp");
  unsigned int i = 0;
  while (fs::exists(file_path))
  {
    ++i;
    if (i > 10)
    {
      #ifdef BSAFS_DEBUG
      std::cerr << "Error: Could not find suitable temporary file path.\n";
      #endif // BSAFS_DEBUG
      return std::nullopt;
    }
    file_path = directory / MWTP::randomAlphaNumericSequence(12).append(".tmp");
  }

  return file_path;
}

int bsa_read(const char* path, char* buffer, size_t count, off_t offset,
		     [[maybe_unused]] struct fuse_file_info* info)
{
  namespace fs = std::filesystem;
  #ifdef BSAFS_DEBUG
  std::clog << "DEBUG: read(" << path << ", ..., count=" << count << ", offset="
            << offset << ", ...)\n";
  #endif // BSAFS_DEBUG

  const std::string real_path = MWTP::flipForwardSlashes(path).substr(1);
  // Check for directory.
  auto dir_index = archive.getIndexOfDirectory(real_path);
  if (dir_index.has_value() || archive.hasIntermediateDirectory(real_path))
  {
    return -EISDIR;
  }

  // Is it an existing file?
  std::optional<uint32_t> file_index = std::nullopt;
  if (!archive.getIndexPairForFile(real_path, dir_index, file_index))
  {
    return -EIO;
  }

  if (!dir_index.has_value() || !file_index.has_value())
  {
    // File does not exist.
    return -ENOENT;
  }

  const auto extracted_size = archive.getExtractedFileSize(dir_index.value(),
                                                           file_index.value());
  if (!extracted_size.has_value())
  {
    #ifdef BSAFS_DEBUG
    std::clog << "DEBUG: Could not calculate extracted file size.\n";
    #endif // BSAFS_DEBUG
    return -EIO;
  }

  if ((offset >= extracted_size) && (count > 0))
  {
    return -EIO;
  }
  std::error_code error;
  auto temp_dir_path = fs::temp_directory_path(error);
  if (error)
  {
    #ifdef BSAFS_DEBUG
    std::clog << "DEBUG: Could not get temporary directory path.\n";
    #endif // BSAFS_DEBUG
    return -EIO;
  }

  const auto file_path = temporary_file_path(temp_dir_path);
  if (!file_path.has_value())
  {
    return -EIO;
  }
  MWTP::FileGuard guard {file_path.value()};
  if (!archive.extractFile(dir_index.value(), file_index.value(), file_path.value().string()))
  {
    #ifdef BSAFS_DEBUG
    std::clog << "DEBUG: File extraction from archive failed.\n";
    #endif // BSAFS_DEBUG
    return -EIO;
  }

  std::ifstream stream(file_path.value());
  stream.seekg(offset);
  if (!stream.good())
  {
    #ifdef BSAFS_DEBUG
    std::clog << "DEBUG: Could not jump to offset " << offset << ".\n";
    #endif // BSAFS_DEBUG
    return -EIO;
  }
  using cast_type = std::make_unsigned_t<decltype(offset)>;
  const auto max_bytes = std::min(count, extracted_size.value() - static_cast<cast_type>(offset));
  stream.read(buffer, max_bytes);
  if (stream.fail())
  {
    #ifdef BSAFS_DEBUG
    std::clog << "DEBUG: Reading data to buffer failed.\n";
    std::clog << "DEBUG: eof: " << std::boolalpha << stream.eof() << ", bad: "
              << stream.bad() << ", fail: " << stream.fail() << "\n";
    #endif // BSAFS_DEBUG
    return -EIO;
  }
  const auto bytes_read = stream.gcount();
  stream.close();
  #ifdef BSAFS_DEBUG
  std::clog << "DEBUG: Transferred " << bytes_read << " of the requested "
            << count << " bytes.\n";
  #endif // BSAFS_DEBUG
  return bytes_read;
}

fuse_operations get_operations()
{
  fuse_operations ops;
  std::memset(&ops, 0, sizeof(ops));

  ops.getattr = bsa_getattr;
  ops.readlink = bsa_readlink;
  ops.mknod = bsa_mknod;
  ops.readdir = bsa_readdir;
  ops.mkdir = bsa_mkdir;
  ops.unlink = bsa_unlink;
  ops.rmdir = bsa_rmdir;
  ops.symlink = bsa_symlink;
  ops.rename = bsa_rename;
  ops.link = bsa_link;
  ops.chmod = bsa_chmod;
  ops.chown = bsa_chown;
  ops.truncate = bsa_truncate;
  ops.read = bsa_read;

  return ops;
}

} // namespace
