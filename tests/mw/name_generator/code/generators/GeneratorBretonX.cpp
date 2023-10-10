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
#include "../../../../../apps/mw/name_generator/generators/GeneratorBretonBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorBretonFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorBretonMale.hpp"

TEST_CASE("MWTP::GeneratorBreton...")
{
  using namespace MWTP;

  NPCRecord aurane;
  aurane.recordID = "aurane frernis";
  aurane.Name = "Aurane Frernis";
  aurane.RaceID = "Breton";
  aurane.NPC_Flag |= 0x00000001;
  REQUIRE( aurane.isFemale() );

  NPCRecord marelle;
  marelle.recordID = "breton dancer girl";
  marelle.Name = "Marelle";
  marelle.RaceID = "Breton";
  marelle.NPC_Flag |= 0x00000001;
  REQUIRE( marelle.isFemale() );

  NPCRecord perien;
  perien.recordID = "test_perien";
  perien.Name = "Perien Aurelie";
  perien.RaceID = "Breton";
  perien.NPC_Flag = 0;
  REQUIRE_FALSE( perien.isFemale() );

  NPCRecord inwold;
  inwold.recordID = "Ingwold";
  inwold.Name = "Inwold";
  inwold.RaceID = "Breton";
  inwold.NPC_Flag = 0;
  REQUIRE_FALSE( inwold.isFemale() );

  NPCRecord foo;
  foo.recordID = "test_foo";
  foo.Name = "Somebody Else";
  foo.RaceID = "DifferentId";

  NPCs::get().clear();
  NPCs::get().addRecord(aurane);
  NPCs::get().addRecord(marelle);
  NPCs::get().addRecord(perien);
  NPCs::get().addRecord(inwold);
  NPCs::get().addRecord(foo);

  SECTION("generate names for both genders")
  {
    GeneratorBretonBoth generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(120);
    REQUIRE( names.size() == 120 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Aurelie") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Frernis") == names.end() );
    // Name "Aurane" should be present with either "Aurelie" or "Frernis" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Aurane Aurelie") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Aurane Frernis") != names.end() );
    // Name "Marelle" should be present with either "Aurelie" or "Frernis" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Marelle Aurelie") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Marelle Frernis") != names.end() );
    // Name "Perien" should be present with either "Aurelie" or "Frernis" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Perien Aurelie") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Perien Frernis") != names.end() );
    // Name "Inwold" should be present with either "Aurelie" or "Frernis" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Inwold Aurelie") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Inwold Frernis") != names.end() );
  }

  SECTION("generate female names")
  {
    GeneratorBretonFemale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(60);
    REQUIRE( names.size() == 60 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Aurelie") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Frernis") == names.end() );
    // Name "Aurane" should be present with either "Aurelie" or "Frernis" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Aurane Aurelie") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Aurane Frernis") != names.end() );
    // Name "Marelle" should be present with either "Aurelie" or "Frernis" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Marelle Aurelie") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Marelle Frernis") != names.end() );
    // Name "Perien" should NOT be present with either "Aurelie" or "Frernis" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Perien Aurelie") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Perien Frernis") == names.end() );
    // Name "Inwold" should NOT be present with either "Aurelie" or "Frernis" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Inwold Aurelie") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Inwold Frernis") == names.end() );
  }

  SECTION("generate male names")
  {
    GeneratorBretonMale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(60);
    REQUIRE( names.size() == 60 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Aurelie") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Frernis") == names.end() );
    // Name "Aurane" should NOT be present with either "Aurelie" or "Frernis" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Aurane Aurelie") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Aurane Frernis") == names.end() );
    // Name "Marelle" should NOT be present with either "Aurelie" or "Frernis" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Marelle Aurelie") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Marelle Frernis") == names.end() );
    // Name "Perien" should be present with either "Aurelie" or "Frernis" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Perien Aurelie") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Perien Frernis") != names.end() );
    // Name "Inwold" should be present with either "Aurelie" or "Frernis" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Inwold Aurelie") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Inwold Frernis") != names.end() );
  }
}
