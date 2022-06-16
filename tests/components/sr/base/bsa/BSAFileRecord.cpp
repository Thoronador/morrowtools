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
#include "../../../../../sr/base/bsa/BSAFileRecord.hpp"

TEST_CASE("BSAFileRecord")
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

  SECTION("loadFromStream")
  {
    using namespace std::string_view_literals;

    SECTION("default: load record")
    {
      const auto data = "\x68\x73\x10\x6B\xB0\x44\xDC\x1F\x3A\x09\0\0\xF4\x18\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should succeed.
      BSAFileRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      REQUIRE( record.nameHash == 0x1FDC44B06B107368 );
      REQUIRE( record.fileSize == 2362 );
      REQUIRE( record.offset == 6388 );
      REQUIRE( record.fileName.empty() );
    }

    SECTION("corrupt state: bad stream on entry")
    {
      const auto data = "\x68\x73\x10\x6B\xB0\x44\xDC\x1F\x3A\x09\0\0\xF4\x18\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      char buffer[100];
      stream.read(buffer, 100);
      REQUIRE_FALSE( stream.good() );

      // Reading should fail.
      BSAFileRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before hash can be read")
    {
      const auto data = "\x68\x73\x10\x6B\xB0\x44"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSAFileRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before file size can be read")
    {
      const auto data = "\x68\x73\x10\x6B\xB0\x44\xDC\x1F\x3A\x09"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSAFileRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before offset can be read")
    {
      const auto data = "\x68\x73\x10\x6B\xB0\x44\xDC\x1F\x3A\x09\0\0\xF4\x18"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSAFileRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }

  SECTION("isCompressionToggled")
  {
    BSAFileRecord record;

    record.fileSize = 0;
    REQUIRE_FALSE( record.isCompressionToggled() );

    record.fileSize = 1073741824; // == 1 << 30
    REQUIRE( record.isCompressionToggled() );

    record.fileSize = 32000;
    REQUIRE_FALSE( record.isCompressionToggled() );

    record.fileSize = 1073741824 + 32000;
    REQUIRE( record.isCompressionToggled() );
  }

  SECTION("getRealFileSize")
  {
    BSAFileRecord record;

    record.fileSize = 0;
    REQUIRE( record.getRealFileSize() == 0 );

    record.fileSize = 1073741824; // == 1 << 30
    REQUIRE( record.getRealFileSize() == 0 );

    record.fileSize = 32015;
    REQUIRE( record.getRealFileSize() == 32015 );

    record.fileSize = 1073741824 + 32031;  // == 1 << 30 + 32031
    REQUIRE( record.getRealFileSize() == 32031 );
  }
}
