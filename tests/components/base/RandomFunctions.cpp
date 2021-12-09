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
#include "../../../base/RandomFunctions.hpp"

TEST_CASE("RandomFunctions")
{
  using namespace MWTP;

  SECTION("randomAlphaNumericSequence")
  {
    SECTION("size changes with requested length")
    {
      REQUIRE( randomAlphaNumericSequence(0) == "" );
      REQUIRE( randomAlphaNumericSequence(1).size() == 1 );
      REQUIRE( randomAlphaNumericSequence(2).size() == 2 );
      REQUIRE( randomAlphaNumericSequence(3).size() == 3 );
      REQUIRE( randomAlphaNumericSequence(4).size() == 4 );
      REQUIRE( randomAlphaNumericSequence(5).size() == 5 );
      REQUIRE( randomAlphaNumericSequence(6).size() == 6 );
    }

    SECTION("random stuff is random")
    {
      const auto s1 = randomAlphaNumericSequence(10);
      REQUIRE( s1.size() == 10 );
      const auto s2 = randomAlphaNumericSequence(10);
      REQUIRE( s2.size() == 10 );
      // Output should rarely ever be the same for two consecutive calls.
      REQUIRE_FALSE( s1 == s2 );
    }
  }
}
