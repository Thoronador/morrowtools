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

#include "../locate_catch.hpp"
#include <string>
#include "../../../lib/base/ComparisonFunctor.hpp"

TEST_CASE("ComparisonFunctor: ci_less")
{
  using namespace MWTP;

  const ci_less is_less;

  SECTION("ci_less: x less than y")
  {
    REQUIRE( is_less("abc", "bcde") );
    REQUIRE( is_less("ABC", "bcdef") );
    REQUIRE( is_less("abc", "BCDf") );
    REQUIRE( is_less("aBcD", "bCdE") );
    REQUIRE( is_less("AbCd", "bCdE") );
    REQUIRE( is_less("AbCd", "BcDe") );
    REQUIRE( is_less("aBcD", "BcDe") );
  }

  SECTION("ci_less: x equal to y")
  {
    REQUIRE_FALSE( is_less("abc", "abc") );
    REQUIRE_FALSE( is_less("ABC", "abc") );
    REQUIRE_FALSE( is_less("abc", "ABC") );
  }

  SECTION("ci_less: x greater than y")
  {
    REQUIRE_FALSE( is_less("def", "abc") );
    REQUIRE_FALSE( is_less("DEF", "abc") );
    REQUIRE_FALSE( is_less("def", "ABC") );
  }
}
