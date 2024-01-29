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
#include "../../../../../apps/mw/name_generator/generators/GeneratorHighElfBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorHighElfFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorHighElfMale.hpp"

TEST_CASE("MWTP::GeneratorHighElf...")
{
  using namespace MWTP;

  NPCRecord ardarume;
  ardarume.recordID = "ardarume";
  ardarume.Name = "Ardarume";
  ardarume.RaceID = "High Elf";
  ardarume.NPC_Flag |= 0x00000001;
  REQUIRE( ardarume.isFemale() );

  NPCRecord tenyeminwe;
  tenyeminwe.recordID = "tenyeminwe";
  tenyeminwe.Name = "Tenyeminwe";
  tenyeminwe.RaceID = "High Elf";
  tenyeminwe.NPC_Flag |= 0x00000001;
  REQUIRE( tenyeminwe.isFemale() );

  NPCRecord arrille;
  arrille.recordID = "arrille";
  arrille.Name = "Arrille";
  arrille.RaceID = "High Elf";
  arrille.NPC_Flag = 0;
  REQUIRE_FALSE( arrille.isFemale() );

  NPCRecord caryarel;
  caryarel.recordID = "caryarel";
  caryarel.Name = "Caryarel";
  caryarel.RaceID = "High Elf";
  caryarel.NPC_Flag = 0;
  REQUIRE_FALSE( caryarel.isFemale() );

  NPCRecord foo;
  foo.recordID = "test_foo";
  foo.Name = "Somebody Else";
  foo.RaceID = "DifferentId";

  NPCs::get().clear();
  NPCs::get().addRecord(ardarume);
  NPCs::get().addRecord(tenyeminwe);
  NPCs::get().addRecord(arrille);
  NPCs::get().addRecord(caryarel);
  NPCs::get().addRecord(foo);

  SECTION("generate names for both genders")
  {
    GeneratorHighElfBoth generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(60);
    REQUIRE( names.size() == 60 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Ardarume" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Ardarume") != names.end() );
    // Name "Tenyeminwe" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Tenyeminwe") != names.end() );
    // Name "Arrille" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Arrille") != names.end() );
    // Name "Caryarel" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Caryarel") != names.end() );
  }

  SECTION("generate female names")
  {
    GeneratorHighElfFemale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(45);
    REQUIRE( names.size() == 45 );


    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Ardarume" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Ardarume") != names.end() );
    // Name "Tenyeminwe" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Tenyeminwe") != names.end() );
    // Name "Arrille" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Arrille") == names.end() );
    // Name "Caryarel" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Caryarel") == names.end() );
  }

  SECTION("generate male names")
  {
    GeneratorHighElfMale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(42);
    REQUIRE( names.size() == 42 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Ardarume" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Ardarume") == names.end() );
    // Name "Tenyeminwe" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Tenyeminwe") == names.end() );
    // Name "Arrille" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Arrille") != names.end() );
    // Name "Caryarel" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Caryarel") != names.end() );
  }
}
