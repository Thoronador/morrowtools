/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021  Thoronador

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
#include "../../../../../mw/base/records/TravelDestination.hpp"

TEST_CASE("MWTP::TravelDestination")
{
  using namespace MWTP;

  SECTION("constructor")
  {
    TravelDestination dest;

    REQUIRE( dest.XPos == 0.0f );
    REQUIRE( dest.YPos == 0.0f );
    REQUIRE( dest.ZPos == 0.0f );
    REQUIRE( dest.XRot == 0.0f );
    REQUIRE( dest.YRot == 0.0f );
    REQUIRE( dest.ZRot == 0.0f );
    REQUIRE( dest.CellName.empty() );
  }

  SECTION("equality operator")
  {
    TravelDestination a;
    TravelDestination b;

    SECTION("equal")
    {
      a.XPos = 1.0f;
      a.CellName = "foo";

      b.XPos = 1.0f;
      b.CellName = "foo";

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("XPos mismatch")
      {
        a.XPos = 1.0f;
        b.XPos = 2.5f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("YPos mismatch")
      {
        a.YPos = 1.0f;
        b.YPos = 2.5f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ZPos mismatch")
      {
        a.ZPos = 1.0f;
        b.ZPos = 2.5f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("XRot mismatch")
      {
        a.XRot = 120.0f;
        b.XRot = 2.75f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("YRot mismatch")
      {
        a.YRot = 120.0f;
        b.YRot = 2.75f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ZRot mismatch")
      {
        a.ZRot = 120.0f;
        b.ZRot = 2.75f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("CellName mismatch")
      {
        a.CellName = "foo";
        b.CellName = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
