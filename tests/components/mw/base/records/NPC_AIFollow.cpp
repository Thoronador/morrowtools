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

#include "../../../locate_catch.hpp"
#include <limits>
#include <sstream>
#include <string_view>
#include "../../../../../mw/base/records/AIPackages.hpp"

TEST_CASE("MWTP::NPC_AIFollow")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    NPC_AIFollow package;

    REQUIRE( package.X == 0.0f );
    REQUIRE( package.Y == 0.0f );
    REQUIRE( package.Z == 0.0f );
    REQUIRE( package.Duration == 0 );
    REQUIRE( package.TargetID.empty() );
    REQUIRE( package.Reset == 0 );
    REQUIRE( package.CellName.empty() );
  }

  SECTION("clear")
  {
    NPC_AIFollow package;

    package.X = 2.0f;
    package.Y = 3.0f;
    package.Z = 1.0f;
    package.Duration = 123;
    package.TargetID = "foo";
    package.Reset = 1;
    package.CellName = "Some Other Place";

    package.clear();

    REQUIRE( package.X == 0.0f );
    REQUIRE( package.Y == 0.0f );
    REQUIRE( package.Z == 0.0f );
    REQUIRE( package.Duration == 0 );
    REQUIRE( package.TargetID.empty() );
    REQUIRE( package.Reset == 0 );
    REQUIRE( package.CellName.empty() );
  }

  SECTION("equals")
  {
    NPC_AIFollow a;
    NPC_AIFollow b;

    SECTION("equal")
    {
      a.TargetID = "foo";
      a.Reset = 1;

      b.TargetID = "foo";
      b.Reset = 1;

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );

      SECTION("NaNs")
      {
        a.X = std::numeric_limits<float>::quiet_NaN();
        b.X = std::numeric_limits<float>::quiet_NaN();

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );

        a.Y = std::numeric_limits<float>::quiet_NaN();
        b.Y = std::numeric_limits<float>::quiet_NaN();

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );

        a.Z = std::numeric_limits<float>::quiet_NaN();
        b.Z = std::numeric_limits<float>::quiet_NaN();

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );
      }
    }

    SECTION("unequal")
    {
      SECTION("X mismatch")
      {
        a.X = 1.0f;
        b.X = 3.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Y mismatch")
      {
        a.Y = 1.0f;
        b.Y = 3.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Z mismatch")
      {
        a.Z = 1.0f;
        b.Z = 3.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Duration mismatch")
      {
        a.Duration = 1;
        b.Duration = 42;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("TargetID mismatch")
      {
        a.TargetID = "foo";
        b.TargetID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Reset mismatch")
      {
        a.Reset = 1;
        b.Reset = 42;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CellName mismatch")
      {
        a.CellName = "foo";
        b.CellName = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getPackageType")
  {
    NPC_AIFollow package;

    REQUIRE( package.getPackageType() == PackageType::ptFollow );
  }

  SECTION("getStreamSize")
  {
    NPC_AIFollow package;

    SECTION("without cell name")
    {
      REQUIRE( package.getStreamSize() == 56 );
    }

    SECTION("with cell name")
    {
      package.CellName = "foo";
      REQUIRE( package.getStreamSize() == 68 );

      package.CellName = "foobar";
      REQUIRE( package.getStreamSize() == 71 );

      package.CellName = "One Two Three";
      REQUIRE( package.getStreamSize() == 78 );
    }
  }

  SECTION("loadFromStream")
  {
    char Buffer[33];
    uint32_t BytesRead = 0;

    SECTION("default: load AI Follow package")
    {
      const auto data = "AI_F\x30\0\0\0\0\0\x80\x3F\0\0\0\x40\0\0\x40\x40\x01\x04goose town\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AI_F, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      NPC_AIFollow package;
      REQUIRE( package.loadFromStream(stream, Buffer, BytesRead) );
      // Check data.
      REQUIRE( package.X == 1.0f );
      REQUIRE( package.Y == 2.0f );
      REQUIRE( package.Z == 3.0f );
      REQUIRE( package.Duration == 1025 );
      REQUIRE( package.TargetID == "goose town" );
      REQUIRE( package.Reset == 1 );
      REQUIRE( package.CellName.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( package.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: length of AI_F is not 48")
    {
      {
        const auto data = "AI_F\x2F\0\0\0\0\0\x80\x3F\0\0\0\x40\0\0\x40\x40\x01\x04goose town\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip AI_F, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPC_AIFollow package;
        REQUIRE_FALSE( package.loadFromStream(stream, Buffer, BytesRead) );
      }

      {
        const auto data = "AI_F\x31\0\0\0\0\0\x80\x3F\0\0\0\x40\0\0\x40\x40\x01\x04goose town\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip AI_F, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPC_AIFollow package;
        REQUIRE_FALSE( package.loadFromStream(stream, Buffer, BytesRead) );
      }
    }

    SECTION("corrupt data: stream ends before AI_F can be read")
    {
      const auto data = "AI_F\x30\0\0\0\0\0\x80\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AI_F, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPC_AIFollow package;
      REQUIRE_FALSE( package.loadFromStream(stream, Buffer, BytesRead) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("default: save package")
    {
      NPC_AIFollow package;
      package.X = 1.0f;
      package.Y = 2.0f;
      package.Z = 3.0f;
      package.Duration = 1025;
      package.TargetID = "goo";
      package.Reset = 42;
      package.CellName = "bar";

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( package.saveToStream(stream) );
      // Check written data.
      const auto data = "AI_F\x30\0\0\0\0\0\x80\x3F\0\0\0\x40\0\0\x40\x40\x01\x04goo\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x2A\0CNDT\x04\0\0\0bar\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("default: save package without cell name")
    {
      NPC_AIFollow package;
      package.X = 1.0f;
      package.Y = 2.0f;
      package.Z = 3.0f;
      package.Duration = 1025;
      package.TargetID = "goo";
      package.Reset = 42;
      package.CellName.clear();

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( package.saveToStream(stream) );
      // Check written data.
      const auto data = "AI_F\x30\0\0\0\0\0\x80\x3F\0\0\0\x40\0\0\x40\x40\x01\x04goo\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x2A\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("default: save package with very long target ID")
    {
      NPC_AIFollow package;
      package.X = 1.0f;
      package.Y = 2.0f;
      package.Z = 3.0f;
      package.Duration = 1025;
      package.TargetID = "goobarbazquux4567890abcdefghij123456789";
      package.Reset = 42;
      package.CellName.clear();

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( package.saveToStream(stream) );
      // Check written data.
      const auto data = "AI_F\x30\0\0\0\0\0\x80\x3F\0\0\0\x40\0\0\x40\x40\x01\x04goobarbazquux4567890abcdefghij1\0\x2A\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
