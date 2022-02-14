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
