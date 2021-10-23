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
#include "../../../../../sr/base/bsa/BSA.hpp"

TEST_CASE("BSA")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    BSAFileRecord file;

    REQUIRE( file.nameHash == 0 );
    REQUIRE( file.fileSize == 0 );
    REQUIRE( file.offset == 0 );
    REQUIRE( file.fileName.empty() );
  }

  SECTION("open")
  {
    using namespace std::string_view_literals;

    SECTION("file does not exist")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.open("/does-not/exist.bsa") );
    }
  }

  SECTION("grabAllStructureData")
  {
    using namespace std::string_view_literals;

    SECTION("BSA has not been opened")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.grabAllStructureData() );
    }
  }

  SECTION("grabFolderData")
  {
    using namespace std::string_view_literals;

    SECTION("BSA has not been opened")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.grabFolderData() );
    }
  }

  SECTION("grabFolderBlocks")
  {
    using namespace std::string_view_literals;

    SECTION("BSA has not been opened")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.grabFolderBlocks() );
    }
  }

  SECTION("grabFileNames")
  {
    using namespace std::string_view_literals;

    SECTION("BSA has not been opened")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.grabFileNames() );
    }
  }

  SECTION("getDirectories")
  {
    using namespace std::string_view_literals;

    SECTION("BSA has not been opened / no structure data")
    {
      BSA bsa;
      REQUIRE( bsa.getDirectories().empty() );
    }
  }

  SECTION("getFilesOfDirectory")
  {
    using namespace std::string_view_literals;

    SECTION("BSA has not been opened / no structure data")
    {
      BSA bsa;
      REQUIRE( bsa.getFilesOfDirectory(1, false).empty() );
    }
  }

  SECTION("getIndexOfFolder")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      REQUIRE( bsa.getIndexOfFolder("foo/bar") == BSA::cIndexNotFound );
    }
  }

  SECTION("getIndexOfFile")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      REQUIRE( bsa.getIndexOfFile(1, "foo.bar") == BSA::cIndexNotFound );
    }
  }

  SECTION("getIndexPairForFile")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      uint32_t folderIndex = 0;
      uint32_t fileIndex = 0;
      REQUIRE_FALSE( bsa.getIndexPairForFile("baz\\foo.bar", folderIndex, fileIndex) );
      REQUIRE( folderIndex == BSA::cIndexNotFound );
      REQUIRE( fileIndex == BSA::cIndexNotFound );
    }
  }

  SECTION("hasFile")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.hasFile("baz\\foo.bar") );
    }
  }

  SECTION("isFileCompressed")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      REQUIRE_THROWS( bsa.isFileCompressed(1, 1) );
    }
  }

  SECTION("extractFile (with index parameters)")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.extractFile(1, 1, "/tmp/foo.out") );
    }
  }

  SECTION("extractFile (with file name parameter)")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.extractFile("foo\\bar.baz", "/tmp/foo.out") );
    }
  }

  SECTION("extractFolder (with index parameter)")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      uint32_t counter = 42;
      REQUIRE_FALSE( bsa.extractFolder(1, "/tmp/directory-out", counter) );
      REQUIRE( counter == 0 );
    }
  }

  SECTION("extractFolder (with folder name parameter)")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      uint32_t counter = 42;
      REQUIRE_FALSE( bsa.extractFolder("foo\\bar", "/tmp/directory-out", counter) );
      REQUIRE( counter == 0 );
    }
  }

  SECTION("extractAll")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      uint32_t counter = 42;
      REQUIRE_FALSE( bsa.extractAll("/tmp/directory-out", counter) );
      REQUIRE( counter == 0 );
    }
  }
}
