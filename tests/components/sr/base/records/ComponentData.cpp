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
#include <sstream>
#include <string_view>
#include "../../../../../sr/base/records/ComponentData.hpp"

TEST_CASE("ComponentData")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    ComponentData cd;

    REQUIRE( cd.formID == 0 );
    REQUIRE( cd.count == 0 );
  }

  SECTION("equality operator")
  {
    ComponentData a;
    ComponentData b;

    SECTION("equal")
    {
      a.formID = 0x12345678;
      a.count = 42;

      b.formID = 0x12345678;
      b.count = 42;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("form id mismatch")
      {
        a.formID = 0x12345678;
        b.formID = 0x90ABCDEF;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("count mismatch")
      {
        a.count = 1;
        b.count = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
