/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2022  Dirk Stolle

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
#include "../../../../../mw/base/records/PathGridRecord.hpp"

TEST_CASE("MWTP::GridPointData")
{
  using namespace MWTP;

  SECTION("constructor")
  {
    GridPointData point;

    REQUIRE( point.X == 0 );
    REQUIRE( point.Y == 0 );
    REQUIRE( point.Z == 0 );
    REQUIRE( point.Flags == 0 );
    REQUIRE( point.ConnectionCount == 0 );
    REQUIRE( point.Unknown == 0 );
  }

  SECTION("equality operator")
  {
    GridPointData a;
    GridPointData b;

    SECTION("equal")
    {
      a.X = 1;
      a.Y = 2;
      a.Z = 512;
      a.Unknown = 1;

      b.X = 1;
      b.Y = 2;
      b.Z = 512;
      b.Unknown = 1;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("X mismatch")
      {
        a.X = 1;
        b.X = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Y mismatch")
      {
        a.Y = 1;
        b.Y = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Z mismatch")
      {
        a.Z = 128;
        b.Z = 256;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Flags mismatch")
      {
        a.Flags = 0;
        b.Flags = 1;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ConnectionCount mismatch")
      {
        a.ConnectionCount = 1;
        b.ConnectionCount = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Unknown mismatch")
      {
        a.Unknown = 1;
        b.Unknown = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
