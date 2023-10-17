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
#include "../../../../../apps/mw/name_generator/generators/SplitNamesDarkElf.hpp"

TEST_CASE("MWTP::SplitNamesDarkElf")
{
  using namespace MWTP;

  SECTION("is_ashlander()")
  {
    NPCRecord npc;
    npc.recordID = "test_foo";
    npc.Name = "Foo Bar";
    npc.RaceID = "Dark Elf";

    SECTION("faction Ashlanders belongs to ashlanders")
    {
      npc.FactionID = "Ashlanders";
      REQUIRE( SplitNamesDarkElf::is_ashlander(npc) );
    }

    SECTION("class Mabrigash belongs to ashlanders")
    {
      // Yes, technically, Mabrigash are exiled ashlanders, but they use the
      // same naming patterns as other ashlanders, so for this purpose they
      // are considered ashlanders, too.
      npc.FactionID = "";
      npc.ClassID = "Mabrigash";
      REQUIRE( SplitNamesDarkElf::is_ashlander(npc) );
    }

    SECTION("class Wise Woman belongs to ashlanders")
    {
      npc.FactionID = "";
      npc.ClassID = "Wise Woman";
      REQUIRE( SplitNamesDarkElf::is_ashlander(npc) );
    }

    SECTION("NPCs whose ID is listed as ashlander should belong to ashlanders")
    {
      npc.FactionID = "Some Guild ID";
      npc.ClassID = "Scout";
      npc.recordID = "assurdan serdimapal";
      npc.Name = "Assurdan Serdimapal";
      REQUIRE( SplitNamesDarkElf::is_ashlander(npc) );

      npc.Name = "Shipal Zansatanit";
      npc.recordID = "shipal zansatanit";
      REQUIRE( SplitNamesDarkElf::is_ashlander(npc) );
    }

    SECTION("'House elves' should not belong to ashlanders")
    {
      npc.FactionID = "Temple";
      npc.ClassID = "Merchant";
      REQUIRE_FALSE( SplitNamesDarkElf::is_ashlander(npc) );
    }
  }

  SECTION("generate_ashlanders")
  {
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

    NPCRecord galos;
    galos.recordID = "test_galos";
    galos.Name = "Galos Farethi";
    galos.RaceID = "Dark Elf";
    galos.NPC_Flag = 0;
    REQUIRE_FALSE( galos.isFemale() );

    // mismatched id
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
    NPCs::get().addRecord(galos);
    NPCs::get().addRecord(foo);

    SECTION("generate female names")
    {
      SplitNamesDarkElf generator;
      REQUIRE( generator.generate_ashlanders(2, true).size() == 2 );
      REQUIRE( generator.generate_ashlanders(3, true).size() == 3 );
      const auto names = generator.generate_ashlanders(60, true);
      REQUIRE( names.size() == 60 );

      // Name "Somebody" should not be present due to different race id.
      REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Somebody Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Somebody Zansatanit") == names.end() );
      // Name "Alenus" should not be present due to not being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Farethi") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Zansatanit") == names.end() );
      // Name "Galos" should not be present due to not being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Galos Farethi") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Galos Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Galos Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Galos Zansatanit") == names.end() );
      // Last name "Farethi" should not be present due to not being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Farethi") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Manirai Farethi") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Farethi") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Kaushad Farethi") == names.end() );
      // Last name "Vendu" should not be present due to not being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Manirai Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Kaushad Vendu") == names.end() );
      // Names "Benudni" should be present with either "Ilurnubishpal" and "Zansatanit" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Ilurnubishpal") != names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Zansatanit") != names.end() );
      // Names "Manirai" should be present with either "Ilurnubishpal" and "Zansatanit" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Manirai Ilurnubishpal") != names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Manirai Zansatanit") != names.end() );
      // Names "Shipal" should not be present, neither with "Ilurnubishpal" nor "Zansatanit" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Zansatanit") == names.end() );
      // Names "Kaushad" should not be present, neither with "Ilurnubishpal" nor "Zansatanit" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Kaushad Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Kaushad Zansatanit") == names.end() );
    }

    SECTION("generate male names")
    {
      SplitNamesDarkElf generator;
      REQUIRE( generator.generate_ashlanders(2, false).size() == 2 );
      REQUIRE( generator.generate_ashlanders(3, false).size() == 3 );
      const auto names = generator.generate_ashlanders(60, false);
      REQUIRE( names.size() == 60 );

      // Name "Somebody" should not be present due to different race id.
      REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Somebody Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Somebody Zansatanit") == names.end() );
      // Name "Alenus" should not be present due to not being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Farethi") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Zansatanit") == names.end() );
      // Name "Galos" should not be present due to not being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Galos Farethi") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Galos Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Galos Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Galos Zansatanit") == names.end() );
      // Last name "Farethi" should not be present due to not being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Farethi") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Manirai Farethi") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Farethi") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Kaushad Farethi") == names.end() );
      // Last name "Vendu" should not be present due to not being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Manirai Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Kaushad Vendu") == names.end() );
      // Names "Benudni" should not be present, neither with "Ilurnubishpal" nor "Zansatanit" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Zansatanit") == names.end() );
      // Names "Manirai" should not be present, neither with "Ilurnubishpal" nor "Zansatanit" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Manirai Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Manirai Zansatanit") == names.end() );
      // Names "Shipal" should be present with either "Ilurnubishpal" and "Zansatanit" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Ilurnubishpal") != names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Zansatanit") != names.end() );
      // Names "Kaushad" should be present with either "Ilurnubishpal" and "Zansatanit" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Kaushad Ilurnubishpal") != names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Kaushad Zansatanit") != names.end() );
    }
  }

  SECTION("generate_non_ashlanders")
  {
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

    // ashlanders
    NPCRecord benudni;
    benudni.recordID = "test_benudni";
    benudni.Name = "Benudni Ilurnubishpal";
    benudni.RaceID = "Dark Elf";
    benudni.ClassID = "Mabrigash";
    benudni.NPC_Flag |= 0x00000001;
    REQUIRE( benudni.isFemale() );

    NPCRecord shipal;
    shipal.recordID = "shipal zansatanit";
    shipal.Name = "Shipal Zansatanit";
    shipal.RaceID = "Dark Elf";
    shipal.NPC_Flag = 0;
    REQUIRE_FALSE( shipal.isFemale() );

    // mismatched id
    NPCRecord foo;
    foo.recordID = "test_foo";
    foo.Name = "Somebody Else";
    foo.RaceID = "DifferentId";

    NPCs::get().clear();
    NPCs::get().addRecord(alenus);
    NPCs::get().addRecord(dratha);
    NPCs::get().addRecord(galos);
    NPCs::get().addRecord(aryon);
    NPCs::get().addRecord(benudni);
    NPCs::get().addRecord(shipal);
    NPCs::get().addRecord(foo);

    SECTION("generate female names")
    {
      SplitNamesDarkElf generator;
      REQUIRE( generator.generate_non_ashlanders(2, true).size() == 2 );
      REQUIRE( generator.generate_non_ashlanders(3, true).size() == 3 );
      const auto names = generator.generate_non_ashlanders(60, true);
      REQUIRE( names.size() == 60 );

      // Name "Somebody" should not be present due to different race id.
      REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Somebody Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Somebody Farethi") == names.end() );
      // Name "Benudni" should not be present due to being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Farethi") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Zansatanit") == names.end() );
      // Name "Shipal" should not be present due to being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Farethi") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Zansatanit") == names.end() );
      // Last name "Ilurnubishpal" should not be present due to being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Dratha Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Galos Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Aryon Ilurnubishpal") == names.end() );
      // Last name "Zansatanit" should not be present due to being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Zansatanit") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Dratha Zansatanit") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Galos Zansatanit") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Aryon Zansatanit") == names.end() );
      // Names "Alenus" should be present with either "Vendu" and "Farethi" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Vendu") != names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Farethi") != names.end() );
      // Names "Dratha" should be present with either "Vendu" and "Farethi" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Dratha Vendu") != names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Dratha Farethi") != names.end() );
      // Names "Galos" should not be present, neither with "Vendu" nor "Farethi" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Galos Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Galos Farethi") == names.end() );
      // Names "Aryon" should not be present, neither with "Vendu" nor "Farethi" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Aryon Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Aryon Farethi") == names.end() );
    }

    SECTION("generate male names")
    {
      SplitNamesDarkElf generator;
      REQUIRE( generator.generate_non_ashlanders(2, false).size() == 2 );
      REQUIRE( generator.generate_non_ashlanders(3, false).size() == 3 );
      const auto names = generator.generate_non_ashlanders(60, false);
      REQUIRE( names.size() == 60 );

      // Name "Somebody" should not be present due to different race id.
      REQUIRE( std::find(names.begin(), names.end(), "Somebody Else") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Somebody Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Somebody Farethi") == names.end() );
      // Name "Benudni" should not be present due to being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Farethi") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Benudni Zansatanit") == names.end() );
      // Name "Shipal" should not be present due to being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Farethi") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Shipal Zansatanit") == names.end() );
      // Last name "Ilurnubishpal" should not be present due to being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Dratha Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Galos Ilurnubishpal") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Aryon Ilurnubishpal") == names.end() );
      // Last name "Zansatanit" should not be present due to being ashlander.
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Zansatanit") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Dratha Zansatanit") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Galos Zansatanit") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Aryon Zansatanit") == names.end() );
      // Names "Alenus" should not be present, neither with "Vendu" nor "Farethi" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Alenus Farethi") == names.end() );
      // Names "Dratha" should not be present, neither with "Vendu" nor "Farethi" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Dratha Vendu") == names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Dratha Farethi") == names.end() );
      // Names "Galos" should be present with either "Vendu" and "Farethi" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Galos Vendu") != names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Galos Farethi") != names.end() );
      // Names "Aryon" should be present with either "Vendu" and "Farethi" as last name.
      REQUIRE( std::find(names.begin(), names.end(), "Aryon Vendu") != names.end() );
      REQUIRE( std::find(names.begin(), names.end(), "Aryon Farethi") != names.end() );
    }
  }
}
