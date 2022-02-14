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
#include <limits>
#include <sstream>
#include <string_view>
#include "../../../../../mw/base/records/AIPackages.hpp"

TEST_CASE("MWTP::NPC_AITravel")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    NPC_AITravel package;

    REQUIRE( package.X == 0.0f );
    REQUIRE( package.Y == 0.0f );
    REQUIRE( package.Z == 0.0f );
    REQUIRE( package.Reset == 0 );
  }

  SECTION("clear")
  {
    NPC_AITravel package;

    package.X = 1.0f;
    package.Y = 1.5f;
    package.Z = 9001.0f;
    package.Reset = 1;

    package.clear();

    REQUIRE( package.X == 0.0f );
    REQUIRE( package.Y == 0.0f );
    REQUIRE( package.Z == 0.0f );
    REQUIRE( package.Reset == 0 );
  }

  SECTION("equals")
  {
    NPC_AITravel a;
    NPC_AITravel b;

    SECTION("equal")
    {
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
    NPC_AITravel package;

    REQUIRE( package.getPackageType() == PackageType::ptTravel );
  }

  SECTION("getStreamSize")
  {
    NPC_AITravel package;

    REQUIRE( package.getStreamSize() == 24 );
  }

  SECTION("saveToStream")
  {
    SECTION("default: save package")
    {
      NPC_AITravel package;
      package.X = 1.0f;
      package.Y = 2.0f;
      package.Z = 3.0f;
      package.Reset = 42;

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( package.saveToStream(stream) );
      // Check written data.
      const auto data = "AI_T\x10\0\0\0\0\0\x80\x3F\0\0\0\x40\0\0\x40\x40\x2A\0\0\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
