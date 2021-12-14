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

TEST_CASE("FactionRecord::RankData")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    FactionRecord::RankData data;

    REQUIRE( data.index == 0 );
    REQUIRE_FALSE( data.maleName.isPresent() );
    REQUIRE_FALSE( data.femaleName.isPresent() );
  }

  SECTION("equals")
  {
    FactionRecord::RankData a;
    FactionRecord::RankData b;

    SECTION("equal")
    {
      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("index mismatch")
      {
        a.index = 1;
        b.index = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("maleName mismatch")
      {
        a.maleName = LocalizedString(LocalizedString::Type::Index, 1, "");
        b.maleName = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("femaleName mismatch")
      {
        a.femaleName = LocalizedString(LocalizedString::Type::Index, 1, "");
        b.femaleName = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
