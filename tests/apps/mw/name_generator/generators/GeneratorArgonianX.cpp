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
#include "../../../../../apps/mw/name_generator/generators/GeneratorArgonianBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorArgonianFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorArgonianMale.hpp"

TEST_CASE("MWTP::GeneratorArgonian...")
{
  using namespace MWTP;

  NPCRecord onasha;
  onasha.recordID = "onasha";
  onasha.Name = "Onasha";
  onasha.RaceID = "Argonian";
  onasha.NPC_Flag |= 0x00000001;
  REQUIRE( onasha.isFemale() );

  NPCRecord an_deesei;
  an_deesei.recordID = "An_Deesei";
  an_deesei.Name = "An-Deesei";
  an_deesei.RaceID = "Argonian";
  an_deesei.NPC_Flag |= 0x00000001;
  REQUIRE( an_deesei.isFemale() );

  NPCRecord chuna;
  chuna.recordID = "chuna";
  chuna.Name = "Chuna";
  chuna.RaceID = "Argonian";
  chuna.NPC_Flag = 0;
  REQUIRE_FALSE( chuna.isFemale() );

  NPCRecord an_zaw;
  an_zaw.recordID = "an-zaw";
  an_zaw.Name = "An-Zaw";
  an_zaw.RaceID = "Argonian";
  an_zaw.NPC_Flag = 0;
  REQUIRE_FALSE( an_zaw.isFemale() );

  NPCRecord foo;
  foo.recordID = "test_foo";
  foo.Name = "Somebody Else";
  foo.RaceID = "DifferentId";

  NPCs::get().clear();
  NPCs::get().addRecord(onasha);
  NPCs::get().addRecord(an_deesei);
  NPCs::get().addRecord(chuna);
  NPCs::get().addRecord(an_zaw);
  NPCs::get().addRecord(foo);

  SECTION("generate names for both genders")
  {
    GeneratorArgonianBoth generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(60);
    REQUIRE( names.size() == 60 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Onasha" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Onasha") != names.end() );
    // Name "An-Deesei" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "An-Deesei") != names.end() );
    // Name "Chuna" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Chuna") != names.end() );
    // Name "An-Zaw" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "An-Zaw") != names.end() );
  }

  SECTION("generate female names")
  {
    GeneratorArgonianFemale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(45);
    REQUIRE( names.size() == 45 );


    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Onasha" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Onasha") != names.end() );
    // Name "An-Deesei" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "An-Deesei") != names.end() );
    // Name "Chuna" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Chuna") == names.end() );
    // Name "An-Zaw" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "An-Zaw") == names.end() );
  }

  SECTION("generate male names")
  {
    GeneratorArgonianMale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(42);
    REQUIRE( names.size() == 42 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Onasha" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Onasha") == names.end() );
    // Name "An-Deesei" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "An-Deesei") == names.end() );
    // Name "Chuna" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Chuna") != names.end() );
    // Name "An-Zaw" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "An-Zaw") != names.end() );
  }
}
