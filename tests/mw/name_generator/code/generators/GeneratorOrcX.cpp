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
#include "../../../../../apps/mw/name_generator/generators/GeneratorOrcBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorOrcFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorOrcMale.hpp"

TEST_CASE("MWTP::GeneratorOrc...")
{
  using namespace MWTP;

  NPCRecord murzush;
  murzush.recordID = "murzush gra-bulfim";
  murzush.Name = "Murzush gra-Bulfim";
  murzush.RaceID = "Orc";
  murzush.NPC_Flag |= 0x00000001;
  REQUIRE( murzush.isFemale() );

  NPCRecord gashnakh;
  gashnakh.recordID = "gashnakh";
  gashnakh.Name = "Gashnakh";
  gashnakh.RaceID = "Orc";
  gashnakh.NPC_Flag |= 0x00000001;
  REQUIRE( gashnakh.isFemale() );

  NPCRecord dul;
  dul.recordID = "test_dul";
  dul.Name = "Dul gro-Dush";
  dul.RaceID = "Orc";
  dul.NPC_Flag = 0;
  REQUIRE_FALSE( dul.isFemale() );

  NPCRecord ra_gruzgob;
  ra_gruzgob.recordID = "ra'gruzgob";
  ra_gruzgob.Name = "Ra'Gruzgob";
  ra_gruzgob.RaceID = "Orc";
  ra_gruzgob.NPC_Flag = 0;
  REQUIRE_FALSE( ra_gruzgob.isFemale() );

  NPCRecord foo;
  foo.recordID = "test_foo";
  foo.Name = "Somebody Else";
  foo.RaceID = "DifferentId";

  NPCs::get().clear();
  NPCs::get().addRecord(murzush);
  NPCs::get().addRecord(gashnakh);
  NPCs::get().addRecord(dul);
  NPCs::get().addRecord(ra_gruzgob);
  NPCs::get().addRecord(foo);

  SECTION("generate names for both genders")
  {
    GeneratorOrcBoth generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(120);
    REQUIRE( names.size() == 120 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody gra-Dush") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody gro-Dush") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody gra-Bulfim") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody gro-Bulfim") == names.end() );
    // Name "Murzush" should be present with either "gra-Bulfim" or "gra-Dush" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Murzush gra-Bulfim") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Murzush gra-Dush") != names.end() );
    // Name "Gashnakh" should be present with either "gra-Bulfim" or "gra-Dush" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Gashnakh gra-Bulfim") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Gashnakh gra-Dush") != names.end() );
    // Name "Dul" should be present with either "gro-Bulfim" or "gro-Dush" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Dul gro-Bulfim") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Dul gro-Dush") != names.end() );
    // Name "Ra'Gruzgob" should be present with either "gro-Bulfim" or "gro-Dush" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Ra'Gruzgob gro-Bulfim") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Ra'Gruzgob gro-Dush") != names.end() );
  }

  SECTION("generate female names")
  {
    GeneratorOrcFemale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(60);
    REQUIRE( names.size() == 60 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody gra-Dush") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody gra-Bulfim") == names.end() );
    // Name "Murzush" should be present with either "gra-Bulfim" or "gra-Dush" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Murzush gra-Bulfim") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Murzush gra-Dush") != names.end() );
    // Name "Gashnakh" should be present with either "gra-Bulfim" or "gra-Dush" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Gashnakh gra-Bulfim") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Gashnakh gra-Dush") != names.end() );
    // Name "Dul" should NOT be present with either "gra-Bulfim" or "gra-Dush" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Dul gra-Bulfim") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Dul gra-Dush") == names.end() );
    // Name "Ra'Gruzgob" should NOT be present with either "gra-Bulfim" or "gra-Dush" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Ra'Gruzgob gra-Bulfim") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Ra'Gruzgob gra-Dush") == names.end() );
  }

  SECTION("generate male names")
  {
    GeneratorOrcMale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(60);
    REQUIRE( names.size() == 60 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody gro-Dush") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody gro-Bulfim") == names.end() );
    // Name "Murzush" should NOT be present with either "gro-Bulfim" or "gro-Dush" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Murzush gro-Bulfim") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Murzush gro-Dush") == names.end() );
    // Name "Gashnakh" should NOT be present with either "gro-Bulfim" or "gro-Dush" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Gashnakh gro-Bulfim") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Gashnakh gro-Dush") == names.end() );
    // Name "Dul" should be present with either "gro-Bulfim" or "gro-Dush" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Dul gro-Bulfim") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Dul gro-Dush") != names.end() );
    // Name "Ra'Gruzgob" should be present with either "gro-Bulfim" or "gro-Dush" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Ra'Gruzgob gro-Bulfim") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Ra'Gruzgob gro-Dush") != names.end() );
  }

  SECTION("names not matching the usual format")
  {
    NPCRecord optio;
    optio.recordID = "test_optio";
    optio.Name = "Optio Bologra";
    optio.RaceID = "Orc";
    optio.NPC_Flag = 0;
    REQUIRE_FALSE( optio.isFemale() );

    NPCs::get().addRecord(optio);

    GeneratorOrcMale generator;
    const auto names = generator.generate(60);
    REQUIRE( names.size() == 60 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Bologra") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody gro-Dush") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody gro-Bulfim") == names.end() );
    // Name "Murzush" should NOT be present.
    REQUIRE( std::find(names.begin(), names.end(), "Murzush gro-Bologra") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Murzush gro-Bulfim") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Murzush gro-Dush") == names.end() );
    // Name "Gashnakh" should NOT be present.
    REQUIRE( std::find(names.begin(), names.end(), "Gashnakh gro-Bologra") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Gashnakh gro-Bulfim") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Gashnakh gro-Dush") == names.end() );
    // Name "Dul" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Dul gro-Bologra") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Dul gro-Bulfim") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Dul gro-Dush") != names.end() );
    // Name "Ra'Gruzgob" should be present.
    REQUIRE( std::find(names.begin(), names.end(), "Ra'Gruzgob gro-Bologra") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Ra'Gruzgob gro-Bulfim") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Ra'Gruzgob gro-Dush") != names.end() );
  }
}
