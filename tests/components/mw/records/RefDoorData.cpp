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

#include "../../locate_catch.hpp"
#include "../../../../lib/mw/records/ReferencedObject.hpp"

TEST_CASE("MWTP::RefDoorData")
{
  using namespace MWTP;

  SECTION("constructor")
  {
    RefDoorData data;

    REQUIRE( data.PosX == 0.0f );
    REQUIRE( data.PosY == 0.0f );
    REQUIRE( data.PosZ == 0.0f );
    REQUIRE( data.RotX == 0.0f );
    REQUIRE( data.RotY == 0.0f );
    REQUIRE( data.RotZ == 0.0f );
    REQUIRE( data.ExitName.empty() );
  }

  SECTION("equality operator")
  {
    RefDoorData a;
    RefDoorData b;

    SECTION("equal")
    {
      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("PosX mismatch")
      {
        a.PosX = 1.0f;
        b.PosX = 200.0f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("PosY mismatch")
      {
        a.PosY = 1.0f;
        b.PosY = 200.0f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("PosZ mismatch")
      {
        a.PosZ = 1.0f;
        b.PosZ = 200.0f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("RotX mismatch")
      {
        a.RotX = 1.0f;
        b.RotX = 200.0f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("RotY mismatch")
      {
        a.RotY = 1.0f;
        b.RotY = 200.0f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("RotZ mismatch")
      {
        a.RotZ = 1.0f;
        b.RotZ = 200.0f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ExitName mismatch")
      {
        a.ExitName = "Balmora";
        b.ExitName = "Seyda Neen";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
