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

#include "../../locate_catch.hpp"
#include "../../../../lib/sr/records/NPCRecord.hpp"

TEST_CASE("NPCRecord::CSDXstruct")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    NPCRecord::CSDXstruct elem;

    REQUIRE( elem.unknownCSDT == 0 );
    REQUIRE( elem.unknownCSDI == 0 );
    REQUIRE( elem.unknownCSDC == 0 );
  }

  SECTION("equality")
  {
    NPCRecord::CSDXstruct a;
    NPCRecord::CSDXstruct b;

    SECTION("equal")
    {
      a.unknownCSDT = 1;
      a.unknownCSDI = 2;
      a.unknownCSDC = 3;

      b.unknownCSDT = 1;
      b.unknownCSDI = 2;
      b.unknownCSDC = 3;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("unknownCSDT mismatch")
      {
        a.unknownCSDT = 1;
        a.unknownCSDI = 2;
        a.unknownCSDC = 3;

        b.unknownCSDT = 42;
        b.unknownCSDI = 2;
        b.unknownCSDC = 3;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("unknownCSDI mismatch")
      {
        a.unknownCSDT = 1;
        a.unknownCSDI = 2;
        a.unknownCSDC = 3;

        b.unknownCSDT = 1;
        b.unknownCSDI = 42;
        b.unknownCSDC = 3;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("unknownCSDC mismatch")
      {
        a.unknownCSDT = 1;
        a.unknownCSDI = 2;
        a.unknownCSDC = 3;

        b.unknownCSDT = 1;
        b.unknownCSDI = 2;
        b.unknownCSDC = 42;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
