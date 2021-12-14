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
#include "../../../../../sr/base/records/NPCRecord.hpp"

TEST_CASE("NPCRecord::PerkElem")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    NPCRecord::PerkElem elem;

    REQUIRE( elem.formID == 0 );
    REQUIRE( elem.valueTwo == 0 );
  }

  SECTION("equality")
  {
    NPCRecord::PerkElem a;
    NPCRecord::PerkElem b;

    SECTION("equal")
    {
      a.formID = 0x01234567;
      a.valueTwo = 0xDEADBEEF;

      b.formID = 0x01234567;
      b.valueTwo = 0xDEADBEEF;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("formID mismatch")
      {
        a.formID = 0xF00BAA12;
        a.valueTwo = 0xDEADBEEF;

        b.formID = 0x01234567;
        b.valueTwo = 0xDEADBEEF;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("valueTwo mismatch")
      {
        a.formID = 0x01234567;
        a.valueTwo = 0xDEADBEEF;

        b.formID = 0x01234567;
        b.valueTwo = 0xF00BAA12;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
