/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021, 2022  Dirk Stolle

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

#include "../../locate_catch.hpp"
#include <sstream>
#include <string_view>
#include "../../../../lib/mw/records/PreNPCRecord.hpp"
#include "../../../../lib/mw/records/NPCRecord.hpp"

TEST_CASE("MWTP::PreNPCRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  // Note: PreNPCRecord is an abstract class, so we use NPCRecord instead.

  SECTION("constructor")
  {
    NPCRecord record;

    REQUIRE( record.Items.empty() );
    REQUIRE( record.NPC_Spells.empty() );
    REQUIRE_FALSE( record.AIData.has_value() );
    REQUIRE( record.AIPackages.empty() );
    REQUIRE( record.Destinations.empty() );
  }

  SECTION("hasEqualAIPackages")
  {
    NPCRecord a;
    NPCRecord b;

    SECTION("equal")
    {
      SECTION("empty")
      {
        REQUIRE( a.hasEqualAIPackages(b) );
        REQUIRE( b.hasEqualAIPackages(a) );
      }

      SECTION("nullptr")
      {
        a.AIPackages.push_back(nullptr);
        b.AIPackages.push_back(nullptr);

        REQUIRE( a.hasEqualAIPackages(b) );
        REQUIRE( b.hasEqualAIPackages(a) );
      }

      SECTION("Activate package")
      {
        auto pkgA = new NPC_AIActivate();
        pkgA->TargetID = "foo";
        pkgA->Reset = 1;
        a.AIPackages.push_back(pkgA);

        auto pkgB = new NPC_AIActivate();
        pkgB->TargetID = "foo";
        pkgB->Reset = 1;
        b.AIPackages.push_back(pkgB);

        REQUIRE( a.hasEqualAIPackages(b) );
        REQUIRE( b.hasEqualAIPackages(a) );
      }

      SECTION("Escort package")
      {
        auto pkgA = new NPC_AIEscort();
        pkgA->TargetID = "foo";
        pkgA->Reset = 1;
        a.AIPackages.push_back(pkgA);

        auto pkgB = new NPC_AIEscort();
        pkgB->TargetID = "foo";
        pkgB->Reset = 1;
        b.AIPackages.push_back(pkgB);

        REQUIRE( a.hasEqualAIPackages(b) );
        REQUIRE( b.hasEqualAIPackages(a) );
      }

      SECTION("Follow package")
      {
        auto pkgA = new NPC_AIFollow();
        pkgA->TargetID = "foo";
        pkgA->Reset = 1;
        a.AIPackages.push_back(pkgA);

        auto pkgB = new NPC_AIFollow();
        pkgB->TargetID = "foo";
        pkgB->Reset = 1;
        b.AIPackages.push_back(pkgB);

        REQUIRE( a.hasEqualAIPackages(b) );
        REQUIRE( b.hasEqualAIPackages(a) );
      }

      SECTION("Travel package")
      {
        auto pkgA = new NPC_AITravel();
        pkgA->X = 1.0f;
        pkgA->Reset = 1;
        a.AIPackages.push_back(pkgA);

        auto pkgB = new NPC_AITravel();
        pkgB->X = 1.0f;
        pkgB->Reset = 1;
        b.AIPackages.push_back(pkgB);

        REQUIRE( a.hasEqualAIPackages(b) );
        REQUIRE( b.hasEqualAIPackages(a) );
      }

      SECTION("Wander package")
      {
        auto pkgA = new NPC_AIWander();
        pkgA->Distance = 10;
        a.AIPackages.push_back(pkgA);

        auto pkgB = new NPC_AIWander();
        pkgB->Distance = 10;
        b.AIPackages.push_back(pkgB);

        REQUIRE( a.hasEqualAIPackages(b) );
        REQUIRE( b.hasEqualAIPackages(a) );
      }
    }

    SECTION("unequal")
    {
      SECTION("size mismatch")
      {
        a.AIPackages.push_back(nullptr);

        REQUIRE_FALSE( a.hasEqualAIPackages(b) );
        REQUIRE_FALSE( b.hasEqualAIPackages(a) );
      }

      SECTION("package vs. nullptr")
      {
        auto pkgA = new NPC_AIWander();
        pkgA->Distance = 10;
        a.AIPackages.push_back(pkgA);

        b.AIPackages.push_back(nullptr);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Activate mismatch")
      {
        auto pkgA = new NPC_AIActivate();
        pkgA->TargetID = "foo";
        pkgA->Reset = 1;
        a.AIPackages.push_back(pkgA);

        auto pkgB = new NPC_AIActivate();
        pkgB->TargetID = "bar";
        pkgB->Reset = 0;
        b.AIPackages.push_back(pkgB);

        REQUIRE_FALSE( a.hasEqualAIPackages(b) );
        REQUIRE_FALSE( b.hasEqualAIPackages(a) );
      }

      SECTION("Escort mismatch")
      {
        auto pkgA = new NPC_AIEscort();
        pkgA->TargetID = "foo";
        pkgA->Reset = 1;
        a.AIPackages.push_back(pkgA);

        auto pkgB = new NPC_AIEscort();
        pkgB->TargetID = "bar";
        pkgB->Reset = 0;
        b.AIPackages.push_back(pkgB);

        REQUIRE_FALSE( a.hasEqualAIPackages(b) );
        REQUIRE_FALSE( b.hasEqualAIPackages(a) );
      }

      SECTION("Follow mismatch")
      {
        auto pkgA = new NPC_AIFollow();
        pkgA->TargetID = "bar";
        pkgA->Reset = 0;
        a.AIPackages.push_back(pkgA);

        auto pkgB = new NPC_AIFollow();
        pkgB->TargetID = "foo";
        pkgB->Reset = 1;
        b.AIPackages.push_back(pkgB);

        REQUIRE_FALSE( a.hasEqualAIPackages(b) );
        REQUIRE_FALSE( b.hasEqualAIPackages(a) );
      }

      SECTION("Travel mismatch")
      {
        auto pkgA = new NPC_AITravel();
        pkgA->X = 1.0f;
        pkgA->Reset = 1;
        a.AIPackages.push_back(pkgA);

        auto pkgB = new NPC_AITravel();
        pkgB->X = 2.5f;
        pkgB->Reset = 0;
        b.AIPackages.push_back(pkgB);

        REQUIRE_FALSE( a.hasEqualAIPackages(b) );
        REQUIRE_FALSE( b.hasEqualAIPackages(a) );
      }

      SECTION("Wander package")
      {
        auto pkgA = new NPC_AIWander();
        pkgA->Distance = 100;
        a.AIPackages.push_back(pkgA);

        auto pkgB = new NPC_AIWander();
        pkgB->Distance = 1;
        b.AIPackages.push_back(pkgB);

        REQUIRE_FALSE( a.hasEqualAIPackages(b) );
        REQUIRE_FALSE( b.hasEqualAIPackages(a) );
      }
    }
  }

  SECTION("fun with flags")
  {
    NPCRecord record;

    SECTION("doesRepair")
    {
      // AIData is not present.
      REQUIRE_FALSE( record.doesRepair() );

      // AIData is present.
      record.AIData = NPC_AIData();
      record.AIData.value().Flags = 131072;
      REQUIRE( record.doesRepair() );

      record.AIData.value().Flags = 1;
      REQUIRE_FALSE( record.doesRepair() );

      record.AIData.value().Flags = 1 + 131072;
      REQUIRE( record.doesRepair() );
    }

    SECTION("isEnchanter")
    {
      // AIData is not present.
      REQUIRE_FALSE( record.isEnchanter() );

      // AIData is present.
      record.AIData = NPC_AIData();
      record.AIData.value().Flags = 65536;
      REQUIRE( record.isEnchanter() );

      record.AIData.value().Flags = 1;
      REQUIRE_FALSE( record.isEnchanter() );

      record.AIData.value().Flags = 1 + 65536;
      REQUIRE( record.isEnchanter() );
    }

    SECTION("isSpellmaker")
    {
      // AIData is not present.
      REQUIRE_FALSE( record.isSpellmaker() );

      // AIData is present.
      record.AIData = NPC_AIData();
      record.AIData.value().Flags = 32768;
      REQUIRE( record.isSpellmaker() );

      record.AIData.value().Flags = 3;
      REQUIRE_FALSE( record.isSpellmaker() );

      record.AIData.value().Flags = 3 + 32768;
      REQUIRE( record.isSpellmaker() );
    }

    SECTION("isTrainer")
    {
      // AIData is not present.
      REQUIRE_FALSE( record.isTrainer() );

      // AIData is present.
      record.AIData = NPC_AIData();
      record.AIData.value().Flags = 16384;
      REQUIRE( record.isTrainer() );

      record.AIData.value().Flags = 1;
      REQUIRE_FALSE( record.isTrainer() );

      record.AIData.value().Flags = 1 + 16384;
      REQUIRE( record.isTrainer() );
    }
  }
}
