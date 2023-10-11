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
#include "../../../../../apps/mw/name_generator/generators/GeneratorRedguardBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorRedguardFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorRedguardMale.hpp"

TEST_CASE("MWTP::GeneratorRedguard...")
{
  using namespace MWTP;

  NPCRecord malexa;
  malexa.recordID = "malexa";
  malexa.Name = "Malexa";
  malexa.RaceID = "Redguard";
  malexa.NPC_Flag |= 0x00000001;
  REQUIRE( malexa.isFemale() );

  NPCRecord rithleen;
  rithleen.recordID = "rithleen";
  rithleen.Name = "Rithleen";
  rithleen.RaceID = "Redguard";
  rithleen.NPC_Flag |= 0x00000001;
  REQUIRE( rithleen.isFemale() );

  NPCRecord hasell;
  hasell.recordID = "hasell";
  hasell.Name = "Hasell";
  hasell.RaceID = "Redguard";
  hasell.NPC_Flag = 0;
  REQUIRE_FALSE( hasell.isFemale() );

  NPCRecord sterdecan;
  sterdecan.recordID = "sterdecan";
  sterdecan.Name = "Sterdecan";
  sterdecan.RaceID = "Redguard";
  sterdecan.NPC_Flag = 0;
  REQUIRE_FALSE( sterdecan.isFemale() );

  NPCRecord foo;
  foo.recordID = "test_foo";
  foo.Name = "Somebody Else";
  foo.RaceID = "DifferentId";

  NPCs::get().clear();
  NPCs::get().addRecord(malexa);
  NPCs::get().addRecord(rithleen);
  NPCs::get().addRecord(hasell);
  NPCs::get().addRecord(sterdecan);
  NPCs::get().addRecord(foo);

  SECTION("generate names for both genders")
  {
    GeneratorRedguardBoth generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(60);
    REQUIRE( names.size() == 60 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Malexa" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Malexa") != names.end() );
    // Name "Rithleen" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Rithleen") != names.end() );
    // Name "Hasell" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Hasell") != names.end() );
    // Name "Sterdecan" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Sterdecan") != names.end() );
  }

  SECTION("generate female names")
  {
    GeneratorRedguardFemale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(45);
    REQUIRE( names.size() == 45 );


    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Malexa" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Malexa") != names.end() );
    // Name "Rithleen" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Rithleen") != names.end() );
    // Name "Hasell" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Hasell") == names.end() );
    // Name "Sterdecan" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Sterdecan") == names.end() );
  }

  SECTION("generate male names")
  {
    GeneratorRedguardMale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(42);
    REQUIRE( names.size() == 42 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Malexa" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Malexa") == names.end() );
    // Name "Rithleen" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Rithleen") == names.end() );
    // Name "Hasell" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Hasell") != names.end() );
    // Name "Sterdecan" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Sterdecan") != names.end() );
  }
}
