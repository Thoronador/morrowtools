/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
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

#include "../../../locate_catch.hpp"
#include <fstream>
#include <unistd.h>
#include <utime.h>
#include "../../../../apps/sr/bsafs/bsafs.hpp"
#include "../../../../lib/base/FileGuard.hpp"
#include "../../../../lib/base/RandomFunctions.hpp"

TEST_CASE("SRTP::bsafs")
{
  using namespace SRTP::bsafs;

  SECTION("fuseVersion")
  {
    const auto version = fuseVersion();
    REQUIRE_FALSE( version.empty() );
    // We are on version 3.x, so it has to start with 3.
    REQUIRE( version.substr(0, 2) == "3." );
  }

  SECTION("file operations")
  {
    SECTION("bsa_chmod")
    {
      REQUIRE( bsa_chmod("/foo", 0777, nullptr) == -EROFS );
      REQUIRE( bsa_chmod("", 0777, nullptr) == -EROFS );
      REQUIRE( bsa_chmod(nullptr, 0777, nullptr) == -EROFS );
    }

    SECTION("bsa_chown")
    {
      REQUIRE( bsa_chown("/foo", 1234, 567, nullptr) == -EROFS );
      REQUIRE( bsa_chown("", 1234, 567, nullptr) == -EROFS );
      REQUIRE( bsa_chown(nullptr, 1234, 567, nullptr) == -EROFS );
    }

    SECTION("bsa_getattr")
    {
      SECTION("invalid paths")
      {
        struct stat buffer;

        REQUIRE( bsa_getattr("", &buffer, nullptr) == -ENOENT );
        REQUIRE( bsa_getattr(nullptr, &buffer, nullptr) == -ENOENT );
      }

      SECTION("root path is always a directory")
      {
        struct stat buffer;
        REQUIRE( bsa_getattr("/", &buffer, nullptr) == 0 );

        REQUIRE( buffer.st_uid == getuid() );
        REQUIRE( buffer.st_gid == getgid() );
        REQUIRE( buffer.st_atime == access_time );
        REQUIRE( buffer.st_mtime == modification_time );
        REQUIRE( buffer.st_ctime == status_change_time );
        REQUIRE( buffer.st_nlink == 2 );
        REQUIRE( buffer.st_mode == (S_IFDIR | 0755) );
        REQUIRE( buffer.st_size == 4096 );
        REQUIRE( buffer.st_blocks == 8 );
      }
    }

    SECTION("bsa_link")
    {
      REQUIRE( bsa_link("/foo", "/bar") == -EROFS );
      REQUIRE( bsa_link("", "/bar") == -EROFS );
      REQUIRE( bsa_link("/foo", "") == -EROFS );
      REQUIRE( bsa_link(nullptr, nullptr) == -EROFS );
    }

    SECTION("bsa_mkdir")
    {
      REQUIRE( bsa_mkdir("/foo", 0755) == -EROFS );
      REQUIRE( bsa_mkdir("", 0755) == -EROFS );
      REQUIRE( bsa_mkdir(nullptr, 0755) == -EROFS );
    }

    SECTION("bsa_mknod")
    {
      REQUIRE( bsa_mknod("/foo", S_IFREG | 0644, 0) == -EROFS );
      REQUIRE( bsa_mknod("", S_IFREG | 0644, 0) == -EROFS );
      REQUIRE( bsa_mknod(nullptr, S_IFREG | 0644, 0) == -EROFS );
    }

    SECTION("bsa_readlink")
    {
      char buffer[100];
      REQUIRE( bsa_readlink("/foo", buffer, 99) == -EINVAL );
      REQUIRE( bsa_readlink("", buffer, 99) == -EINVAL );
      REQUIRE( bsa_readlink(nullptr, buffer, 99) == -EINVAL );
    }

    SECTION("bsa_rename")
    {
      REQUIRE( bsa_rename("/foo", "/bar", 0) == -EROFS );
      REQUIRE( bsa_rename("", "/bar", 0) == -EROFS );
      REQUIRE( bsa_rename("/foo", "", 0) == -EROFS );
      REQUIRE( bsa_rename(nullptr, nullptr, 0) == -EROFS );
    }

    SECTION("bsa_rmdir")
    {
      REQUIRE( bsa_rmdir("/foo/bar") == -EROFS );
      REQUIRE( bsa_rmdir("/foo") == -EROFS );
      REQUIRE( bsa_rmdir("") == -EROFS );
      REQUIRE( bsa_rmdir(nullptr) == -EROFS );
    }

    SECTION("bsa_symlink")
    {
      REQUIRE( bsa_symlink("/foo", "/bar") == -EROFS );
      REQUIRE( bsa_symlink("", "/bar") == -EROFS );
      REQUIRE( bsa_symlink("/foo", "") == -EROFS );
      REQUIRE( bsa_symlink(nullptr, nullptr) == -EROFS );
    }

    SECTION("bsa_truncate")
    {
      REQUIRE( bsa_truncate("/foo", 123, nullptr) == -EROFS );
      REQUIRE( bsa_truncate("", 12, nullptr) == -EROFS );
      REQUIRE( bsa_truncate(nullptr, 1, nullptr) == -EROFS );
    }

    SECTION("bsa_unlink")
    {
      REQUIRE( bsa_unlink("/foo") == -EROFS );
      REQUIRE( bsa_unlink("") == -EROFS );
      REQUIRE( bsa_unlink(nullptr) == -EROFS );
    }
  }

  SECTION("get_operations")
  {
    const fuse_operations ops = get_operations();

    REQUIRE_FALSE( ops.getattr == nullptr );
    REQUIRE_FALSE( ops.read == nullptr );
    REQUIRE_FALSE( ops.readdir == nullptr );

    REQUIRE( ops.chmod == &bsa_chmod );
    REQUIRE( ops.chown == &bsa_chown );
    REQUIRE( ops.getattr == &bsa_getattr );
    REQUIRE( ops.link == &bsa_link );
    REQUIRE( ops.mkdir == &bsa_mkdir );
    REQUIRE( ops.mknod == &bsa_mknod );
    REQUIRE( ops.read == &bsa_read );
    REQUIRE( ops.readdir == &bsa_readdir );
    REQUIRE( ops.readlink == &bsa_readlink );
    REQUIRE( ops.rename == &bsa_rename );
    REQUIRE( ops.rmdir == &bsa_rmdir );
    REQUIRE( ops.symlink == &bsa_symlink );
    REQUIRE( ops.truncate == &bsa_truncate );
    REQUIRE( ops.unlink == &bsa_unlink );
  }

  SECTION("set_time_values")
  {
    SECTION("path does not exist")
    {
      const auto pre = time(nullptr);
      set_time_values("path/does-not/exist.txt");
      const auto post = time(nullptr);

      REQUIRE( pre <= access_time );
      REQUIRE( access_time <= post );

      REQUIRE( pre <= modification_time );
      REQUIRE( modification_time <= post );

      REQUIRE( pre <= status_change_time );
      REQUIRE( status_change_time <= post );
    }

    SECTION("path exists")
    {
      const auto path = std::filesystem::temp_directory_path() / (MWTP::randomAlphaNumericSequence(12) + ".tmp");
      MWTP::FileGuard guard{path};
      {
        std::ofstream stream{path, std::ios_base::out | std::ios_base::trunc};
        stream.close();
      }

      // Set access and modification time to something else.
      {
        struct utimbuf t;
        t.actime = time(nullptr) - 42;
        t.modtime = time(nullptr) - 123;
        REQUIRE( utime(path.string().c_str(), &t) == 0 );
      }

      set_time_values(path);

      struct stat buf;
      REQUIRE( stat(path.string().c_str(), &buf) == 0 );

      REQUIRE( access_time == buf.st_atime );
      REQUIRE( modification_time == buf.st_mtime );
      REQUIRE( status_change_time == buf.st_ctime );
    }
  }
}
