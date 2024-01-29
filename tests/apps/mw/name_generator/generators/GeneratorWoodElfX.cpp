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
#include "../../../../../apps/mw/name_generator/generators/GeneratorWoodElfBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorWoodElfFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorWoodElfMale.hpp"

TEST_CASE("MWTP::GeneratorWoodElf...")
{
  using namespace MWTP;

  NPCRecord berwen;
  berwen.recordID = "berwen";
  berwen.Name = "Berwen";
  berwen.RaceID = "Wood Elf";
  berwen.NPC_Flag |= 0x00000001;
  REQUIRE( berwen.isFemale() );

  NPCRecord eraldil;
  eraldil.recordID = "eraldil";
  eraldil.Name = "Eraldil";
  eraldil.RaceID = "Wood Elf";
  eraldil.NPC_Flag |= 0x00000001;
  REQUIRE( eraldil.isFemale() );

  NPCRecord baradras;
  baradras.recordID = "baradras";
  baradras.Name = "Baradras";
  baradras.RaceID = "Wood Elf";
  baradras.NPC_Flag = 0;
  REQUIRE_FALSE( baradras.isFemale() );

  NPCRecord meldor;
  meldor.recordID = "meldor";
  meldor.Name = "Meldor";
  meldor.RaceID = "Wood Elf";
  meldor.NPC_Flag = 0;
  REQUIRE_FALSE( meldor.isFemale() );

  NPCRecord foo;
  foo.recordID = "test_foo";
  foo.Name = "Somebody Else";
  foo.RaceID = "DifferentId";

  NPCs::get().clear();
  NPCs::get().addRecord(berwen);
  NPCs::get().addRecord(eraldil);
  NPCs::get().addRecord(baradras);
  NPCs::get().addRecord(meldor);
  NPCs::get().addRecord(foo);

  SECTION("generate names for both genders")
  {
    GeneratorWoodElfBoth generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(60);
    REQUIRE( names.size() == 60 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Berwen" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Berwen") != names.end() );
    // Name "Eraldil" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Eraldil") != names.end() );
    // Name "Baradras" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Baradras") != names.end() );
    // Name "Meldor" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Meldor") != names.end() );
  }

  SECTION("generate female names")
  {
    GeneratorWoodElfFemale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(45);
    REQUIRE( names.size() == 45 );


    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Berwen" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Berwen") != names.end() );
    // Name "Eraldil" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Eraldil") != names.end() );
    // Name "Baradras" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Baradras") == names.end() );
    // Name "Meldor" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Meldor") == names.end() );
  }

  SECTION("generate male names")
  {
    GeneratorWoodElfMale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(42);
    REQUIRE( names.size() == 42 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Berwen" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Berwen") == names.end() );
    // Name "Eraldil" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Eraldil") == names.end() );
    // Name "Baradras" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Baradras") != names.end() );
    // Name "Meldor" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Meldor") != names.end() );
  }
}
