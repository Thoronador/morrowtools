/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2021  Dirk Stolle

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
#include <filesystem>
#include "../../../../../sr/base/bsa/BSA.hpp"

void writeBsaHeaderForTest(const std::string& fileName)
{
  using namespace std::string_view_literals;
  const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x07\0\0\0\xC5\0\0\0\x88\0\0\0\x81\x0B\0\0\0\0\0\0"sv;

  // write BSA header to file
  std::ofstream file(fileName);
  file.write(data.data(), data.size());
  file.close();
}


TEST_CASE("BSA")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    BSA bsa;

    REQUIRE_FALSE( bsa.hasAllStructureData() );
    REQUIRE( bsa.getFolders().empty() );
    REQUIRE( bsa.getFolderBlocks().empty() );
  }

  SECTION("open")
  {
    using namespace std::string_view_literals;

    SECTION("file does not exist")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.open("/does-not/exist.bsa") );
    }

    SECTION("load header-only BSA")
    {
      const auto fileName = "test_sr_bsa_open_header-only.bsa";
      writeBsaHeaderForTest(fileName);

      BSA bsa;
      REQUIRE( bsa.open(fileName) );
      bsa.close();

      REQUIRE( std::filesystem::remove(fileName) );
    }

    SECTION("try to re-open header-only BSA")
    {
      const auto fileName = "test_sr_bsa_open_-reopen-header-only.bsa";
      writeBsaHeaderForTest(fileName);

      BSA bsa;
      REQUIRE( bsa.open(fileName) );
      // Re-open attempt shall fail.
      REQUIRE_FALSE( bsa.open(fileName) );
      bsa.close();

      REQUIRE( std::filesystem::remove(fileName) );
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
