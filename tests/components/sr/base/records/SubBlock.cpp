/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
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
#include <sstream>
#include <string_view>
#include "../../../../../sr/base/records/SubBlock.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"

TEST_CASE("SubBlock")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    SubBlock block;

    REQUIRE( block.subType == 0 );
    REQUIRE_FALSE( block.subData.isPresent() );
  }

  SECTION("equality operator")
  {
    SubBlock a;
    SubBlock b;

    SECTION("equal")
    {
      a.subType = cVMAD;
      b.subType = cVMAD;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("subType mismatch")
      {
        a.subType = cTINC;
        b.subType = cVMAD;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("data mismatch")
      {
        a.subData.setPresence(true);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
