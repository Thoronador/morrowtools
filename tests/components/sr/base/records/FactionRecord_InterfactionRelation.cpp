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
#include "../../../../../sr/base/records/FactionRecord.hpp"

TEST_CASE("FactionRecord::InterfactionRelation")
{
  using namespace SRTP;

  SECTION("equals")
  {
    FactionRecord::InterfactionRelation a;
    FactionRecord::InterfactionRelation b;

    a.factionFormID = 0;
    a.mod = 0;
    a.groupCombatReaction = 0;

    b.factionFormID = 0;
    b.mod = 0;
    b.groupCombatReaction = 0;

    SECTION("equal")
    {
      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("factionFormID mismatch")
      {
        a.factionFormID = 0x01234567;
        b.factionFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("mod mismatch")
      {
        a.mod = 0x01234567;
        b.mod = 0x0FEDCBA9;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("groupCombatReaction mismatch")
      {
        a.groupCombatReaction = 0x01234567;
        b.groupCombatReaction = 0x0FED;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
