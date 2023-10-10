/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2023  Dirk Stolle

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

#include "../../../lib/locate_catch.hpp"
#include "../../../../apps/mw/name_generator/Gender.hpp"

TEST_CASE("MWTP::Gender")
{
  using namespace MWTP;

  SECTION("to_string")
  {
    const auto b = to_string(Gender::Both);
    REQUIRE_FALSE( b.empty() );

    const auto f = to_string(Gender::Female);
    REQUIRE_FALSE( f.empty() );

    const auto m = to_string(Gender::Male);
    REQUIRE_FALSE( m.empty() );

    REQUIRE_FALSE( b == f );
    REQUIRE_FALSE( b == m );
    REQUIRE_FALSE( f == m );
  }
}
