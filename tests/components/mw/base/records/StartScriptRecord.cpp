/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
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

#include <catch.hpp>
#include <sstream>
#include <string_view>
#include "../../../../../mw/base/records/StartScriptRecord.hpp"

TEST_CASE("MWTP::StartScriptRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    StartScriptRecord record;

    REQUIRE( record.Data.empty() );
    REQUIRE( record.recordID.empty() );
  }

  SECTION("equals")
  {
    StartScriptRecord a;
    StartScriptRecord b;

    SECTION("equal")
    {
      a.Data = "123456";
      a.recordID = "foo";

      b.Data = "123456";
      b.recordID = "foo";

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("Data mismatch")
      {
        a.Data = "12345678";
        b.Data = "98765432";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("recordID mismatch")
      {
        a.recordID = "foo";
        b.recordID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;

    SECTION("default: load record")
    {
      const auto data = "SSCR\x2D\0\0\0\0\0\0\0\0\0\0\0DATA\x11\0\0\0\x36\x37\x33\x32\x32\x38\x32\x36\x38\x32\x35\x31\x31\x32\x31\x33\x37NAME\x0C\0\0\0TribunalMain"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SSCR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      StartScriptRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.Data == "67322826825112137" );
      REQUIRE( record.recordID == "TribunalMain" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "SSCR\x48\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SSCR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      StartScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no DATA")
    {
      const auto data = "SSCR\x2D\0\0\0\0\0\0\0\0\0\0\0FAIL\x11\0\0\0\x36\x37\x33\x32\x32\x38\x32\x36\x38\x32\x35\x31\x31\x32\x31\x33\x37NAME\x0C\0\0\0TribunalMain"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SSCR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      StartScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DATA > 256")
    {
      const auto data = "SSCR\x2D\0\0\0\0\0\0\0\0\0\0\0DATA\x11\x01\0\0\x36\x37\x33\x32\x32\x38\x32\x36\x38\x32\x35\x31\x31\x32\x31\x33\x37NAME\x0C\0\0\0TribunalMain"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SSCR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      StartScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DATA is beyond stream")
    {
      const auto data = "SSCR\x2D\0\0\0\0\0\0\0\0\0\0\0DATA\x42\0\0\0\x36\x37\x33\x32\x32\x38\x32\x36\x38\x32\x35\x31\x31\x32\x31\x33\x37NAME\x0C\0\0\0TribunalMain"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SSCR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      StartScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "SSCR\x2D\0\0\0\0\0\0\0\0\0\0\0DATA\x11\0\0\0\x36\x37\x33\x32\x32\x38\x32\x36\x38\x32\x35\x31\x31\x32\x31\x33\x37GONE\x0C\0\0\0TribunalMain"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SSCR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      StartScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "SSCR\x2D\0\0\0\0\0\0\0\0\0\0\0DATA\x11\0\0\0\x36\x37\x33\x32\x32\x38\x32\x36\x38\x32\x35\x31\x31\x32\x31\x33\x37NAME\x0C\x01\0\0TribunalMain"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SSCR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      StartScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "SSCR\x2D\0\0\0\0\0\0\0\0\0\0\0DATA\x11\0\0\0\x36\x37\x33\x32\x32\x38\x32\x36\x38\x32\x35\x31\x31\x32\x31\x33\x37NAME\x0F\0\0\0TribunalMain"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SSCR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      StartScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
