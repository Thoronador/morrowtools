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
#include <limits>
#include <sstream>
#include <string_view>
#include "../../../../../mw/base/records/CreatureRecord.hpp"

TEST_CASE("MWTP::CreatureRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    CreatureRecord record;

    // stuff from PreNPCRecord
    REQUIRE( record.Items.empty() );
    REQUIRE( record.NPC_Spells.empty() );
    REQUIRE_FALSE( record.AIData.has_value() );
    REQUIRE( record.AIPackages.empty() );
    REQUIRE( record.Destinations.empty() );
    // stuff from CreatureRecord
    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.SoundGenCreature.empty() );
    REQUIRE( record.CreatureType == 0 );
    REQUIRE( record.Level == 0 );
    REQUIRE( record.Strength == 0 );
    REQUIRE( record.Intelligence == 0 );
    REQUIRE( record.Willpower == 0 );
    REQUIRE( record.Agility == 0 );
    REQUIRE( record.Speed == 0 );
    REQUIRE( record.Endurance == 0 );
    REQUIRE( record.Personality == 0 );
    REQUIRE( record.Luck == 0 );
    REQUIRE( record.Health == 0 );
    REQUIRE( record.SpellPoints == 0 );
    REQUIRE( record.Fatigue == 0 );
    REQUIRE( record.Soul == 0 );
    REQUIRE( record.Combat == 0 );
    REQUIRE( record.Magic == 0 );
    REQUIRE( record.Stealth == 0 );
    REQUIRE( record.AttackMin1 == 0 );
    REQUIRE( record.AttackMax1 == 0 );
    REQUIRE( record.AttackMin2 == 0 );
    REQUIRE( record.AttackMax2 == 0 );
    REQUIRE( record.AttackMin3 == 0 );
    REQUIRE( record.AttackMax3 == 0 );
    REQUIRE( record.Gold == 0 );
    REQUIRE( record.CreatureFlag == 0 );
    REQUIRE( record.ScriptID.empty() );
    REQUIRE( record.Scale == 1.0f );
  }

  SECTION("equals")
  {
    CreatureRecord a;
    CreatureRecord b;

    SECTION("equal")
    {
      a.recordID = "Creature_One";
      a.Name = "foobar";

      b.recordID = "Creature_One";
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
        a.AIData = NPC_AIData();
        b.AIData.reset();

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

      // CreatureRecord's stuff
      SECTION("recordID mismatch")
      {
        a.recordID = "foo";
        b.recordID = "bar";

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

      SECTION("Name mismatch")
      {
        a.Name = "One";
        b.Name = "Another";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SoundGenCreature mismatch")
      {
        a.SoundGenCreature = "foo";
        b.SoundGenCreature = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CreatureType mismatch")
      {
        a.CreatureType = 15;
        b.CreatureType = 25;

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

      SECTION("Combat mismatch")
      {
        a.Combat = 15;
        b.Combat = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Magic mismatch")
      {
        a.Magic = 15;
        b.Magic = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Stealth mismatch")
      {
        a.Stealth = 15;
        b.Stealth = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AttackMin1 mismatch")
      {
        a.AttackMin1 = 60;
        b.AttackMin1 = 90;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AttackMax1 mismatch")
      {
        a.AttackMax1 = 60;
        b.AttackMax1 = 90;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AttackMin2 mismatch")
      {
        a.AttackMin2 = 60;
        b.AttackMin2 = 90;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AttackMax2 mismatch")
      {
        a.AttackMax2 = 60;
        b.AttackMax2 = 90;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AttackMin3 mismatch")
      {
        a.AttackMin3 = 60;
        b.AttackMin3 = 90;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AttackMax3 mismatch")
      {
        a.AttackMax3 = 60;
        b.AttackMax3 = 90;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Gold mismatch")
      {
        a.Gold = 60;
        b.Gold = 90;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CreatureFlag mismatch")
      {
        a.CreatureFlag = 60;
        b.CreatureFlag = 90;

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


      SECTION("Scale mismatch")
      {
        a.Scale = 1.0f;
        b.Scale = 2.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    SECTION("default: load record")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CreatureRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      // stuff from CreatureRecord
      REQUIRE( record.recordID == "clannfear_summon" );
      REQUIRE( record.ModelPath == "r\\Clannfear.NIF" );
      REQUIRE( record.Name == "Clannbann" );
      REQUIRE( record.SoundGenCreature.empty() );
      REQUIRE( record.CreatureType == 1 );
      REQUIRE( record.Level == 7 );
      REQUIRE( record.Strength == 110 );
      REQUIRE( record.Intelligence == 20 );
      REQUIRE( record.Willpower == 70 );
      REQUIRE( record.Agility == 80 );
      REQUIRE( record.Speed == 14 );
      REQUIRE( record.Endurance == 255 );
      REQUIRE( record.Personality == 60 );
      REQUIRE( record.Luck == 20 );
      REQUIRE( record.Health == 113 );
      REQUIRE( record.SpellPoints == 100 );
      REQUIRE( record.Fatigue == 500 );
      REQUIRE( record.Soul == 100 );
      REQUIRE( record.Combat == 60 );
      REQUIRE( record.Magic == 90 );
      REQUIRE( record.Stealth == 100 );
      REQUIRE( record.AttackMin1 == 6 );
      REQUIRE( record.AttackMax1 == 18 );
      REQUIRE( record.AttackMin2 == 6 );
      REQUIRE( record.AttackMax2 == 18 );
      REQUIRE( record.AttackMin3 == 6 );
      REQUIRE( record.AttackMax3 == 18 );
      REQUIRE( record.Gold == 0 );
      REQUIRE( record.CreatureFlag == 0x00000048 );
      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.Scale == 1.0f );
      // stuff from PreNPCRecord
      REQUIRE( record.Items.empty() );
      REQUIRE( record.NPC_Spells.empty() );
      REQUIRE( record.AIData.has_value() );
      REQUIRE( record.AIData.value().Hello == 0 );
      REQUIRE( record.AIData.value().Unknown1 == 0 );
      REQUIRE( record.AIData.value().Fight == 50 );
      REQUIRE( record.AIData.value().Flee == 30 );
      REQUIRE( record.AIData.value().Alarm == 0 );
      REQUIRE( record.AIData.value().Unknown2 == 0 );
      REQUIRE( record.AIData.value().Unknown3 == 0 );
      REQUIRE( record.AIData.value().Unknown4 == 0 );
      REQUIRE( record.AIData.value().Flags == 0x00A00000 );
      REQUIRE( record.AIPackages.size() == 1 );
      REQUIRE( record.AIPackages[0]->getPackageType() == PackageType::ptWander );
      const auto package_one = static_cast<NPC_AIWander*>(record.AIPackages[0]);
      REQUIRE( package_one->Distance == 1000 );
      REQUIRE( package_one->Duration == 5 );
      REQUIRE( package_one->Time == 0 );
      REQUIRE( package_one->Idle[0] == 50 );
      REQUIRE( package_one->Idle[1] == 20 );
      REQUIRE( package_one->Idle[2] == 30 );
      REQUIRE( package_one->Idle[3] == 0 );
      REQUIRE( package_one->Idle[4] == 0 );
      REQUIRE( package_one->Idle[5] == 0 );
      REQUIRE( package_one->Idle[6] == 0 );
      REQUIRE( package_one->Idle[7] == 0 );
      REQUIRE( package_one->Reset == 1 );
      REQUIRE( record.Destinations.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "CREA\xF7\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0FAIL\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\x01\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\xF1\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0FAIL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\x01\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before MODL can be read completely")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Cl"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FAIL\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\x01\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\xEA\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple AIDTs")
    {
      const auto data = "CREA\xF5\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of AIDT is not twelve")
    {
      {
        const auto data = "CREA\xE0\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0B\0\0\0\0\0\x32\x1E\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CREA, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CreatureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "CREA\xE2\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0D\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CREA, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CreatureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before AIDT can be read")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before AI_A can be read")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_A\x21\0\0\0\xE8"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before AI_E can be read")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_E\x30\0\0\0\xE8"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before AI_F can be read")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_F\x30\0\0\0\xE8"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before AI_T can be read")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_T\x10\0\0\0\xE8"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before AI_W can be read")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FLAG")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FAIL\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FLAG is not four")
    {
      {
        const auto data = "CREA\xE0\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x03\0\0\0H\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CREA, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CreatureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "CREA\xE2\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x05\0\0\0H\0\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CREA, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CreatureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before FLAG can be read")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NPDT")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0FAIL`\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NPDT is not 96")
    {
      {
        const auto data = "CREA\xE0\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT\x5F\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CREA, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CreatureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "CREA\xE2\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT\x61\0\0\0\x01\0\0\0\x07\0\0\0n\0\0\0\x14\0\0\0F\0\0\0P\0\0\0\x0E\0\0\0\xFF\0\0\0<\0\0\0\x14\0\0\0q\0\0\0d\0\0\0\xF4\x01\0\0d\0\0\0<\0\0\0Z\0\0\0d\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\x06\0\0\0\x12\0\0\0\0\0\0\0\0FLAG\x04\0\0\0H\0\0\0AIDT\x0C\0\0\0\0\0\x32\x1E\0\0\0\0\0\0\xA0\0AI_W\x0E\0\0\0\xE8\x03\x05\0\0\x32\x14\x1E\0\0\0\0\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CREA, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CreatureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before NPDT can be read")
    {
      const auto data = "CREA\xE1\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0clannfear_summon\0MODL\x10\0\0\0r\\Clannfear.NIF\0FNAM\x0A\0\0\0Clannbann\0NPDT`\0\0\0\x01\0\0\0\x07\0\0\0n"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CREA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CreatureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
