/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021  Thoronador

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

#include <catch.hpp>
#include "../../../base/DirectoryFunctions.hpp"
#include "../../../base/FileFunctions.hpp"

TEST_CASE("DirectoryFunctions")
{
  using namespace MWTP;

  const std::string delim = std::string(1, pathDelimiter);
  const std::string currentFile = std::string(__FILE__);
  const std::string test_directory = currentFile.substr(0, currentFile.size() - std::string("DirectoryFunctions.cpp").size())
          .append("test_files").append(delim);

  SECTION("createDirectory")
  {
    SECTION("creating a directory that already exists shall fail")
    {
      const std::string existing = test_directory;
      REQUIRE( directoryExists(existing) );
      // Creation fails.
      REQUIRE_FALSE( createDirectory(existing) );
    }

    SECTION("creating a new directory")
    {
      const std::string path = test_directory + delim + "new_dir_create";
      // Directory must not exist yet.
      REQUIRE_FALSE( directoryExists(path) );
      // Creation shall succeed.
      REQUIRE( createDirectory(path) );
      // Directory does exist now.
      REQUIRE( directoryExists(path) );

      // Clean up.
      REQUIRE( deleteFile(path) );
    }

    SECTION("creating multiple hierarchy levels fails")
    {
      const std::string path = test_directory + delim + "dir1" + delim + "dir2";
      // Directory must not exist yet.
      REQUIRE_FALSE( directoryExists(path) );
      // Creation shall fail.
      REQUIRE_FALSE( createDirectory(path) );
    }
  }

  SECTION("createDirectoryRecursive")
  {
    SECTION("creating a directory that already exists shall fail")
    {
      const std::string existing = test_directory;
      REQUIRE( directoryExists(existing) );
      // Creation fails.
      REQUIRE_FALSE( createDirectoryRecursive(existing) );
    }

    SECTION("creating a new directory: single level")
    {
      // Creating just one additional level is not what this function is meant
      // to do, but it can do it, too.
      const std::string path = test_directory + delim + "dir_create_rec";
      // Directory must not exist yet.
      REQUIRE_FALSE( directoryExists(path) );
      // Creation shall succeed.
      REQUIRE( createDirectory(path) );
      // Directory does exist now.
      REQUIRE( directoryExists(path) );

      // Clean up.
      REQUIRE( deleteFile(path) );
    }

    SECTION("creating multiple hierarchy levels at once")
    {
      const std::string path = test_directory + delim + "recursive_dir1" + delim + "dir2" + delim + "d3";
      // Directory must not exist yet.
      REQUIRE_FALSE( directoryExists(path) );
      // Creation shall succeed.
      REQUIRE( createDirectoryRecursive(path) );
      // Directory does exist now.
      REQUIRE( directoryExists(path) );

      // Clean up.
      REQUIRE( deleteFile(path) );
      REQUIRE( deleteFile(test_directory + delim + "recursive_dir1" + delim + "dir2") );
      REQUIRE( deleteFile(test_directory + delim + "recursive_dir1") );
    }
  }

  SECTION("directoryExists")
  {
    SECTION("known existing directories")
    {
      #if defined(__linux__) || defined(linux)
      // Both "/etc" and "/usr" are usually present on a Linux-like system.
      REQUIRE( directoryExists("/etc") );
      REQUIRE( directoryExists("/etc/") );
      REQUIRE( directoryExists("/usr") );
      REQUIRE( directoryExists("/usr/") );
      #elif defined(_WIN32)
      // Usually "C:\\Windows" is the installation location of the Windows OS.
      // Other locations are possible in theory, but nobody seems to use it.
      REQUIRE( directoryExists("C:\\Windows") );
      REQUIRE( directoryExists("C:\\Windows\\") );
      #endif
    }

    SECTION("known (probably) non-existing directories")
    {
      #if defined(__linux__) || defined(linux)
      // While these directories might exist on a Linux-like system, it is
      // rather unlikely that they do. We can never be sure, but then there is
      // no way to be sure except for deleting them, but the tests shall be as
      // unintrusive as possible.
      REQUIRE_FALSE( directoryExists("/etc/foo/bar/baz/quux") );
      REQUIRE_FALSE( directoryExists("/usr/ping/pong/missing/directory") );
      #elif defined(_WIN32)
      // While this directory might exist on a Windows system, it is rather
      // unlikely that it does. We can never be sure, but then there is no way
      // to be sure except for deleting it, but the tests shall be as
      // unintrusive as possible.
      REQUIRE_FALSE( directoryExists("C:\\Windows\\foo\\bar\\baz\\quux") );
      #endif
    }

    SECTION("normal files do not count as directory")
    {
      #if defined(__linux__) || defined(linux)
      REQUIRE_FALSE( directoryExists("/bin/sh") );
      #elif defined(_WIN32)
      REQUIRE_FALSE( directoryExists("C:\\Windows\\notepad.exe") );
      #endif
    }
  }

  SECTION("slashify")
  {
    #if defined(_WIN32)
    REQUIRE( slashify("C:\\Windows")   == "C:\\Windows\\" );
    REQUIRE( slashify("C:\\Windows\\") == "C:\\Windows\\" );
    REQUIRE( slashify("foo\\bar")   == "foo\\bar\\" );
    REQUIRE( slashify("foo\\bar\\") == "foo\\bar\\" );
    REQUIRE( slashify(".\\foo\\bar\\baz")   == ".\\foo\\bar\\baz\\" );
    REQUIRE( slashify(".\\foo\\bar\\baz\\") == ".\\foo\\bar\\baz\\" );
    #else
    REQUIRE( slashify("/opt")  == "/opt/" );
    REQUIRE( slashify("/opt/") == "/opt/" );
    REQUIRE( slashify("foo/bar")  == "foo/bar/" );
    REQUIRE( slashify("foo/bar/") == "foo/bar/" );
    REQUIRE( slashify("./foo/bar/baz")  == "./foo/bar/baz/" );
    REQUIRE( slashify("./foo/bar/baz/") == "./foo/bar/baz/" );
    #endif
  }
}
