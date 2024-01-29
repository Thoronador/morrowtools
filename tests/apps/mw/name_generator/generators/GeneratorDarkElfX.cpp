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
#include "../../../../../apps/mw/name_generator/generators/GeneratorDarkElfBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorDarkElfFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorDarkElfMale.hpp"

TEST_CASE("MWTP::GeneratorDarkElf...")
{
  using namespace MWTP;

  // ashlanders
  NPCRecord benudni;
  benudni.recordID = "test_benudni";
  benudni.Name = "Benudni Ilurnubishpal";
  benudni.RaceID = "Dark Elf";
  benudni.ClassID = "Mabrigash";
  benudni.NPC_Flag |= 0x00000001;
  REQUIRE( benudni.isFemale() );

  NPCRecord manirai;
  manirai.recordID = "test_manirai";
  manirai.Name = "Manirai";
  manirai.FactionID = "Ashlanders";
  manirai.RaceID = "Dark Elf";
  manirai.NPC_Flag |= 0x00000001;
  REQUIRE( manirai.isFemale() );

  NPCRecord shipal;
  shipal.recordID = "shipal zansatanit";
  shipal.Name = "Shipal Zansatanit";
  shipal.RaceID = "Dark Elf";
  shipal.NPC_Flag = 0;
  REQUIRE_FALSE( shipal.isFemale() );

  NPCRecord kaushad;
  kaushad.recordID = "test_Kaushad";
  kaushad.Name = "Kaushad";
  kaushad.FactionID = "Ashlanders";
  kaushad.RaceID = "Dark Elf";
  kaushad.NPC_Flag = 0;
  REQUIRE_FALSE( kaushad.isFemale() );

  // non-ashlanders
  NPCRecord alenus;
  alenus.recordID = "test_alenus";
  alenus.Name = "Alenus Vendu";
  alenus.RaceID = "Dark Elf";
  alenus.ClassID = "Enchanter Service";
  alenus.NPC_Flag |= 0x00000001;
  REQUIRE( alenus.isFemale() );

  NPCRecord dratha;
  dratha.recordID = "test_dratha";
  dratha.Name = "Dratha";
  dratha.FactionID = "House Telvanni";
  dratha.RaceID = "Dark Elf";
  dratha.NPC_Flag |= 0x00000001;
  REQUIRE( dratha.isFemale() );

  NPCRecord galos;
  galos.recordID = "test_galos";
  galos.Name = "Galos Farethi";
  galos.RaceID = "Dark Elf";
  galos.NPC_Flag = 0;
  REQUIRE_FALSE( galos.isFemale() );

  NPCRecord aryon;
  aryon.recordID = "test_aryon";
  aryon.Name = "Aryon";
  aryon.FactionID = "House Telvannis";
  aryon.RaceID = "Dark Elf";
  aryon.NPC_Flag = 0;
  REQUIRE_FALSE( aryon.isFemale() );

  NPCRecord foo;
  foo.recordID = "test_foo";
  foo.Name = "Somebody Else";
  foo.RaceID = "DifferentId";

  NPCs::get().clear();
  NPCs::get().addRecord(benudni);
  NPCs::get().addRecord(manirai);
  NPCs::get().addRecord(shipal);
  NPCs::get().addRecord(kaushad);
  NPCs::get().addRecord(alenus);
  NPCs::get().addRecord(dratha);
  NPCs::get().addRecord(galos);
  NPCs::get().addRecord(aryon);
  NPCs::get().addRecord(foo);

  SECTION("generate names for both genders")
  {
    GeneratorDarkElfBoth generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(160);
    REQUIRE( names.size() == 160 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Zansatanit") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Farethi") == names.end() );

    // Name "Benudni" should be present with either "Ilurnubishpal" and "Zansatanit" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Benudni Ilurnubishpal") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Benudni Zansatanit") != names.end() );
    // ..., but not with "Vendu" or "Farethi" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Benudni Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Benudni Farethi") == names.end() );
    // Name "Manirai" should be present with either "Ilurnubishpal" and "Zansatanit" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Manirai Ilurnubishpal") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Manirai Zansatanit") != names.end() );
    // ..., but not with "Vendu" or "Farethi" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Manirai Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Manirai Farethi") == names.end() );
    // Name "Shipal" should be present with either "Ilurnubishpal" and "Zansatanit" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Shipal Ilurnubishpal") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Shipal Zansatanit") != names.end() );
    // ..., but not with "Vendu" or "Farethi" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Shipal Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Shipal Farethi") == names.end() );
    // Name "Kaushad" should be present with either "Ilurnubishpal" and "Zansatanit" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Kaushad Ilurnubishpal") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Kaushad Zansatanit") != names.end() );
    // ..., but not with "Vendu" or "Farethi" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Kaushad Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Kaushad Farethi") == names.end() );

    // Name "Alenus" should be present with either "Vendu" and "Farethi" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Alenus Vendu") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Alenus Farethi") != names.end() );
    // ..., but not with "Ilurnubishpal" or "Zansatanit" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Alenus Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Alenus Zansatanit") == names.end() );
    // Name "Dratha" should be present with either "Vendu" and "Farethi" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Dratha Vendu") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Dratha Farethi") != names.end() );
    // ..., but not with "Ilurnubishpal" or "Zansatanit" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Dratha Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Dratha Zansatanit") == names.end() );
    // Name "Galos" should be present with either "Vendu" and "Farethi" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Galos Vendu") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Galos Farethi") != names.end() );
    // ..., but not with "Ilurnubishpal" or "Zansatanit" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Galos Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Galos Zansatanit") == names.end() );
    // Name "Aryon" should be present with either "Vendu" and "Farethi" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Aryon Vendu") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Aryon Farethi") != names.end() );
    // ..., but not with "Ilurnubishpal" or "Zansatanit" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Aryon Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Aryon Zansatanit") == names.end() );
  }

  SECTION("generate female names")
  {
    GeneratorDarkElfFemale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(145);
    REQUIRE( names.size() == 145 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Zansatanit") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Farethi") == names.end() );

    // Name "Benudni" should be present with either "Ilurnubishpal" and "Zansatanit" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Benudni Ilurnubishpal") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Benudni Zansatanit") != names.end() );
    // ..., but not with "Vendu" or "Farethi" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Benudni Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Benudni Farethi") == names.end() );
    // Name "Manirai" should be present with either "Ilurnubishpal" and "Zansatanit" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Manirai Ilurnubishpal") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Manirai Zansatanit") != names.end() );
    // ..., but not with "Vendu" or "Farethi" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Manirai Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Manirai Farethi") == names.end() );

    // Name "Alenus" should be present with either "Vendu" and "Farethi" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Alenus Vendu") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Alenus Farethi") != names.end() );
    // ..., but not with "Ilurnubishpal" or "Zansatanit" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Alenus Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Alenus Zansatanit") == names.end() );
    // Name "Dratha" should be present with either "Vendu" and "Farethi" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Dratha Vendu") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Dratha Farethi") != names.end() );
    // ..., but not with "Ilurnubishpal" or "Zansatanit" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Dratha Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Dratha Zansatanit") == names.end() );

    // Absence of male names

    // Name "Shipal" should not be present with either "Ilurnubishpal" and "Zansatanit" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Shipal Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Shipal Zansatanit") == names.end() );
    // ..., and not with "Vendu" or "Farethi" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Shipal Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Shipal Farethi") == names.end() );
    // Name "Kaushad" should not be present with either "Ilurnubishpal" and "Zansatanit" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Kaushad Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Kaushad Zansatanit") == names.end() );
    // ..., and not with "Vendu" or "Farethi" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Kaushad Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Kaushad Farethi") == names.end() );

    // Name "Galos" should not be present with either "Vendu" and "Farethi" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Galos Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Galos Farethi") == names.end() );
    // ..., and not with "Ilurnubishpal" or "Zansatanit" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Galos Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Galos Zansatanit") == names.end() );
    // Name "Aryon" should be present with either "Vendu" and "Farethi" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Aryon Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Aryon Farethi") == names.end() );
    // ..., and not with "Ilurnubishpal" or "Zansatanit" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Aryon Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Aryon Zansatanit") == names.end() );
  }

  SECTION("generate male names")
  {
    GeneratorDarkElfMale generator;
    REQUIRE( generator.generate(2).size() == 2 );
    REQUIRE( generator.generate(3).size() == 3 );
    const auto names = generator.generate(142);
    REQUIRE( names.size() == 142 );

    // Name "Somebody" should not be present due to different race id.
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Zansatanit") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Somebody Farethi") == names.end() );

    // Name "Shipal" should be present with either "Ilurnubishpal" and "Zansatanit" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Shipal Ilurnubishpal") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Shipal Zansatanit") != names.end() );
    // ..., but not with "Vendu" or "Farethi" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Shipal Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Shipal Farethi") == names.end() );
    // Name "Kaushad" should be present with either "Ilurnubishpal" and "Zansatanit" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Kaushad Ilurnubishpal") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Kaushad Zansatanit") != names.end() );
    // ..., but not with "Vendu" or "Farethi" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Kaushad Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Kaushad Farethi") == names.end() );

    // Name "Galos" should be present with either "Vendu" and "Farethi" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Galos Vendu") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Galos Farethi") != names.end() );
    // ..., but not with "Ilurnubishpal" or "Zansatanit" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Galos Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Galos Zansatanit") == names.end() );
    // Name "Aryon" should be present with either "Vendu" and "Farethi" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Aryon Vendu") != names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Aryon Farethi") != names.end() );
    // ..., but not with "Ilurnubishpal" or "Zansatanit" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Aryon Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Aryon Zansatanit") == names.end() );

    // Absence of female names

    // Name "Benudni" should not be present with either "Ilurnubishpal" or "Zansatanit" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Benudni Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Benudni Zansatanit") == names.end() );
    // ..., and not with "Vendu" or "Farethi" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Benudni Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Benudni Farethi") == names.end() );
    // Name "Manirai" should not be present with either "Ilurnubishpal" or "Zansatanit" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Manirai Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Manirai Zansatanit") == names.end() );
    // ..., and not with "Vendu" or "Farethi" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Manirai Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Manirai Farethi") == names.end() );

    // Name "Alenus" should not be present with either "Vendu" or "Farethi" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Alenus Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Alenus Farethi") == names.end() );
    // ..., and not with "Ilurnubishpal" or "Zansatanit" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Alenus Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Alenus Zansatanit") == names.end() );
    // Name "Dratha" should not be present with either "Vendu" or "Farethi" as last name.
    REQUIRE( std::find(names.begin(), names.end(), "Dratha Vendu") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Dratha Farethi") == names.end() );
    // ..., and not with "Ilurnubishpal" or "Zansatanit" as last name, because that would be a mix-up.
    REQUIRE( std::find(names.begin(), names.end(), "Dratha Ilurnubishpal") == names.end() );
    REQUIRE( std::find(names.begin(), names.end(), "Dratha Zansatanit") == names.end() );
  }
}
