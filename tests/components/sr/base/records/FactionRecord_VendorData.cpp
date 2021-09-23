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
#include "../../../../../sr/base/records/FactionRecord.hpp"

TEST_CASE("FactionRecord::VendorData")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    FactionRecord::VendorData data;

    REQUIRE( data.startHour == 0 );
    REQUIRE( data.endHour == 0 );
    REQUIRE( data.radius == 0 );
    REQUIRE( data.flagsVendor == 0 );
    REQUIRE_FALSE( data.isPresent );
  }

  SECTION("equals")
  {
    FactionRecord::VendorData a;
    FactionRecord::VendorData b;

    a.isPresent = true;
    b.isPresent = true;

    SECTION("equal")
    {
      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("startHour mismatch")
      {
        a.startHour = 1;
        b.startHour = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("endHour mismatch")
      {
        a.endHour = 1;
        b.endHour = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("radius mismatch")
      {
        a.radius = 3;
        b.radius = 4;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("flagsVendor mismatch")
      {
        a.flagsVendor = 3;
        b.flagsVendor = 4;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("isPresent mismatch")
      {
        a.isPresent = false;
        b.isPresent = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
