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
#include "../../../../sr/formID_finder/AuxFunctions.hpp"

TEST_CASE("AuxFunctions")
{
  SECTION("matchesKeyword")
  {
    SECTION("case-sensitive")
    {

      REQUIRE( matchesKeyword("foobar", "foo", true) );
      REQUIRE( matchesKeyword("foobar", "bar", true) );
      REQUIRE( matchesKeyword("foobar", "ooba", true) );
      REQUIRE_FALSE( matchesKeyword("foobar", "Foo", true) );
      REQUIRE_FALSE( matchesKeyword("foobar", "Bar", true) );
      REQUIRE_FALSE( matchesKeyword("foobar", "OOBA", true) );
      REQUIRE_FALSE( matchesKeyword("Foobar", "foo", true) );
      REQUIRE_FALSE( matchesKeyword("fooBAR", "bar", true) );
      REQUIRE_FALSE( matchesKeyword("fOOBar", "ooba", true) );

      REQUIRE_FALSE( matchesKeyword("foobar", "baz", true) );
    }

    SECTION("case-insensitive")
    {
      REQUIRE( matchesKeyword("foobar", "foo", false) );
      REQUIRE( matchesKeyword("foobar", "bar", false) );
      REQUIRE( matchesKeyword("foobar", "ooba", false) );
      REQUIRE( matchesKeyword("Foobar", "foo", false) );
      REQUIRE( matchesKeyword("fooBAR", "bar", false) );
      REQUIRE( matchesKeyword("fOOBar", "ooba", false) );

      REQUIRE_FALSE( matchesKeyword("foobar", "baz", false) );
    }

    SECTION("empty haystack")
    {
      REQUIRE_FALSE( matchesKeyword("", "foo", true) );
      REQUIRE_FALSE( matchesKeyword("", "foo", false) );
    }
  }
}
