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

#include <sstream>
#include "../../locate_catch.hpp"
#include "../../../../lib/sr/bsa/BSADirectoryRecord.hpp"

TEST_CASE("BSADirectoryRecord")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    BSADirectoryRecord directory;

    REQUIRE( directory.nameHash == 0 );
    REQUIRE( directory.count == 0 );
    REQUIRE( directory.offset == 0 );
  }

  SECTION("loadFromStream")
  {
    using namespace std::string_view_literals;

    SECTION("default: load record version 104")
    {
      const auto data = "\x30\x36\x16\x69\xF9\xF3\xC5\x3D\x03\0\0\0\x15\x0C\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should succeed.
      BSADirectoryRecord directory;
      REQUIRE( directory.loadFromStream(stream, 104) );
      // Check data.
      REQUIRE( directory.nameHash == 0x3DC5F3F969163630 );
      REQUIRE( directory.count == 3 );
      REQUIRE( directory.offset == 3093 );
    }

    SECTION("default: load record version 105")
    {
      const auto data = "\x30\x36\x16\x69\xF9\xF3\xC5\x3D\x03\0\0\0\0\0\0\0\x40\x19\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should succeed.
      BSADirectoryRecord directory;
      REQUIRE( directory.loadFromStream(stream, 105) );
      // Check data.
      REQUIRE( directory.nameHash == 0x3DC5F3F969163630 );
      REQUIRE( directory.count == 3 );
      REQUIRE( directory.offset == 6464 );
    }

    SECTION("corrupt state: bad stream on entry")
    {
      const auto data = "\x30\x36\x16\x69\xF9\xF3\xC5\x3D\x03\0\0\0\x15\x0C\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      char buffer[100];
      stream.read(buffer, 100);
      REQUIRE_FALSE( stream.good() );

      // Reading should fail.
      BSADirectoryRecord directory;
      REQUIRE_FALSE( directory.loadFromStream(stream, 104) );
    }

    SECTION("corrupt data: stream ends before hash can be read")
    {
      const auto data = "\x30\x36\x16\x69"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSADirectoryRecord directory;
      REQUIRE_FALSE( directory.loadFromStream(stream, 104) );
    }

    SECTION("corrupt data: stream ends before count can be read, version 104")
    {
      const auto data = "\x30\x36\x16\x69\xF9\xF3\xC5\x3D\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSADirectoryRecord directory;
      REQUIRE_FALSE( directory.loadFromStream(stream, 104) );
    }

    SECTION("corrupt data: stream ends before count can be read, version 105")
    {
      const auto data = "\x30\x36\x16\x69\xF9\xF3\xC5\x3D\x03\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSADirectoryRecord directory;
      REQUIRE_FALSE( directory.loadFromStream(stream, 105) );
    }

    SECTION("corrupt data: stream ends before offset can be read, version 104")
    {
      const auto data = "\x30\x36\x16\x69\xF9\xF3\xC5\x3D\x03\0\0\0\x15"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSADirectoryRecord directory;
      REQUIRE_FALSE( directory.loadFromStream(stream, 104) );
    }

    SECTION("corrupt data: stream ends before offset can be read, version 105")
    {
      const auto data = "\x30\x36\x16\x69\xF9\xF3\xC5\x3D\x03\0\0\0\0\0\0\0\x40\x19\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSADirectoryRecord directory;
      REQUIRE_FALSE( directory.loadFromStream(stream, 105) );
    }
  }
}
