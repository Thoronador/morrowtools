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
#include "../../../lib/sr/Localization.hpp"

TEST_CASE("Localization")
{
  using namespace SRTP;

  SECTION("stringTableSuffix")
  {
    REQUIRE( stringTableSuffix(Localization::English) == "english" );
    REQUIRE( stringTableSuffix(Localization::French) == "french" );
    REQUIRE( stringTableSuffix(Localization::German) == "german" );
    REQUIRE( stringTableSuffix(Localization::Italian) == "italian" );
    REQUIRE( stringTableSuffix(Localization::Polish) == "polish" );
    REQUIRE( stringTableSuffix(Localization::Russian) == "russian" );
    REQUIRE( stringTableSuffix(Localization::Spanish) == "spanish" );
  }
}
