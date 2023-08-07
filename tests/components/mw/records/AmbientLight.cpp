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
#include "../../../../lib/mw/records/CellRecord.hpp"

TEST_CASE("MWTP::AmbientLight")
{
  using namespace MWTP;

  SECTION("constructor")
  {
    AmbientLight ambi;

    REQUIRE_FALSE( ambi.isPresent );
    REQUIRE( ambi.AmbientColour == 0 );
    REQUIRE( ambi.SunlightColour == 0 );
    REQUIRE( ambi.FogColour == 0 );
    REQUIRE( ambi.FogDensity == 0.0f );
  }

  SECTION("equality operator")
  {
    AmbientLight a;
    AmbientLight b;

    SECTION("equal")
    {
      SECTION("absence")
      {
        a.isPresent = false;
        a.AmbientColour = 0x00123456;
        b.isPresent = false;
        b.AmbientColour = 0x00555555;

        REQUIRE( a == b );
        REQUIRE( b == a );
      }

      SECTION("presence")
      {
        a.isPresent = true;
        a.AmbientColour = 0x00123456;
        a.SunlightColour = 0x00789ABC;
        a.FogColour = 0x00DEF012;
        a.FogDensity = 0.5f;

        b.isPresent = true;
        b.AmbientColour = 0x00123456;
        b.SunlightColour = 0x00789ABC;
        b.FogColour = 0x00DEF012;
        b.FogDensity = 0.5f;

        REQUIRE( a == b );
        REQUIRE( b == a );
      }
    }

    SECTION("unequal")
    {
      a.isPresent = true;
      b.isPresent = true;

      SECTION("presence mismatch")
      {
        a.isPresent = false;
        b.isPresent = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("AmbientColour mismatch")
      {
        a.AmbientColour = 0x00123456;
        b.AmbientColour = 0x00789ABC;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("SunlightColour mismatch")
      {
        a.SunlightColour = 0x00123456;
        b.SunlightColour = 0x00789ABC;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("FogColour mismatch")
      {
        a.FogColour = 0x00123456;
        b.FogColour = 0x00789ABC;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("FogDensity mismatch")
      {
        a.FogDensity = 0.5f;
        b.FogDensity = 0.75f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
