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
#include "../../../../../apps/mw/name_generator/generators/GeneratorKhajiitBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorKhajiitFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorKhajiitMale.hpp"

TEST_CASE("MWTP::GeneratorKhajiit...")
{
  using namespace MWTP;

  NPCRecord inorra;
  inorra.recordID = "inorra";
  inorra.Name = "Inorra";
  inorra.RaceID = "Khajiit";
  inorra.NPC_Flag |= 0x00000001;
  REQUIRE( inorra.isFemale() );

  NPCRecord tsalani;
  tsalani.recordID = "tsalani";
  tsalani.Name = "Tsalani";
  tsalani.RaceID = "Khajiit";
  tsalani.NPC_Flag |= 0x00000001;
  REQUIRE( tsalani.isFemale() );

  NPCRecord jobasha;
  jobasha.recordID = "jobasha";
  jobasha.Name = "Jobasha";
  jobasha.RaceID = "Khajiit";
  jobasha.NPC_Flag = 0;
  REQUIRE_FALSE( jobasha.isFemale() );

  NPCRecord j_zhirr;
  j_zhirr.recordID = "j'zhirr";
  j_zhirr.Name = "J'Zhirr";
  j_zhirr.RaceID = "Khajiit";
  j_zhirr.NPC_Flag = 0;
  REQUIRE_FALSE( j_zhirr.isFemale() );

  NPCRecord foo;
  foo.recordID = "test_foo";
  foo.Name = "Somebody Else";
  foo.RaceID = "DifferentId";

  NPCs::get().clear();
  NPCs::get().addRecord(inorra);
  NPCs::get().addRecord(tsalani);
  NPCs::get().addRecord(jobasha);
  NPCs::get().addRecord(j_zhirr);
  NPCs::get().addRecord(foo);

  SECTION("generate names for both genders")
  {
    GeneratorKhajiitBoth generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(60);
    REQUIRE( names.size() == 60 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Inorra" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Inorra") != names.end() );
    // Name "Tsalani" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Tsalani") != names.end() );
    // Name "Jobasha" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Jobasha") != names.end() );
    // Name "J'Zhirr" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "J'Zhirr") != names.end() );
  }

  SECTION("generate female names")
  {
    GeneratorKhajiitFemale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(45);
    REQUIRE( names.size() == 45 );


    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Inorra" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Inorra") != names.end() );
    // Name "Tsalani" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Tsalani") != names.end() );
    // Name "Jobasha" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Jobasha") == names.end() );
    // Name "J'Zhirr" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "J'Zhirr") == names.end() );
  }

  SECTION("generate male names")
  {
    GeneratorKhajiitMale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(42);
    REQUIRE( names.size() == 42 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    // Name "Inorra" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Inorra") == names.end() );
    // Name "Tsalani" should not be present due to different gender.
    REQUIRE( std::find(names.begin(), names.end(), "Tsalani") == names.end() );
    // Name "Jobasha" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Jobasha") != names.end() );
    // Name "J'Zhirr" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "J'Zhirr") != names.end() );
  }
}
