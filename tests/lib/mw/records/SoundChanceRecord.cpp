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
#include "../../../../lib/mw/records/RegionRecord.hpp"

TEST_CASE("MWTP::SoundChanceRecord")
{
  using namespace MWTP;

  SECTION("constructor")
  {
    SoundChanceRecord sc;

    REQUIRE( sc.Sound.empty() );
    REQUIRE( sc.Chance == 0 );
  }

  SECTION("equality operator")
  {
    SoundChanceRecord a;
    SoundChanceRecord b;

    SECTION("equal")
    {
      a.Sound = "foo";
      a.Chance = 50;

      b.Sound = "foo";
      b.Chance = 50;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("Sound mismatch")
      {
        a.Sound = "foo";
        b.Sound = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Chance mismatch")
      {
        a.Chance = 15;
        b.Chance = 25;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
