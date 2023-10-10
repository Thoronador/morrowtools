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
#include "../../../../../apps/mw/name_generator/generators/SplitNamesBoth.hpp"

TEST_CASE("MWTP::SplitNamesBoth")
{
  using namespace MWTP;

  SECTION("generate")
  {
    NPCRecord jane;
    jane.recordID = "test_jane";
    jane.Name = "Jane Example";
    jane.RaceID = "SomeTestId";
    jane.NPC_Flag |= 0x00000001;
    REQUIRE( jane.isFemale() );

    NPCRecord barbara;
    barbara.recordID = "test_barbara";
    barbara.Name = "Barbara";
    barbara.RaceID = "SomeTestId";
    barbara.NPC_Flag |= 0x00000001;
    REQUIRE( barbara.isFemale() );

    NPCRecord john;
    john.recordID = "test_john";
    john.Name = "John Doe";
    john.RaceID = "SomeTestId";
    john.NPC_Flag = 0;
    REQUIRE_FALSE( john.isFemale() );

    NPCRecord bob;
    bob.recordID = "test_bob";
    bob.Name = "Bob";
    bob.RaceID = "SomeTestId";
    bob.NPC_Flag = 0;
    REQUIRE_FALSE( bob.isFemale() );

    NPCRecord foo;
    foo.recordID = "test_foo";
    foo.Name = "Somebody Else";
    foo.RaceID = "DifferentId";

    NPCs::get().clear();
    NPCs::get().addRecord(jane);
    NPCs::get().addRecord(barbara);
    NPCs::get().addRecord(john);
    NPCs::get().addRecord(bob);
    NPCs::get().addRecord(foo);

    SplitNamesBoth generator;
    REQUIRE( generator.generate(3, "SomeTestId").size() == 3 );
    REQUIRE( generator.generate(5, "SomeTestId").size() == 5 );
    const auto names = generator.generate(120, "SomeTestId");
    REQUIRE( names.size() == 120 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Example") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Doe") == names.end() );
    // Names "Jane" should be present with either "Example" and "Doe" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Jane Doe") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Jane Example") != names.end() );
    // Names "Barbara" should be present with either "Example" and "Doe" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Barbara Doe") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Barbara Example") != names.end() );
    // Names "John" should be present with either "Example" and "Doe" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "John Doe") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "John Example") != names.end() );
    // Names "Bob" should be present with either "Example" and "Doe" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Bob Doe") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Bob Example") != names.end() );
  }
}
