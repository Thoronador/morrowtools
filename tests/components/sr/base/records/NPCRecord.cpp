/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021  Thoronador

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

#include <catch.hpp>
#include <sstream>
#include <string_view>
#include "../../../../../sr/base/records/NPCRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("NPCRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    NPCRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.unknownVMAD.isPresent() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    for (unsigned int i = 0; i < 24; ++i)
    {
      REQUIRE( record.unknownACBS[i] == 0 );
    }
    REQUIRE( record.factions.empty() );
    REQUIRE( record.deathItemFormID == 0 );
    REQUIRE( record.voiceTypeFormID == 0 );
    REQUIRE( record.templateActorBaseFormID == 0 );
    REQUIRE( record.raceFormID == 0 );
    REQUIRE_FALSE( record.hasDEST );
    REQUIRE( record.unknownDEST == 0 );
    REQUIRE( record.skinFormID == 0 );
    REQUIRE( record.farAwayModelSkinFormID == 0 );
    REQUIRE_FALSE( record.hasATKR );
    REQUIRE( record.unknownATKR == 0 );
    REQUIRE_FALSE( record.unknownATKD.isPresent() );
    REQUIRE( record.unknownATKE.empty() );
    REQUIRE( record.spellFormIDs.empty() );
    REQUIRE( record.perkList.empty() );
    REQUIRE( record.items.empty() );
    REQUIRE( record.spectatorOverridePackageListFormID == 0 );
    REQUIRE( record.combatOverridePackageListFormID == 0 );
    for (unsigned int i = 0; i < 20; ++i)
    {
      REQUIRE( record.unknownAIDT[i] == 0 );
    }
    REQUIRE( record.unknownPKIDs.empty() );
    REQUIRE( record.keywordArray.empty() );
    REQUIRE( record.classFormID == 0 );
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE_FALSE( record.hasSHRT );
    REQUIRE( record.unknownSHRT == 0 );
    for (unsigned int i = 0; i < 52; ++i)
    {
      REQUIRE( record.unknownDNAM[i] == 0 );
    }
    REQUIRE( record.unknownPNAMs.empty() );
    REQUIRE( record.hairColorFormID == 0 );
    REQUIRE( record.giftFilterFormID == 0 );
    REQUIRE( record.combatStyleFormID == 0 );
    REQUIRE( record.unknownNAM5 == 0 );
    REQUIRE( record.unknownNAM6 == 0 );
    REQUIRE( record.unknownNAM7 == 0 );
    REQUIRE( record.unknownNAM8 == 0 );
    REQUIRE( record.defaultOutfitFormID == 0 );
    REQUIRE( record.sleepOutfitFormID == 0 );
    REQUIRE( record.crimeFactionFormID == 0 );
    REQUIRE( record.soundTemplateFormID == 0 );
    REQUIRE( record.unknownCSDXs.empty() );
    REQUIRE( record.defaultPackageListFormID == 0 );
    REQUIRE( record.faceComplexionFormID == 0 );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownQNAM[i] == 0 );
    }
    REQUIRE_FALSE( record.unknownNAM9.isPresent() );
    REQUIRE_FALSE( record.unknownNAMA.isPresent() );
    REQUIRE( record.unknownTINXs.empty() );
  }

  SECTION("equals")
  {
    NPCRecord a;
    NPCRecord b;

    SECTION("equal")
    {
      a.editorID = "foo";
      b.editorID = "foo";

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("editorID mismatch")
      {
        a.editorID = "foo";
        b.editorID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("VMAD mismatch")
      {
        a.unknownVMAD.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownVMAD.setPresence(false);
        b.unknownVMAD.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("OBND mismatch")
      {
        a.unknownOBND[11] = 12;
        b.unknownOBND[11] = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ACBS mismatch")
      {
        a.unknownACBS[23] = 12;
        b.unknownACBS[23] = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      // TODO: Add sections for other data members.

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    NPCRecord record;

    REQUIRE( record.getRecordType() == cNPC_ );
  }
}
