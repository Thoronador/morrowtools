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
#include <ctime>
#include <fstream>
#include "../../../base/DirectoryFunctions.hpp"
#include "../../../base/FileFunctions.hpp"

TEST_CASE("FileFunctions")
{
  const std::string delim = std::string(1, MWTP::pathDelimiter);
  const std::string currentFile = std::string(__FILE__);
  const std::string test_directory = currentFile.substr(0, currentFile.size() - std::string("FileFunctions.cpp").size())
          .append("test_files").append(delim);

  SECTION("deleteFile")
  {
    SECTION("delete existing file")
    {
      const auto fileName = test_directory + "will-be-deleted.txt";
      // File has to exist before deletion.
      REQUIRE( FileExists(fileName) );
      // Deletion should succeed.
      REQUIRE( deleteFile(fileName) );
      // File does not exist after deletion.
      REQUIRE_FALSE( FileExists(fileName) );

      // Clean up: restore file.
      std::ofstream fs(fileName, std::ios::out | std::ios::trunc);
      fs.close();
    }

    SECTION("non-existing file")
    {
      // If there is no file to delete, it should fail.
      REQUIRE_FALSE( deleteFile(test_directory + "is-not.there") );
    }
  }

  SECTION("FileExists")
  {
    SECTION("known existing files")
    {
      #if defined(__linux__) || defined(linux)
      REQUIRE( FileExists("/bin/sh") );
      REQUIRE( FileExists("/bin/chown") );
      #elif defined(_WIN32)
      // Usually "C:\\Windows" is the installation location of the Windows OS.
      // Other locations are possible in theory, but nobody seems to use it.
      REQUIRE( FileExists("C:\\Windows\\notepad.exe") );
      REQUIRE( FileExists("C:\\Windows\\regedit.exe") );
      #endif
    }

    SECTION("known (probably) non-existing files")
    {
      #if defined(__linux__) || defined(linux)
      // While these files might exist on a Linux-like system, it is rather
      // unlikely that they do. We can never be sure, but then there is no way
      // to be sure except for deleting them, but the tests shall be as
      // unintrusive as possible.
      REQUIRE_FALSE( FileExists("/bin/foo-bar-baz-quux") );
      REQUIRE_FALSE( FileExists("/usr/ping/pong/missing/directory") );
      #elif defined(_WIN32)
      // While these files might exist on a Windows system, it is rather
      // unlikely that they do. We can never be sure, but then there is no way
      // to be sure except for deleting them, but the tests shall be as
      // unintrusive as possible.
      REQUIRE_FALSE( FileExists("C:\\Windows\\foo_bar.baz") );
      #endif
    }
  }

  SECTION("getDirectoryFileList")
  {
    SECTION("directory 'list' in test_files")
    {
      const auto vec = getDirectoryFileList(test_directory + "list");

      REQUIRE( vec.size() == 5 );
      // directory ".."
      auto iter = std::find_if(vec.begin(), vec.end(), [] (FileEntry e) { return e.fileName == ".."; });
      REQUIRE( iter != vec.end() );
      REQUIRE( iter->isDirectory );
      REQUIRE( iter->fileName == ".." );
      // directory "."
      iter = std::find_if(vec.begin(), vec.end(), [] (FileEntry e) { return e.fileName == "."; });
      REQUIRE( iter != vec.end() );
      REQUIRE( iter->isDirectory );
      REQUIRE( iter->fileName == "." );
      // directory "d"
      iter = std::find_if(vec.begin(), vec.end(), [] (FileEntry e) { return e.fileName == "d"; });
      REQUIRE( iter != vec.end() );
      REQUIRE( iter->isDirectory );
      REQUIRE( iter->fileName == "d" );
      // file "a.txt"
      iter = std::find_if(vec.begin(), vec.end(), [] (FileEntry e) { return e.fileName == "a.txt"; });
      REQUIRE( iter != vec.end() );
      REQUIRE_FALSE( iter->isDirectory );
      REQUIRE( iter->fileName == "a.txt" );
      // file "empty.jpg"
      iter = std::find_if(vec.begin(), vec.end(), [] (FileEntry e) { return e.fileName == "empty.jpg"; });
      REQUIRE( iter != vec.end() );
      REQUIRE_FALSE( iter->isDirectory );
      REQUIRE( iter->fileName == "empty.jpg" );

      // The file "d/no_content.txt" is within a sub-directory and should not be listed.
      iter = std::find_if(vec.begin(), vec.end(), [] (FileEntry e) { return e.fileName == "d/no_content.txt"; });
      REQUIRE( iter == vec.end() );
      iter = std::find_if(vec.begin(), vec.end(), [] (FileEntry e) { return e.fileName == "d\\no_content.txt"; });
      REQUIRE( iter == vec.end() );
    }

    SECTION("non-existing directory")
    {
      const auto vec = getDirectoryFileList(test_directory + "list_missing");
      // If there is no matching directory, the result is empty.
      REQUIRE( vec.empty() );
    }
  }

  SECTION("getFileSize64")
  {
    REQUIRE( getFileSize64(test_directory + "size_five.dat") == 5 );
    REQUIRE( getFileSize64(test_directory + "size_42.dat") == 42 );
    // "Size" of file that does not exist is -1. i. e. indicates error.
    #if defined(__linux__) || defined(linux)
    REQUIRE( getFileSize64("/bin/foo-bar-baz-quux") == -1 );
    #elif defined(_WIN32)
    REQUIRE( getFileSize64("C:\\Windows\\foo_bar.baz") == -1 );
    #endif
  }

  SECTION("getFileSizeAndModificationTime")
  {
    SECTION("normal operation on existing files")
    {
      // Set some given modification time.
      // The time cannot be predetermined, because we do not know when the last
      // git checkout took place.
      const std::time_t now = std::time(nullptr);
      const auto timeFive = now - 120; // 120 seconds ago
      const auto timeFortyTwo = now - 3600; // one hour ago
      REQUIRE( setFileModificationTime(test_directory + "size_five.dat", timeFive ) );
      REQUIRE( setFileModificationTime(test_directory + "size_42.dat", timeFortyTwo ) );

      int64_t size = 0;
      std::time_t modificationTime = now;

      REQUIRE( getFileSizeAndModificationTime(test_directory + "size_five.dat", size, modificationTime) );
      REQUIRE( size == 5 );
      REQUIRE( modificationTime == timeFive );

      REQUIRE( getFileSizeAndModificationTime(test_directory + "size_42.dat", size, modificationTime) );
      REQUIRE( size == 42 );
      REQUIRE( modificationTime == timeFortyTwo );
    }

    SECTION("missing file")
    {
      int64_t size = 0;
      std::time_t modificationTime = std::time(nullptr);

      REQUIRE_FALSE( getFileSizeAndModificationTime(test_directory + "does-not-ex.ist", size, modificationTime) );
      // Size and mTime are returned as negative one.
      REQUIRE( size == -1 );
      REQUIRE( modificationTime == static_cast<std::time_t>(-1) );
    }
  }

  SECTION("getSizeString")
  {
    REQUIRE( getSizeString(0) == "0 byte" );
    REQUIRE( getSizeString(1) == "1 byte" );
    REQUIRE( getSizeString(123) == "123 byte" );
    REQUIRE( getSizeString(1025) == "1 KB" );
    REQUIRE( getSizeString(4096) == "4 KB" );
    REQUIRE( getSizeString(1024 * 1024 + 1) == "1 MB" );
    REQUIRE( getSizeString(1024 * 1024 * 8) == "8 MB" );
    REQUIRE( getSizeString(1024 * 1024 * 3.75) == "3.75 MB" );
    REQUIRE( getSizeString(1024 * 1024 * 12.5) == "12.5 MB" );
    REQUIRE( getSizeString(1024 * 1024 * 1024 + 1) == "1 GB" );
    REQUIRE( getSizeString(1024 * 1024 * 1024 * 12.5) == "12.5 GB" );

    REQUIRE( getSizeString(-1) == "-1 byte" );
    REQUIRE( getSizeString(-123) == "-123 byte" );
    REQUIRE( getSizeString(-1025) == "-1 KB" );
    REQUIRE( getSizeString(-4096) == "-4 KB" );
    REQUIRE( getSizeString(-1024 * 1024 - 1) == "-1 MB" );
    REQUIRE( getSizeString(1024 * 1024 * -8) == "-8 MB" );
    REQUIRE( getSizeString(1024 * 1024 * -3.75) == "-3.75 MB" );
    REQUIRE( getSizeString(1024 * 1024 * -12.5) == "-12.5 MB" );
    REQUIRE( getSizeString(-1024 * 1024 * 1024 - 1) == "-1 GB" );
    REQUIRE( getSizeString(1024 * 1024 * 1024 * -12.5) == "-12.5 GB" );
  }

  SECTION("setFileModificationTime")
  {
    SECTION("normal operation on existing files")
    {
      int64_t size = -1;
      std::time_t modificationTime = static_cast<std::time_t>(-1);
      // Get the mTime before it was changed.
      REQUIRE( getFileSizeAndModificationTime(test_directory + "size_five.dat", size, modificationTime) );
      const std::time_t before = modificationTime;

      // Set some given modification time.
      const auto timeSet = std::time(nullptr) - 5400; // 5400 seconds ago
      REQUIRE( setFileModificationTime(test_directory + "size_five.dat", timeSet ) );

      // Get the mTime after it was changed.
      REQUIRE( getFileSizeAndModificationTime(test_directory + "size_five.dat", size, modificationTime) );
      const std::time_t after = modificationTime;
      // Must match the set time.
      REQUIRE( timeSet == after );
      // Should not match the time before.
      REQUIRE_FALSE( before == after );
    }

    SECTION("operation on missing file should fail")
    {
      const std::time_t modificationTime = std::time(nullptr) - 5400;
      REQUIRE_FALSE( setFileModificationTime(test_directory + "does-not-ex.ist", modificationTime) );
    }
  }

  SECTION("splitPathFileExtension")
  {
    std::string path;
    std::string name;
    std::string extension;

    splitPathFileExtension("/home/user/path/readme.txt", '/', path, name, extension );
    REQUIRE( path == "/home/user/path/" );
    REQUIRE( name == "readme" );
    REQUIRE( extension == "txt" );

    splitPathFileExtension("/home/user/readme", '/', path, name, extension );
    REQUIRE( path == "/home/user/" );
    REQUIRE( name == "readme" );
    REQUIRE( extension == "" );

    splitPathFileExtension("/readme.txt", '/', path, name, extension );
    REQUIRE( path == "/" );
    REQUIRE( name == "readme" );
    REQUIRE( extension == "txt" );

    splitPathFileExtension("C:\\Windows\\notepad.exe", '\\', path, name, extension );
    REQUIRE( path == "C:\\Windows\\" );
    REQUIRE( name == "notepad" );
    REQUIRE( extension == "exe" );

    splitPathFileExtension("D:\\Folder\\file", '\\', path, name, extension );
    REQUIRE( path == "D:\\Folder\\" );
    REQUIRE( name == "file" );
    REQUIRE( extension == "" );

    splitPathFileExtension("E:\\binary.dat", '\\', path, name, extension );
    REQUIRE( path == "E:\\" );
    REQUIRE( name == "binary" );
    REQUIRE( extension == "dat" );
  }
}
