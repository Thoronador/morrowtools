/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021, 2022  Dirk Stolle

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
#include "../../../../../mw/base/records/AIPackages.hpp"

TEST_CASE("MWTP::NPC_AIWander")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    NPC_AIWander package;

    REQUIRE( package.Distance == 0 );
    REQUIRE( package.Duration == 0 );
    REQUIRE( package.Time == 0 );
    REQUIRE( package.Idle[0] == 0 );
    REQUIRE( package.Idle[1] == 0 );
    REQUIRE( package.Idle[2] == 0 );
    REQUIRE( package.Idle[3] == 0 );
    REQUIRE( package.Idle[4] == 0 );
    REQUIRE( package.Idle[5] == 0 );
    REQUIRE( package.Idle[6] == 0 );
    REQUIRE( package.Idle[7] == 0 );
    REQUIRE( package.Reset == 0 );
  }

  SECTION("clear")
  {
    NPC_AIWander package;

    package.Distance = 5;
    package.Duration = 2;
    package.Time = 4;
    package.Idle[0] = 1;
    package.Idle[1] = 2;
    package.Idle[2] = 3;
    package.Idle[3] = 4;
    package.Idle[4] = 5;
    package.Idle[5] = 6;
    package.Idle[6] = 7;
    package.Idle[7] = 8;
    package.Reset = 1;

    package.clear();

    REQUIRE( package.Distance == 0 );
    REQUIRE( package.Duration == 0 );
    REQUIRE( package.Time == 0 );
    REQUIRE( package.Idle[0] == 0 );
    REQUIRE( package.Idle[1] == 0 );
    REQUIRE( package.Idle[2] == 0 );
    REQUIRE( package.Idle[3] == 0 );
    REQUIRE( package.Idle[4] == 0 );
    REQUIRE( package.Idle[5] == 0 );
    REQUIRE( package.Idle[6] == 0 );
    REQUIRE( package.Idle[7] == 0 );
    REQUIRE( package.Reset == 0 );
  }

  SECTION("equals")
  {
    NPC_AIWander a;
    NPC_AIWander b;

    SECTION("equal")
    {
      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("Distance mismatch")
      {
        a.Distance = 2;
        b.Distance = 3;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Duration mismatch")
      {
        a.Duration = 10;
        b.Duration = 5;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Time mismatch")
      {
        a.Time = 2;
        b.Time = 3;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Idle_0 mismatch")
      {
        a.Idle[0] = 2;
        b.Idle[0] = 3;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Idle_1 mismatch")
      {
        a.Idle[1] = 2;
        b.Idle[1] = 3;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Idle_2 mismatch")
      {
        a.Idle[2] = 2;
        b.Idle[2] = 3;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Idle_3 mismatch")
      {
        a.Idle[3] = 2;
        b.Idle[3] = 3;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Idle_4 mismatch")
      {
        a.Idle[4] = 2;
        b.Idle[4] = 3;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Idle_5 mismatch")
      {
        a.Idle[5] = 2;
        b.Idle[5] = 3;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Idle_6 mismatch")
      {
        a.Idle[6] = 2;
        b.Idle[6] = 3;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Idle_7 mismatch")
      {
        a.Idle[7] = 2;
        b.Idle[7] = 3;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Reset mismatch")
      {
        a.Reset = 1;
        b.Reset = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getPackageType")
  {
    NPC_AIWander package;

    REQUIRE( package.getPackageType() == PackageType::ptWander );
  }

  SECTION("getStreamSize")
  {
    NPC_AIWander package;

    REQUIRE( package.getStreamSize() == 22 );
  }

  SECTION("loadFromStream")
  {
    uint32_t BytesRead = 0;

    SECTION("default: load AI wander package")
    {
      const auto data = "AI_W\x0E\0\0\0\x80\0\x05\0\0\x3C\x14\x0A\x0A\0\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AI_W, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      NPC_AIWander package;
      REQUIRE( package.loadFromStream(stream, BytesRead) );
      // Check data.
      REQUIRE( package.Distance == 128 );
      REQUIRE( package.Duration == 5 );
      REQUIRE( package.Time == 0 );
      REQUIRE( package.Idle[0] == 60 );
      REQUIRE( package.Idle[1] == 20 );
      REQUIRE( package.Idle[2] == 10 );
      REQUIRE( package.Idle[3] == 10 );
      REQUIRE( package.Idle[4] == 0 );
      REQUIRE( package.Idle[5] == 0 );
      REQUIRE( package.Idle[6] == 0 );
      REQUIRE( package.Idle[7] == 0 );
      REQUIRE( package.Reset == 1 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( package.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: length of AI_W is not 14")
    {
      {
        const auto data = "AI_W\x0D\0\0\0\x80\0\x05\0\0\x3C\x14\x0A\x0A\0\0\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip AI_W, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPC_AIWander package;
        REQUIRE_FALSE( package.loadFromStream(stream, BytesRead) );
      }

      {
        const auto data = "AI_W\x0F\0\0\0\x80\0\x05\0\0\x3C\x14\x0A\x0A\0\0\0\0\x01\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip AI_W, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPC_AIWander package;
        REQUIRE_FALSE( package.loadFromStream(stream, BytesRead) );
      }
    }

    SECTION("corrupt data: stream ends before AI_W can be read")
    {
      const auto data = "AI_W\x0E\0\0\0\x80\0\x05"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AI_W, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPC_AIWander package;
      REQUIRE_FALSE( package.loadFromStream(stream, BytesRead) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("default: save package")
    {
      NPC_AIWander package;
      package.Distance = 5;
      package.Duration = 2;
      package.Time = 4;
      package.Idle[0] = 1;
      package.Idle[1] = 2;
      package.Idle[2] = 3;
      package.Idle[3] = 4;
      package.Idle[4] = 5;
      package.Idle[5] = 6;
      package.Idle[6] = 7;
      package.Idle[7] = 8;
      package.Reset = 42;

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( package.saveToStream(stream) );
      // Check written data.
      const auto data = "AI_W\x0E\0\0\0\x05\0\x02\0\x04\x01\x02\x03\x04\x05\x06\x07\x08\x2A"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
