/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2022  Dirk Stolle

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
#include "../../../../../mw/base/records/NPCRecord.hpp"

TEST_CASE("MWTP::NPCRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    NPCRecord record;

    // stuff from PreNPCRecord
    REQUIRE( record.Items.empty() );
    REQUIRE( record.NPC_Spells.empty() );
    REQUIRE_FALSE( record.AIData.isPresent );
    REQUIRE( record.AIPackages.empty() );
    REQUIRE( record.Destinations.empty() );
    // stuff from NPCRecord
    REQUIRE( record.recordID.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.RaceID.empty() );
    REQUIRE( record.FactionID.empty() );
    REQUIRE( record.HeadModel.empty() );
    REQUIRE( record.ClassID.empty() );
    REQUIRE( record.HairModel.empty() );
    REQUIRE( record.ScriptID.empty() );
    REQUIRE( record.Level == 0 );
    REQUIRE( record.Strength == 0 );
    REQUIRE( record.Intelligence == 0 );
    REQUIRE( record.Willpower == 0 );
    REQUIRE( record.Agility == 0 );
    REQUIRE( record.Speed == 0 );
    REQUIRE( record.Endurance == 0 );
    REQUIRE( record.Personality == 0 );
    REQUIRE( record.Luck == 0 );
    for (int i = 0; i < 27; ++i)
    {
      REQUIRE( record.Skills[i] == 0 );
    }
    REQUIRE( record.Reputation == 0 );
    REQUIRE( record.Health == 0 );
    REQUIRE( record.SpellPoints == 0 );
    REQUIRE( record.Fatigue == 0 );
    REQUIRE( record.Disposition == 0 );
    REQUIRE( record.Data_FactionID == 0 );
    REQUIRE( record.Rank == 0 );
    REQUIRE( record.Unknown1 == 0 );
    REQUIRE( record.Gold == 0 );
    REQUIRE( record.NPCDataType == NPDT_Type::ndtNone );
    REQUIRE( record.NPC_Flag == 0 );
  }

  SECTION("equals")
  {
    NPCRecord a;
    NPCRecord b;

    SECTION("equal")
    {
      a.recordID = "NPC_One";
      a.Name = "foobar";

      b.recordID = "NPC_One";
      b.Name = "foobar";

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      // PreNPCRecord's stuff
      SECTION("Items mismatch")
      {
        a.Items.clear();
        b.Items.push_back(ItemRecord());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Spells mismatch")
      {
        a.NPC_Spells.clear();
        b.NPC_Spells.push_back("SpellIdOne");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AIData mismatch")
      {
        a.AIData.isPresent = true;
        b.AIData.isPresent = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AIPackages mismatch")
      {
        auto pkgA = new NPC_AIActivate();
        pkgA->TargetID = "foo";
        pkgA->Reset = 1;
        a.AIPackages.push_back(pkgA);

        auto pkgB = new NPC_AIActivate();
        pkgB->TargetID = "bar";
        pkgB->Reset = 0;
        b.AIPackages.push_back(pkgB);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Destinations mismatch")
      {
        a.Destinations.clear();
        b.Destinations.push_back(TravelDestination());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      // NPCRecord's stuff
      SECTION("recordID mismatch")
      {
        a.recordID = "foo";
        b.recordID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Name mismatch")
      {
        a.Name = "One";
        b.Name = "Another";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ModelPath mismatch")
      {
        a.ModelPath = "foo.nif";
        b.ModelPath = "bar.nif";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("RaceID mismatch")
      {
        a.RaceID = "foo";
        b.RaceID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("FactionID mismatch")
      {
        a.FactionID = "foo";
        b.FactionID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("HeadModel mismatch")
      {
        a.HeadModel = "head.nif";
        b.HeadModel = "box.nif";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ClassID mismatch")
      {
        a.ClassID = "foo";
        b.ClassID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("HairModel mismatch")
      {
        a.HairModel = "hair.nif";
        b.HairModel = "whatever.nif";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ScriptID mismatch")
      {
        a.ScriptID = "foo";
        b.ScriptID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Level mismatch")
      {
        a.Level = 15;
        b.Level = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Strength mismatch")
      {
        a.Strength = 15;
        b.Strength = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Intelligence mismatch")
      {
        a.Intelligence = 15;
        b.Intelligence = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Willpower mismatch")
      {
        a.Willpower = 15;
        b.Willpower = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Agility mismatch")
      {
        a.Agility = 15;
        b.Agility = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Speed mismatch")
      {
        a.Speed = 15;
        b.Speed = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Endurance mismatch")
      {
        a.Endurance = 15;
        b.Endurance = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Personality mismatch")
      {
        a.Personality = 15;
        b.Personality = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Luck mismatch")
      {
        a.Luck = 15;
        b.Luck = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Skills mismatch")
      {
        a.Skills[15] = 15;
        b.Skills[15] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Reputation mismatch")
      {
        a.Reputation = 60;
        b.Reputation = 90;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Health mismatch")
      {
        a.Health = 15;
        b.Health = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SpellPoints mismatch")
      {
        a.SpellPoints = 15;
        b.SpellPoints = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Fatigue mismatch")
      {
        a.Fatigue = 15;
        b.Fatigue = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Disposition mismatch")
      {
        a.Disposition = 0;
        b.Disposition = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Data_FactionID mismatch")
      {
        a.Data_FactionID = 15;
        b.Data_FactionID = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Rank mismatch")
      {
        a.Rank = 15;
        b.Rank = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Unknown1 mismatch")
      {
        a.Unknown1 = 15;
        b.Unknown1 = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Gold mismatch")
      {
        a.Gold = 15;
        b.Gold = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NPCDataType mismatch")
      {
        a.NPCDataType = ndt12Bytes;
        b.NPCDataType = ndt52Bytes;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NPC_Flag mismatch")
      {
        a.NPC_Flag = 0x000000FF;
        b.NPC_Flag = 0x000000AB;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("fun with flags")
  {
    NPCRecord record;

    SECTION("isFemale")
    {
      record.NPC_Flag = 0;
      REQUIRE_FALSE( record.isFemale() );

      record.NPC_Flag = 0x00000001;
      REQUIRE( record.isFemale() );

      record.NPC_Flag = 0x00000002;
      REQUIRE_FALSE( record.isFemale() );

      record.NPC_Flag = 0x0000FFFF;
      REQUIRE( record.isFemale() );
    }

    SECTION("isEssential")
    {
      record.NPC_Flag = 0;
      REQUIRE_FALSE( record.isEssential() );

      record.NPC_Flag = 0x00000002;
      REQUIRE( record.isEssential() );

      record.NPC_Flag = 0x00000008;
      REQUIRE_FALSE( record.isEssential() );

      record.NPC_Flag = 0x0000FFFF;
      REQUIRE( record.isEssential() );
    }

    SECTION("doesRespawn")
    {
      record.NPC_Flag = 0;
      REQUIRE_FALSE( record.doesRespawn() );

      record.NPC_Flag = 0x00000004;
      REQUIRE( record.doesRespawn() );

      record.NPC_Flag = 0x00000002;
      REQUIRE_FALSE( record.doesRespawn() );

      record.NPC_Flag = 0x0000FFFF;
      REQUIRE( record.doesRespawn() );
    }

    SECTION("hasAutoCalcStats")
    {
      record.NPC_Flag = 0;
      REQUIRE_FALSE( record.hasAutoCalcStats() );

      record.NPC_Flag = 0x00000010;
      REQUIRE( record.hasAutoCalcStats() );

      record.NPC_Flag = 0x00000002;
      REQUIRE_FALSE( record.hasAutoCalcStats() );

      record.NPC_Flag = 0x0000FFFF;
      REQUIRE( record.hasAutoCalcStats() );
    }

    SECTION("hasWhiteBloodTex")
    {
      record.NPC_Flag = 0;
      REQUIRE_FALSE( record.hasWhiteBloodTex() );

      record.NPC_Flag = 0x00000400;
      REQUIRE( record.hasWhiteBloodTex() );

      record.NPC_Flag = 0x00000002;
      REQUIRE_FALSE( record.hasWhiteBloodTex() );

      record.NPC_Flag = 0x0000FFFF;
      REQUIRE( record.hasWhiteBloodTex() );
    }

    SECTION("hasGoldBloodTex")
    {
      record.NPC_Flag = 0;
      REQUIRE_FALSE( record.hasGoldBloodTex() );

      record.NPC_Flag = 0x00000800;
      REQUIRE( record.hasGoldBloodTex() );

      record.NPC_Flag = 0x00000002;
      REQUIRE_FALSE( record.hasGoldBloodTex() );

      record.NPC_Flag = 0x0000FFFF;
      REQUIRE( record.hasGoldBloodTex() );
    }
  }
}
