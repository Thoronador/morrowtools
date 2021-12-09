/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
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

TEST_CASE("NPCRecord::TINXstructure")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    NPCRecord::TINXstructure elem;

    REQUIRE( elem.unknownTINI == 0 );
    REQUIRE( elem.unknownTINC == 0 );
    REQUIRE( elem.unknownTINV == 0 );
    REQUIRE( elem.unknownTIAS == 0 );
  }

  SECTION("equality")
  {
    NPCRecord::TINXstructure a;
    NPCRecord::TINXstructure b;

    SECTION("equal")
    {
      a.unknownTINI = 1;
      a.unknownTINC = 2;
      a.unknownTINV = 3;
      a.unknownTIAS = 4;

      b.unknownTINI = 1;
      b.unknownTINC = 2;
      b.unknownTINV = 3;
      b.unknownTIAS = 4;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("TINI mismatch")
      {
        a.unknownTINI = 1;
        a.unknownTINC = 2;
        a.unknownTINV = 3;
        a.unknownTIAS = 4;

        b.unknownTINI = 42;
        b.unknownTINC = 2;
        b.unknownTINV = 3;
        b.unknownTIAS = 4;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("TINC mismatch")
      {
        a.unknownTINI = 1;
        a.unknownTINC = 2;
        a.unknownTINV = 3;
        a.unknownTIAS = 4;

        b.unknownTINI = 1;
        b.unknownTINC = 42;
        b.unknownTINV = 3;
        b.unknownTIAS = 4;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("TINV mismatch")
      {
        a.unknownTINI = 1;
        a.unknownTINC = 2;
        a.unknownTINV = 3;
        a.unknownTIAS = 4;

        b.unknownTINI = 1;
        b.unknownTINC = 2;
        b.unknownTINV = 42;
        b.unknownTIAS = 4;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("TIAS mismatch")
      {
        a.unknownTINI = 1;
        a.unknownTINC = 2;
        a.unknownTINV = 3;
        a.unknownTIAS = 4;

        b.unknownTINI = 1;
        b.unknownTINC = 2;
        b.unknownTINV = 3;
        b.unknownTIAS = 42;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
