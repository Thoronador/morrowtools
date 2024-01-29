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
#include "../../../../../apps/mw/name_generator/generators/GeneratorNordBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorNordFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorNordMale.hpp"

TEST_CASE("MWTP::GeneratorNord...")
{
  using namespace MWTP;

  NPCRecord sottilde;
  sottilde.recordID = "sottilde";
  sottilde.Name = "Sottilde";
  sottilde.RaceID = "Nord";
  sottilde.NPC_Flag |= 0x00000001;
  REQUIRE( sottilde.isFemale() );

  NPCRecord fryfnhild;
  fryfnhild.recordID = "fryfnhild";
  fryfnhild.Name = "Fryfnhild";
  fryfnhild.RaceID = "Nord";
  fryfnhild.NPC_Flag |= 0x00000001;
  REQUIRE( fryfnhild.isFemale() );

  NPCRecord hrundi;
  hrundi.recordID = "hrundi";
  hrundi.Name = "Hrundi";
  hrundi.RaceID = "Nord";
  hrundi.NPC_Flag = 0;
  REQUIRE_FALSE( hrundi.isFemale() );

  NPCRecord fjol;
  fjol.recordID = "fjol";
  fjol.Name = "Fjol";
  fjol.RaceID = "Nord";
  fjol.NPC_Flag = 0;
  REQUIRE_FALSE( fjol.isFemale() );

  NPCRecord foo;
  foo.recordID = "test_foo";
  foo.Name = "Somebody Else";
  foo.RaceID = "DifferentId";

  NPCs::get().clear();
  NPCs::get().addRecord(sottilde);
  NPCs::get().addRecord(fryfnhild);
  NPCs::get().addRecord(hrundi);
  NPCs::get().addRecord(fjol);
  NPCs::get().addRecord(foo);

  SECTION("generate names for both genders")
  {
    GeneratorNordBoth generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(60);
    REQUIRE( names.size() == 60 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Sottilde" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Sottilde") != names.end() );
    // Name "Fryfnhild" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Fryfnhild") != names.end() );
    // Name "Hrundi" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Hrundi") != names.end() );
    // Name "Fjol" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Fjol") != names.end() );
  }

  SECTION("generate female names")
  {
    GeneratorNordFemale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(45);
    REQUIRE( names.size() == 45 );


    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Sottilde" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Sottilde") != names.end() );
    // Name "Fryfnhild" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Fryfnhild") != names.end() );
    // Name "Hrundi" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Hrundi") == names.end() );
    // Name "Fjol" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Fjol") == names.end() );
  }

  SECTION("generate male names")
  {
    GeneratorNordMale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(42);
    REQUIRE( names.size() == 42 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Sottilde" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Sottilde") == names.end() );
    // Name "Fryfnhild" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Fryfnhild") == names.end() );
    // Name "Hrundi" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Hrundi") != names.end() );
    // Name "Fjol" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Fjol") != names.end() );
  }
}
