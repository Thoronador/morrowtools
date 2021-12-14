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

#include <catch.hpp>
#include "../../../../../sr/base/records/CellRecord.hpp"

TEST_CASE("CellRecord::SubrecordXCLC")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    CellRecord::SubrecordXCLC record;

    REQUIRE_FALSE( record.presence );
    REQUIRE( record.locationX == 0 );
    REQUIRE( record.locationY == 0 );
    REQUIRE( record.unknownThird == 0 );
  }

  SECTION("equality")
  {
    CellRecord::SubrecordXCLC a;
    CellRecord::SubrecordXCLC b;

    SECTION("equal")
    {
      SECTION("both present")
      {
        a.presence = true;
        a.locationX = 1;
        a.locationY = 2;
        a.unknownThird = 3;

        b.presence = true;
        b.locationX = 1;
        b.locationY = 2;
        b.unknownThird = 3;

        REQUIRE( a == b );
        REQUIRE( b == a );
      }

      SECTION("both absent")
      {
        a.presence = false;
        a.locationX = 123;
        a.locationY = 2;
        a.unknownThird = 3;

        b.presence = false;
        b.locationX = 1;
        b.locationY = 2;
        b.unknownThird = 3;

        REQUIRE( a == b );
        REQUIRE( b == a );
      }
    }

    SECTION("unequal")
    {
      SECTION("presence mismatch")
      {
        a.presence = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.presence = false;
        b.presence = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("locationX mismatch")
      {
        a.presence = true;
        a.locationX = 1;
        a.locationY = 42;
        a.unknownThird = 0xDEADBEEF;

        b.presence = true;
        b.locationX = 2;
        b.locationY = 42;
        b.unknownThird = 0xDEADBEEF;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("locationY mismatch")
      {
        a.presence = true;
        a.locationX = 42;
        a.locationY = 1;
        a.unknownThird = 0xDEADBEEF;

        b.presence = true;
        b.locationX = 42;
        b.locationY = 2;
        b.unknownThird = 0xDEADBEEF;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("unkownThird mismatch")
      {
        a.presence = true;
        a.locationX = 1;
        a.locationY = 2;
        a.unknownThird = 0xABCDEF01;

        b.presence = true;
        b.locationX = 1;
        b.locationY = 2;
        b.unknownThird = 0x01234567;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
