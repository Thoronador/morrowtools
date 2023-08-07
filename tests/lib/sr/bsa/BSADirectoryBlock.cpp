/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2021, 2022, 2023  Dirk Stolle

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

#include <sstream>
#include "../../locate_catch.hpp"
#include "../../../../lib/sr/bsa/BSADirectoryBlock.hpp"

TEST_CASE("BSADirectoryBlock")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    BSADirectoryBlock block;

    REQUIRE( block.name.empty() );
    REQUIRE( block.files.empty() );
  }

  SECTION("loadFromStream")
  {
    using namespace std::string_view_literals;

    SECTION("default: load block with two file records")
    {
      const auto data = "\x17interface\\controls\\360\0\x68\x73\x10\x6B\xB0\x44\xDC\x1F\x3A\x09\0\0\xF4\x18\0\0\x64\x61\x07\x67\x59\x72\xF7\x6C\x17\x02\0\0\x2E\x22\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should succeed.
      BSADirectoryBlock block;
      REQUIRE( block.loadFromStream(stream, 2) );
      // Check data.
      REQUIRE( block.name == "interface\\controls\\360" );
      REQUIRE( block.files.size() == 2 );
      REQUIRE( block.files[0].nameHash == 0x1FDC44B06B107368 );
      REQUIRE( block.files[0].fileBlockSize == 2362 );
      REQUIRE( block.files[0].offset == 6388 );
      REQUIRE( block.files[0].fileName.empty() );
      REQUIRE( block.files[1].nameHash == 0x6CF7725967076164 );
      REQUIRE( block.files[1].fileBlockSize == 535 );
      REQUIRE( block.files[1].offset == 8750 );
      REQUIRE( block.files[1].fileName.empty() );
    }

    SECTION("default: load block with one file record")
    {
      const auto data = "\x17interface\\controls\\360\0\x68\x73\x10\x6B\xB0\x44\xDC\x1F\x3A\x09\0\0\xF4\x18\0\0\x64\x61\x07\x67\x59\x72\xF7\x6C\x17\x02\0\0\x2E\x22\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should succeed.
      BSADirectoryBlock block;
      REQUIRE( block.loadFromStream(stream, 1) );
      // Check data.
      REQUIRE( block.name == "interface\\controls\\360" );
      REQUIRE( block.files.size() == 1 );
      REQUIRE( block.files[0].nameHash == 0x1FDC44B06B107368 );
      REQUIRE( block.files[0].fileBlockSize == 2362 );
      REQUIRE( block.files[0].offset == 6388 );
      REQUIRE( block.files[0].fileName.empty() );
    }

    SECTION("corrupt state: bad stream on entry")
    {
      const auto data = "\x17interface\\controls\\360\0\x68\x73\x10\x6B\xB0\x44\xDC\x1F\x3A\x09\0\0\xF4\x18\0\0\x64\x61\x07\x67\x59\x72\xF7\x6C\x17\x02\0\0\x2E\x22\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      char buffer[100];
      stream.read(buffer, 100);
      REQUIRE_FALSE( stream.good() );

      // Reading should fail.
      BSADirectoryBlock block;
      REQUIRE_FALSE( block.loadFromStream(stream, 2) );
    }

    SECTION("corrupt data: stream ends before folder name length can be read")
    {
      std::istringstream stream;
      stream.str(std::string());
      REQUIRE( stream.good() );

      // Reading should fail.
      BSADirectoryBlock block;
      REQUIRE_FALSE( block.loadFromStream(stream, 2) );
    }

    SECTION("corrupt data: stream ends before folder name can be read")
    {
      const auto data = "\x17interface\\con"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSADirectoryBlock block;
      REQUIRE_FALSE( block.loadFromStream(stream, 2) );
    }

    SECTION("corrupt data: stream ends before full block can be read")
    {
      const auto data = "\x17interface\\controls\\360\0\x68\x73\x10"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSADirectoryBlock block;
      REQUIRE_FALSE( block.loadFromStream(stream, 1) );
    }
  }
}
