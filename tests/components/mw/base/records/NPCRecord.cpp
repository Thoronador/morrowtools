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
    REQUIRE( record.Unknown2 == 0 );
    REQUIRE( record.Unknown3 == 0 );
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

      SECTION("Unknown2 mismatch")
      {
        a.Unknown2 = 15;
        b.Unknown2 = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Unknown3 mismatch")
      {
        a.Unknown3 = 15;
        b.Unknown3 = 25;

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

  SECTION("loadFromStream")
  {
    SECTION("default: load record with 12-byte NPDT")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      NPCRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      // stuff from NPCRecord
      REQUIRE( record.recordID == "listien bierles" );
      REQUIRE( record.Name == "Listien Bierles" );
      REQUIRE( record.ModelPath.empty() );
      REQUIRE( record.RaceID == "Breton" );
      REQUIRE( record.FactionID == "Mages Guild" );
      REQUIRE( record.HeadModel == "b_n_breton_m_head_02" );
      REQUIRE( record.ClassID == "Sorcerer" );
      REQUIRE( record.HairModel == "b_n_breton_m_hair_03" );
      REQUIRE( record.ScriptID == "nolore" );
      REQUIRE( record.Level == 2 );
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
      REQUIRE( record.Disposition == 50 );
      REQUIRE( record.Data_FactionID == 4 );
      REQUIRE( record.Rank == 1 );
      REQUIRE( record.Unknown1 == 0xD6 );
      REQUIRE( record.Gold == 0 );
      REQUIRE( record.NPCDataType == NPDT_Type::ndt12Bytes );
      REQUIRE( record.NPC_Flag == 0x00000018 );
      // stuff from PreNPCRecord
      REQUIRE( record.Items.empty() );
      REQUIRE( record.NPC_Spells.empty() );
      REQUIRE( record.AIData.isPresent );
      REQUIRE( record.AIData.Hello == 30 );
      REQUIRE( record.AIData.Unknown1 == 0 );
      REQUIRE( record.AIData.Fight == 30 );
      REQUIRE( record.AIData.Flee == 20 );
      REQUIRE( record.AIData.Alarm == 0 );
      REQUIRE( record.AIData.Unknown2 == 0xD6 );
      REQUIRE( record.AIData.Unknown3 == 0xE8 );
      REQUIRE( record.AIData.Unknown4 == 0 );
      REQUIRE( record.AIData.Flags == 0 );
      REQUIRE( record.AIPackages.size() == 1 );
      REQUIRE( record.AIPackages[0]->getPackageType() == PackageType::ptWander );
      const auto package_one = static_cast<NPC_AIWander*>(record.AIPackages[0]);
      REQUIRE( package_one->Distance == 0 );
      REQUIRE( package_one->Duration == 5 );
      REQUIRE( package_one->Time == 0 );
      REQUIRE( package_one->Idle[0] == 40 );
      REQUIRE( package_one->Idle[1] == 20 );
      REQUIRE( package_one->Idle[2] == 20 );
      REQUIRE( package_one->Idle[3] == 10 );
      REQUIRE( package_one->Idle[4] == 0 );
      REQUIRE( package_one->Idle[5] == 0 );
      REQUIRE( package_one->Idle[6] == 10 );
      REQUIRE( package_one->Idle[7] == 0 );
      REQUIRE( package_one->Reset == 1 );
      REQUIRE( record.Destinations.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with items")
    {
      const auto data = "NPC_\x31\x01\0\0\0\0\0\0\0\0\0\0NAME\x0C\0\0\0galthragoth\0FNAM\x0C\0\0\0Galthragoth\0RNAM\x09\0\0\0Wood Elf\0CNAM\x07\0\0\0Pauper\0ANAM\x01\0\0\0\0BNAM\x17\0\0\0B_N_Wood Elf_M_Head_06\0KNAM\x17\0\0\0b_n_wood elf_m_hair_03\0NPDT\x0C\0\0\0\x05\0\x32\0\0\x09m\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0NPCO$\0\0\0\x01\0\0\0common_shirt_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_pants_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x14\0\x0A\x32\0\x09m\0\0\0\0\0AI_W\x0E\0\0\0\0\x01\x05\0\0<\x14\x0A\x0A\0\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      NPCRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      // stuff from NPCRecord
      REQUIRE( record.recordID == "galthragoth" );
      REQUIRE( record.Name == "Galthragoth" );
      REQUIRE( record.ModelPath.empty() );
      REQUIRE( record.RaceID == "Wood Elf" );
      REQUIRE( record.FactionID.empty() );
      REQUIRE( record.HeadModel == "B_N_Wood Elf_M_Head_06" );
      REQUIRE( record.ClassID == "Pauper" );
      REQUIRE( record.HairModel == "b_n_wood elf_m_hair_03" );
      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.Level == 5 );
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
      REQUIRE( record.Disposition == 50 );
      REQUIRE( record.Data_FactionID == 0 );
      REQUIRE( record.Rank == 0 );
      REQUIRE( record.Unknown1 == 0x09 );
      REQUIRE( record.Unknown2 == 0x6D );
      REQUIRE( record.Unknown3 == 0x00 );
      REQUIRE( record.Gold == 0 );
      REQUIRE( record.NPCDataType == NPDT_Type::ndt12Bytes );
      REQUIRE( record.NPC_Flag == 0x00000018 );
      // stuff from PreNPCRecord
      REQUIRE( record.Items.size() == 2 );
      REQUIRE( record.Items[0].Count == 1 );
      REQUIRE( record.Items[0].Item == "common_shirt_02" );
      REQUIRE( record.Items[1].Count == 1 );
      REQUIRE( record.Items[1].Item == "common_pants_02" );
      REQUIRE( record.NPC_Spells.empty() );
      REQUIRE( record.AIData.isPresent );
      REQUIRE( record.AIData.Hello == 20 );
      REQUIRE( record.AIData.Unknown1 == 0 );
      REQUIRE( record.AIData.Fight == 10 );
      REQUIRE( record.AIData.Flee == 50 );
      REQUIRE( record.AIData.Alarm == 0 );
      REQUIRE( record.AIData.Unknown2 == 0x09 );
      REQUIRE( record.AIData.Unknown3 == 0x6D );
      REQUIRE( record.AIData.Unknown4 == 0 );
      REQUIRE( record.AIData.Flags == 0 );
      REQUIRE( record.AIPackages.size() == 1 );
      REQUIRE( record.AIPackages[0]->getPackageType() == PackageType::ptWander );
      const auto package_one = static_cast<NPC_AIWander*>(record.AIPackages[0]);
      // AI_W\x0E\0\0\0 \0\x01\x05\0\0<\x14\x0A\x0A\0\0\0\0\x01
      REQUIRE( package_one->Distance == 256 );
      REQUIRE( package_one->Duration == 5 );
      REQUIRE( package_one->Time == 0 );
      REQUIRE( package_one->Idle[0] == 60 );
      REQUIRE( package_one->Idle[1] == 20 );
      REQUIRE( package_one->Idle[2] == 10 );
      REQUIRE( package_one->Idle[3] == 10 );
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
      const auto data = "NPC_\xF7\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0FAIL\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\x01\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\xF7\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FAIL\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple FNAMs")
    {
      const auto data = "NPC_\x0F\x01\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\x01\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\xF0\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple AIDTs")
    {
      const auto data = "NPC_\x0B\x01\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of AIDT is not twelve")
    {
      {
        const auto data = "NPC_\xF6\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0B\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip NPC_, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0D\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip NPC_, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before AIDT can be read")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ANAMs")
    {
      const auto data = "NPC_\x0B\x01\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ANAM > 256")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\x01\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ANAM is beyond stream")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\xF7\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple BNAMs")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0BNAM\x15\0\0\0b_n_breton_m_head_02\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of BNAM > 256")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\x01\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of BNAM is beyond stream")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\xF5\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple CNAMs")
    {
      const auto data = "NPC_\x08\x01\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of CNAM > 256")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\x01\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of CNAM is beyond stream")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\xF7\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FLAG is not four")
    {
      {
        const auto data = "NPC_\xF6\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x03\0\0\0\x18\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip NPC_, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "NPC_\xF8\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x05\0\0\0\x18\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip NPC_, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before FLAG can be read")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple KNAMs")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0KNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of KNAM > 256")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\x01\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of KNAM is beyond stream")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\xF5\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NPCO is not 36")
    {
      {
        const auto data = "NPC_\x30\x01\0\0\0\0\0\0\0\0\0\0NAME\x0C\0\0\0galthragoth\0FNAM\x0C\0\0\0Galthragoth\0RNAM\x09\0\0\0Wood Elf\0CNAM\x07\0\0\0Pauper\0ANAM\x01\0\0\0\0BNAM\x17\0\0\0B_N_Wood Elf_M_Head_06\0KNAM\x17\0\0\0b_n_wood elf_m_hair_03\0NPDT\x0C\0\0\0\x05\0\x32\0\0\x09m\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0NPCO$\0\0\0\x01\0\0\0common_shirt_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO\x23\0\0\0\x01\0\0\0common_pants_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x14\0\x0A\x32\0\x09m\0\0\0\0\0AI_W\x0E\0\0\0\0\x01\x05\0\0<\x14\x0A\x0A\0\0\0\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip NPC_, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "NPC_\x31\x01\0\0\0\0\0\0\0\0\0\0NAME\x0C\0\0\0galthragoth\0FNAM\x0C\0\0\0Galthragoth\0RNAM\x09\0\0\0Wood Elf\0CNAM\x07\0\0\0Pauper\0ANAM\x01\0\0\0\0BNAM\x17\0\0\0B_N_Wood Elf_M_Head_06\0KNAM\x17\0\0\0b_n_wood elf_m_hair_03\0NPDT\x0C\0\0\0\x05\0\x32\0\0\x09m\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0NPCO$\0\0\0\x01\0\0\0common_shirt_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO\x24\0\0\0\x01\0\0\0common_pants_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x14\0\x0A\x32\0\x09m\0\0\0\0\0AI_W\x0E\0\0\0\0\x01\x05\0\0<\x14\x0A\x0A\0\0\0\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip NPC_, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before NPCO can be read completely")
    {
      const auto data = "NPC_\x31\x01\0\0\0\0\0\0\0\0\0\0NAME\x0C\0\0\0galthragoth\0FNAM\x0C\0\0\0Galthragoth\0RNAM\x09\0\0\0Wood Elf\0CNAM\x07\0\0\0Pauper\0ANAM\x01\0\0\0\0BNAM\x17\0\0\0B_N_Wood Elf_M_Head_06\0KNAM\x17\0\0\0b_n_wood elf_m_hair_03\0NPDT\x0C\0\0\0\x05\0\x32\0\0\x09m\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0NPCO$\0\0\0\x01\0\0\0com"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple NPDTs")
    {
      const auto data = "NPC_\x0B\x01\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NPDT is not twelve")
    {
      {
        const auto data = "NPC_\xF6\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0B\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip NPC_, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0D\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip NPC_, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before NPDT can be read")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no RNAM")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0FAIL\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple RNAMs")
    {
      const auto data = "NPC_\x08\x01\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of RNAM > 256")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\x01\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of RNAM is beyond stream")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\xF7\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple SCRIs")
    {
      const auto data = "NPC_\x06\x01\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\x01\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\xF7\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
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
