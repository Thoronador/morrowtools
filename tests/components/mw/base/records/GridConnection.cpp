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

TEST_CASE("MWTP::GridConnection")
{
  using namespace MWTP;

  SECTION("constructor")
  {
    GridConnection conn;

    REQUIRE( conn.Start == 0 );
    REQUIRE( conn.End == 0 );
  }

  SECTION("equality operator")
  {
    GridConnection a;
    GridConnection b;

    SECTION("equal")
    {
      a.Start = 1;
      a.End = 2;

      b.Start = 1;
      b.End = 2;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("Start index mismatch")
      {
        a.Start = 1;
        b.Start = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("End index mismatch")
      {
        a.End = 1;
        b.End = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
