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
#include "../../../../../mw/base/records/ItemRecord.hpp"

TEST_CASE("MWTP::ItemRecord")
{
  using namespace MWTP;

  SECTION("constructor")
  {
    ItemRecord item;

    REQUIRE( item.Count == 0 );
    REQUIRE( item.Item.empty() );
  }

  SECTION("equality operator")
  {
    ItemRecord a;
    ItemRecord b;

    SECTION("equal")
    {
      a.Count = 1;
      a.Item = "foo";

      b.Count = 1;
      b.Item = "foo";

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("count mismatch")
      {
        a.Count = 1;
        b.Count = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Item mismatch")
      {
        a.Item = "foo";
        b.Item = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    SECTION("long id gets truncated")
    {
      ItemRecord item;
      item.Count = 5;
      item.Item = "veryLongIdentifierThatShouldBeCutDown";

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( item.saveToStream(streamOut) );
      // Check written data.
      const auto data = "NPCO$\0\0\0\x05\0\0\0veryLongIdentifierThatShouldBeC\0"sv;
      REQUIRE( streamOut.str() == data );
    }
  }
}
