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

#include "../../../../lib/locate_catch.hpp"
#include <algorithm>
#include "../../../../../lib/mw/NPCs.hpp"
#include "../../../../../apps/mw/name_generator/generators/WholeNamesMale.hpp"

TEST_CASE("MWTP::WholeNamesMale")
{
  using namespace MWTP;

  SECTION("generate")
  {
    NPCRecord jane;
    jane.recordID = "test_jane";
    jane.Name = "Jane";
    jane.RaceID = "SomeTestId";
    jane.NPC_Flag |= 0x00000001;
    REQUIRE( jane.isFemale() );

    NPCRecord john;
    john.recordID = "test_john";
    john.Name = "John";
    john.RaceID = "SomeTestId";
    john.NPC_Flag = 0;
    REQUIRE_FALSE( john.isFemale() );

    NPCRecord foo;
    foo.recordID = "test_foo";
    foo.Name = "Somebody";
    foo.RaceID = "DifferentId";

    NPCs::get().clear();
    NPCs::get().addRecord(jane);
    NPCs::get().addRecord(john);
    NPCs::get().addRecord(foo);

    WholeNamesMale generator;
    const auto names = generator.generate(7, "SomeTestId");
    REQUIRE( names.size() == 7 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody") == names.end() );
    // Name "Jane" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Jane") == names.end() );
    // Names "John" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "John") != names.end() );
  }
}
