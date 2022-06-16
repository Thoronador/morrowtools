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

#include "../../../locate_catch.hpp"
#include "../../../../../sr/base/records/NPCRecord.hpp"

TEST_CASE("NPCRecord::FactionElem")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    NPCRecord::FactionElem elem;

    REQUIRE( elem.formID == 0 );
    REQUIRE( elem.rank == 0 );
  }

  SECTION("equality")
  {
    NPCRecord::FactionElem a;
    NPCRecord::FactionElem b;

    SECTION("equal")
    {
      a.formID = 0x01234567;
      a.rank = 1;

      b.formID = 0x01234567;
      b.rank = 1;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("formID mismatch")
      {
        a.formID = 0x01234567;
        a.rank = 1;

        b.formID = 0x89ABCDEF;
        b.rank = 1;


        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("rank mismatch")
      {
        a.formID = 0x01234567;
        a.rank = 1;

        b.formID = 0x01234567;
        b.rank = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
