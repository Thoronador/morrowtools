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
#include <sstream>
#include <string_view>
#include "../../../../../sr/base/records/LocalizedString.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("LocalizedString")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    LocalizedString localized;

    REQUIRE_FALSE( localized.isPresent() );
    REQUIRE( localized.getType() == LocalizedString::Type::None );
    REQUIRE_THROWS_AS( localized.getIndex(), WrongStringType );
    REQUIRE_THROWS_AS( localized.getString(), WrongStringType );
    REQUIRE( localized.getWriteSize() == 0 );
  }

  SECTION("loadFromStream and saveToStream")
  {
    using namespace std::string_view_literals;

    uint32_t dummy = 0;
    StringTable table;
    uint32_t bytesRead = 0;
    char buffer[512];

    SECTION("localized indexed string with header")
    {
      const std::string_view data = "FULL\x04\0\x7c\x26\x01\0"sv;
      std::istringstream stream_in;
      stream_in.str(std::string(data));

      table.addString(0x0001267C, "foo bar");

      // Reading should succeed.
      LocalizedString localized;
      REQUIRE( localized.loadFromStream(stream_in, cFULL, true, bytesRead, true, table, nullptr) );
      // Check data.
      REQUIRE( localized.getType() == LocalizedString::Type::Index );
      REQUIRE( localized.getIndex() == 0x0001267C );
      REQUIRE( localized.getString() == "foo bar" );
      REQUIRE( localized.isPresent() );

      // Check write size.
      REQUIRE( localized.getWriteSize() == 10 );

      // Write data to stream.
      std::ostringstream stream_out;
      REQUIRE( stream_out.good() );
      REQUIRE( localized.saveToStream(stream_out, cFULL) );
      REQUIRE( stream_out.str() == data );

      // Reset.
      localized.reset();
      REQUIRE_FALSE( localized.isPresent() );
    }

    SECTION("localized indexed string with header but index zero")
    {
      const std::string_view data = "TNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream_in;
      stream_in.str(std::string(data));

      // Reading should succeed.
      LocalizedString localized;
      REQUIRE( localized.loadFromStream(stream_in, cTNAM, true, bytesRead, true, table, nullptr) );
      // Check data: index zero means empty string.
      REQUIRE( localized.getType() == LocalizedString::Type::Index );
      REQUIRE( localized.getIndex() == 0 );
      REQUIRE( localized.getString().empty() );
      REQUIRE( localized.isPresent() );

      // Check write size.
      REQUIRE( localized.getWriteSize() == 10 );

      // Write data to stream.
      std::ostringstream stream_out;
      REQUIRE( stream_out.good() );
      REQUIRE( localized.saveToStream(stream_out, cTNAM) );
      REQUIRE( stream_out.str() == data );

      // Reset.
      localized.reset();
      REQUIRE_FALSE( localized.isPresent() );
    }

    SECTION("localized indexed string without header")
    {
      const std::string_view data = "FULL\x04\0\x7c\x34\x02\x01"sv;
      std::istringstream stream_in;
      stream_in.str(std::string(data));

      table.addString(0x0102347C, "foo bar baz");

      // read FULL, because header is handled before loadFromStream.
      stream_in.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream_in.good() );

      // Reading should succeed.
      LocalizedString localized;
      REQUIRE( localized.loadFromStream(stream_in, cFULL, false, bytesRead, true, table, nullptr) );
      // Check data.
      REQUIRE( localized.getType() == LocalizedString::Type::Index );
      REQUIRE( localized.getIndex() == 0x0102347C );
      REQUIRE( localized.getString() == "foo bar baz" );
      REQUIRE( localized.isPresent() );

      // Check write size.
      REQUIRE( localized.getWriteSize() == 10 );

      // Write data to stream.
      std::ostringstream stream_out;
      REQUIRE( stream_out.good() );
      REQUIRE( localized.saveToStream(stream_out, cFULL) );
      REQUIRE( stream_out.str() == data );

      // Reset.
      localized.reset();
      REQUIRE_FALSE( localized.isPresent() );
    }

    SECTION("plain string with header")
    {
      const std::string_view data = "FULL\x0A\0foo bar 3\0"sv;
      std::istringstream stream_in;
      stream_in.str(std::string(data));

      // Reading should succeed.
      LocalizedString localized;
      REQUIRE( localized.loadFromStream(stream_in, cFULL, true, bytesRead, false, table, buffer) );
      // Check data.
      REQUIRE( localized.getType() == LocalizedString::Type::String );
      REQUIRE( localized.getString() == "foo bar 3" );
      REQUIRE( localized.isPresent() );
      REQUIRE_THROWS_AS( localized.getIndex(), WrongStringType );

      // Check write size.
      REQUIRE( localized.getWriteSize() == 16 );

      // Write data to stream.
      std::ostringstream stream_out;
      REQUIRE( stream_out.good() );
      REQUIRE( localized.saveToStream(stream_out, cFULL) );
      REQUIRE( stream_out.str() == data );

      // Reset.
      localized.reset();
      REQUIRE_FALSE( localized.isPresent() );
    }

    SECTION("plain string without header")
    {
      const std::string_view data = "FULL\x0B\0foo bar 42\0"sv;
      std::istringstream stream_in;
      stream_in.str(std::string(data));

      // read FULL, because header is handled before loadFromStream.
      stream_in.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream_in.good() );

      // Reading should succeed.
      LocalizedString localized;
      REQUIRE( localized.loadFromStream(stream_in, cFULL, false, bytesRead, false, table, buffer) );
      // Check data.
      REQUIRE( localized.getType() == LocalizedString::Type::String );
      REQUIRE( localized.getString() == "foo bar 42" );
      REQUIRE_THROWS_AS( localized.getIndex(), WrongStringType );
      REQUIRE( localized.isPresent() );

      // Check write size.
      REQUIRE( localized.getWriteSize() == 17 );

      // Write data to stream.
      std::ostringstream stream_out;
      REQUIRE( stream_out.good() );
      REQUIRE( localized.saveToStream(stream_out, cFULL) );
      REQUIRE( stream_out.str() == data );

      // Reset.
      localized.reset();
      REQUIRE_FALSE( localized.isPresent() );
    }

    SECTION("saving uninitialized string (type none) to stream")
    {
      LocalizedString localized;
      REQUIRE( localized.getType() == LocalizedString::Type::None );
      REQUIRE_FALSE( localized.isPresent() );

      // Check write size.
      REQUIRE( localized.getWriteSize() == 0 );

      // Write data to stream.
      std::ostringstream stream_out;
      REQUIRE( stream_out.good() );
      REQUIRE( localized.saveToStream(stream_out, cTNAM) );
      REQUIRE( stream_out.str().empty() );
    }

    SECTION("corrupt data: localized indexed string with mismatched header")
    {
      const std::string_view data = "FULL\x04\0\x7c\x26\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      table.addString(0x0001267C, "foo bar");

      // Reading should fail.
      LocalizedString localized;
      REQUIRE_FALSE( localized.loadFromStream(stream, cXNAM, true, bytesRead, true, table, nullptr) );
      REQUIRE_FALSE( localized.isPresent() );
    }

    SECTION("corrupt data: localized indexed string with wrong size")
    {
      const std::string_view data = "FULL\x05\0\x7c\x26\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      table.addString(0x0001267C, "foo bar");

      // Reading should fail.
      LocalizedString localized;
      REQUIRE_FALSE( localized.loadFromStream(stream, cFULL, true, bytesRead, true, table, nullptr) );
      REQUIRE_FALSE( localized.isPresent() );
    }

    SECTION("corrupt data: localized indexed string with header and short stream")
    {
      const std::string_view data = "FULL\x04\0\x7c\x26"sv;
      std::istringstream stream_in;
      stream_in.str(std::string(data));

      table.addString(0x0000267C, "foo bar");

      // Reading should succeed.
      LocalizedString localized;
      REQUIRE_FALSE( localized.loadFromStream(stream_in, cFULL, true, bytesRead, true, table, nullptr) );
    }

    SECTION("corrupt table: localized indexed string with header has no matching index in table")
    {
      const std::string_view data = "FULL\x04\0\x7c\x26\x01\0"sv;
      std::istringstream stream_in;
      stream_in.str(std::string(data));

      // Reading should fail.
      LocalizedString localized;
      REQUIRE_FALSE( localized.loadFromStream(stream_in, cFULL, true, bytesRead, true, table, nullptr) );
    }

    SECTION("corrupt data: plain string with header and length > 512")
    {
      const std::string_view data = "FULL\x02\x02Noo bar 3\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Reading should fail.
      LocalizedString localized;
      REQUIRE_FALSE( localized.loadFromStream(stream, cFULL, true, bytesRead, false, table, buffer) );
      REQUIRE_FALSE( localized.isPresent() );
    }

    SECTION("corrupt data: plain string without header and length > 512")
    {
      const std::string_view data = "FULL\x01\x02Noo bar 42\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FULL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LocalizedString localized;
      REQUIRE_FALSE( localized.loadFromStream(stream, cFULL, false, bytesRead, false, table, buffer) );
      REQUIRE_FALSE( localized.isPresent() );
    }

    SECTION("corrupt data: length of record is beyond stream")
    {
      const std::string_view data = "FULL\x0B\0foo bar 3\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Reading should fail.
      LocalizedString localized;
      REQUIRE_FALSE( localized.loadFromStream(stream, cFULL, true, bytesRead, false, table, buffer) );
      REQUIRE_FALSE( localized.isPresent() );
    }

    SECTION("corrupt data: plain string with header has wrong length")
    {
      const std::string_view data = "FULL\x0B\0foo bar 3\0"sv;
      std::istringstream stream_in;
      stream_in.str(std::string(data));

      // Reading should fail.
      LocalizedString localized;
      REQUIRE_FALSE( localized.loadFromStream(stream_in, cFULL, true, bytesRead, false, table, buffer) );
      REQUIRE_FALSE( localized.isPresent() );
    }
  }

  SECTION("equality")
  {
    SECTION("equal indexed string")
    {
      LocalizedString a(LocalizedString::Type::Index, 12345, "foo bar");
      LocalizedString b(LocalizedString::Type::Index, 12345, "foo bar");

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("equal indexed string: string does not matter")
    {
      LocalizedString a(LocalizedString::Type::Index, 12345, "foo bar");
      LocalizedString b(LocalizedString::Type::Index, 12345, "foo bar baz");

      REQUIRE( a == b );
      REQUIRE( b == a );
      REQUIRE_FALSE( a != b );
      REQUIRE_FALSE( b != a );
    }

    SECTION("unequal indexed string: index")
    {
      LocalizedString a(LocalizedString::Type::Index, 12345, "foo bar");
      LocalizedString b(LocalizedString::Type::Index, 54321, "foo bar");

      REQUIRE_FALSE( a == b );
      REQUIRE_FALSE( b == a );
      REQUIRE( a != b );
      REQUIRE( b != a );
    }

    SECTION("equal direct string")
    {
      LocalizedString a(LocalizedString::Type::String, 0, "foo bar");
      LocalizedString b(LocalizedString::Type::String, 0, "foo bar");

      REQUIRE( a == b );
      REQUIRE( b == a );
      REQUIRE_FALSE( a != b );
      REQUIRE_FALSE( b != a );
    }

    SECTION("equal direct string: index does not matter")
    {
      LocalizedString a(LocalizedString::Type::String, 3210, "foo bar");
      LocalizedString b(LocalizedString::Type::String, 1234, "foo bar");

      REQUIRE( a == b );
      REQUIRE( b == a );
      REQUIRE_FALSE( a != b );
      REQUIRE_FALSE( b != a );
    }

    SECTION("unequal direct string: string")
    {
      LocalizedString a(LocalizedString::Type::String, 0, "foo bar 123");
      LocalizedString b(LocalizedString::Type::String, 0, "foo bar");

      REQUIRE_FALSE( a == b );
      REQUIRE_FALSE( b == a );
      REQUIRE( a != b );
      REQUIRE( b != a );
    }
  }
}
