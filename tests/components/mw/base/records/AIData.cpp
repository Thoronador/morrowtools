/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021, 2022  Dirk Stolle

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
#include "../../../../../mw/base/records/AIData.hpp"

TEST_CASE("MWTP::NPC_AIData")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    NPC_AIData data;

    REQUIRE( data.Hello == 0 );
    REQUIRE( data.Unknown1 == 0 );
    REQUIRE( data.Fight == 0 );
    REQUIRE( data.Flee == 0 );
    REQUIRE( data.Alarm == 0 );
    REQUIRE( data.Unknown2 == 0 );
    REQUIRE( data.Unknown3 == 0 );
    REQUIRE( data.Unknown4 == 0 );
    REQUIRE( data.Flags == 0 );
  }

  SECTION("equals")
  {
    NPC_AIData a;
    NPC_AIData b;

    SECTION("equal")
    {
      a.Hello = 1;
      b.Hello = 1;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("Hello mismatch")
      {
        a.Hello = 1;
        b.Hello = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Unknown1 mismatch")
      {
        a.Unknown1 = 1;
        b.Unknown1 = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Fight mismatch")
      {
        a.Fight = 1;
        b.Fight = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Flee mismatch")
      {
        a.Flee = 1;
        b.Flee = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Alarm mismatch")
      {
        a.Alarm = 1;
        b.Alarm = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Unknown2 mismatch")
      {
        a.Unknown2 = 1;
        b.Unknown2 = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Unknown3 mismatch")
      {
        a.Unknown3 = 1;
        b.Unknown3 = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Unknown4 mismatch")
      {
        a.Unknown4 = 1;
        b.Unknown4 = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Flags mismatch")
      {
        a.Flags = 1;
        b.Flags = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("clear")
  {
    NPC_AIData data;

    data.Hello = 1;
    data.Unknown1 = 1;
    data.Fight = 1;
    data.Flee = 1;
    data.Alarm = 1;
    data.Unknown2 = 1;
    data.Unknown3 = 1;
    data.Unknown4 = 1;
    data.Flags = 1;

    data.clear();

    REQUIRE( data.Hello == 0 );
    REQUIRE( data.Unknown1 == 0 );
    REQUIRE( data.Fight == 0 );
    REQUIRE( data.Flee == 0 );
    REQUIRE( data.Alarm == 0 );
    REQUIRE( data.Unknown2 == 0 );
    REQUIRE( data.Unknown3 == 0 );
    REQUIRE( data.Unknown4 == 0 );
    REQUIRE( data.Flags == 0 );
  }
}
