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
#include "../../../../../apps/mw/name_generator/generators/GeneratorImperialBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorImperialFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorImperialMale.hpp"

TEST_CASE("MWTP::GeneratorImperial...")
{
  using namespace MWTP;

  NPCRecord agrippina;
  agrippina.recordID = "agrippina herennia";
  agrippina.Name = "Agrippina Herennia";
  agrippina.RaceID = "Imperial";
  agrippina.NPC_Flag |= 0x00000001;
  REQUIRE( agrippina.isFemale() );

  NPCRecord germia;
  germia.recordID = "germia";
  germia.Name = "Germia";
  germia.RaceID = "Imperial";
  germia.NPC_Flag |= 0x00000001;
  REQUIRE( germia.isFemale() );

  NPCRecord albecius;
  albecius.recordID = "test_Albecius";
  albecius.Name = "Albecius Colollius";
  albecius.RaceID = "Imperial";
  albecius.NPC_Flag = 0;
  REQUIRE_FALSE( albecius.isFemale() );

  NPCRecord calvario;
  calvario.recordID = "Calvario";
  calvario.Name = "Calvario";
  calvario.RaceID = "Imperial";
  calvario.NPC_Flag = 0;
  REQUIRE_FALSE( calvario.isFemale() );

  NPCRecord foo;
  foo.recordID = "test_foo";
  foo.Name = "Somebody Else";
  foo.RaceID = "DifferentId";

  NPCs::get().clear();
  NPCs::get().addRecord(agrippina);
  NPCs::get().addRecord(germia);
  NPCs::get().addRecord(albecius);
  NPCs::get().addRecord(calvario);
  NPCs::get().addRecord(foo);

  SECTION("generate names for both genders")
  {
    GeneratorImperialBoth generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(120);
    REQUIRE( names.size() == 120 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Herennia") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Colollius") == names.end() );
    // Name "Agrippina" should be present with either "Herennia" or "Colollius" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Agrippina Herennia") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Agrippina Colollius") != names.end() );
    // Name "Germia" should be present with either "Herennia" or "Colollius" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Germia Herennia") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Germia Colollius") != names.end() );
    // Name "Albecius" should be present with either "Herennia" or "Colollius" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Albecius Herennia") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Albecius Colollius") != names.end() );
    // Name "Calvario" should be present with either "Herennia" or "Colollius" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Calvario Herennia") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Calvario Colollius") != names.end() );
  }

  SECTION("generate female names")
  {
    GeneratorImperialFemale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(60);
    REQUIRE( names.size() == 60 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Herennia") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Colollius") == names.end() );
    // Name "Agrippina" should be present with either "Herennia" or "Colollius" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Agrippina Herennia") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Agrippina Colollius") != names.end() );
    // Name "Germia" should be present with either "Herennia" or "Colollius" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Germia Herennia") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Germia Colollius") != names.end() );
    // Name "Albecius" should NOT be present with either "Herennia" or "Colollius" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Albecius Herennia") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Albecius Colollius") == names.end() );
    // Name "Calvario" should NOT be present with either "Herennia" or "Colollius" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Calvario Herennia") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Calvario Colollius") == names.end() );
  }

  SECTION("generate male names")
  {
    GeneratorImperialMale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(60);
    REQUIRE( names.size() == 60 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Herennia") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Colollius") == names.end() );
    // Name "Agrippina" should NOT be present with either "Herennia" or "Colollius" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Agrippina Herennia") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Agrippina Colollius") == names.end() );
    // Name "Germia" should NOT be present with either "Herennia" or "Colollius" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Germia Herennia") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Germia Colollius") == names.end() );
    // Name "Albecius" should be present with either "Herennia" or "Colollius" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Albecius Herennia") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Albecius Colollius") != names.end() );
    // Name "Calvario" should be present with either "Herennia" or "Colollius" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Calvario Herennia") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Calvario Colollius") != names.end() );
  }
}
