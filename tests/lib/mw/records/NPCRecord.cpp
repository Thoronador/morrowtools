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

#include "../../locate_catch.hpp"
#include <limits>
#include <sstream>
#include <string_view>
#include "../../../../lib/mw/records/NPCRecord.hpp"

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
    REQUIRE_FALSE( record.AIData.has_value() );
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

  SECTION("getWriteSize")
  {
    SECTION("throws when NPDT size is 'none'")
    {
      NPCRecord record;
      record.NPCDataType = NPDT_Type::ndtNone;

      REQUIRE_THROWS( record.getWriteSize() );
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
      REQUIRE( record.AIData.has_value() );
      REQUIRE( record.AIData.value().Hello == 30 );
      REQUIRE( record.AIData.value().Unknown1 == 0 );
      REQUIRE( record.AIData.value().Fight == 30 );
      REQUIRE( record.AIData.value().Flee == 20 );
      REQUIRE( record.AIData.value().Alarm == 0 );
      REQUIRE( record.AIData.value().Unknown2 == 0xD6 );
      REQUIRE( record.AIData.value().Unknown3 == 0xE8 );
      REQUIRE( record.AIData.value().Unknown4 == 0 );
      REQUIRE( record.AIData.value().Flags == 0 );
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

    SECTION("default: load record with 52-byte NPDT")
    {
      const auto data = "NPC_\x41\x01\0\0\0\0\0\0\0\0\0\0NAME\x0C\0\0\0j'othri_dar\0MODL\x11\0\0\0base_animKnA.nif\0FNAM\x0C\0\0\0Jo'Thri-Dar\0RNAM\x08\0\0\0Khajiit\0CNAM\x06\0\0\0Thief\0ANAM\x01\0\0\0\0BNAM\x16\0\0\0b_n_khajiit_m_head_02\0KNAM\x15\0\0\0b_n_khajiit_m_hair01\0NPDT4\0\0\0\x03\0#-*094,(\x05\x05\x05\x05\x05\x05\x05\x05\x17\x05\x05\x05\x05\x05\x05\x05\x05\x05((2((\x14\x14\x14\x18\0:\0\x16\0\xB1\0\x32\0\0\0\0\0\0\0FLAG\x04\0\0\0\x08\0\0\0NPCO$\0\0\0\x01\0\0\0common_pants_04\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x1E\0\0\0\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0<\x14\x0A\x0A\0\x0A\0\0\x01"sv;
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
      REQUIRE( record.recordID == "j'othri_dar" );
      REQUIRE( record.Name == "Jo'Thri-Dar" );
      REQUIRE( record.ModelPath == "base_animKnA.nif" );
      REQUIRE( record.RaceID == "Khajiit" );
      REQUIRE( record.FactionID.empty() );
      REQUIRE( record.HeadModel == "b_n_khajiit_m_head_02" );
      REQUIRE( record.ClassID == "Thief" );
      REQUIRE( record.HairModel == "b_n_khajiit_m_hair01" );
      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.Level == 3 );
      REQUIRE( record.Strength == 35 );
      REQUIRE( record.Intelligence == 45 );
      REQUIRE( record.Willpower == 42 );
      REQUIRE( record.Agility == 48 );
      REQUIRE( record.Speed == 57 );
      REQUIRE( record.Endurance == 52 );
      REQUIRE( record.Personality == 44 );
      REQUIRE( record.Luck == 40 );
      REQUIRE( record.Skills[0] == 5 );
      REQUIRE( record.Skills[1] == 5 );
      REQUIRE( record.Skills[2] == 5 );
      REQUIRE( record.Skills[3] == 5 );
      REQUIRE( record.Skills[4] == 5 );
      REQUIRE( record.Skills[5] == 5 );
      REQUIRE( record.Skills[6] == 5 );
      REQUIRE( record.Skills[7] == 5 );
      REQUIRE( record.Skills[8] == 23 );
      REQUIRE( record.Skills[9] == 5 );
      REQUIRE( record.Skills[10] == 5 );
      REQUIRE( record.Skills[11] == 5 );
      REQUIRE( record.Skills[12] == 5 );
      REQUIRE( record.Skills[13] == 5 );
      REQUIRE( record.Skills[14] == 5 );
      REQUIRE( record.Skills[15] == 5 );
      REQUIRE( record.Skills[16] == 5 );
      REQUIRE( record.Skills[17] == 5 );
      REQUIRE( record.Skills[18] == 40 );
      REQUIRE( record.Skills[19] == 40 );
      REQUIRE( record.Skills[20] == 50 );
      REQUIRE( record.Skills[21] == 40 );
      REQUIRE( record.Skills[22] == 40 );
      REQUIRE( record.Skills[23] == 20 );
      REQUIRE( record.Skills[24] == 20 );
      REQUIRE( record.Skills[25] == 20 );
      REQUIRE( record.Skills[26] == 24 );
      REQUIRE( record.Reputation == 0 );
      REQUIRE( record.Health == 58 );
      REQUIRE( record.SpellPoints == 22 );
      REQUIRE( record.Fatigue == 177 );
      REQUIRE( record.Disposition == 50 );
      REQUIRE( record.Data_FactionID == 0 );
      REQUIRE( record.Rank == 0 );
      REQUIRE( record.Unknown1 == 0x00 );
      REQUIRE( record.Gold == 0 );
      REQUIRE( record.NPCDataType == NPDT_Type::ndt52Bytes );
      REQUIRE( record.NPC_Flag == 0x00000008 );
      // stuff from PreNPCRecord
      REQUIRE( record.Items.size() == 1 );
      REQUIRE( record.Items[0].Count == 1 );
      REQUIRE( record.Items[0].Item == "common_pants_04" );
      REQUIRE( record.NPC_Spells.empty() );
      REQUIRE( record.AIData.has_value() );
      REQUIRE( record.AIData.value().Hello == 30 );
      REQUIRE( record.AIData.value().Unknown1 == 0 );
      REQUIRE( record.AIData.value().Fight == 30 );
      REQUIRE( record.AIData.value().Flee == 30 );
      REQUIRE( record.AIData.value().Alarm == 0 );
      REQUIRE( record.AIData.value().Unknown2 == 0x00 );
      REQUIRE( record.AIData.value().Unknown3 == 0x00 );
      REQUIRE( record.AIData.value().Unknown4 == 0x00 );
      REQUIRE( record.AIData.value().Flags == 0 );
      REQUIRE( record.AIPackages.size() == 1 );
      REQUIRE( record.AIPackages[0]->getPackageType() == PackageType::ptWander );
      const auto package_one = static_cast<NPC_AIWander*>(record.AIPackages[0]);
      REQUIRE( package_one->Distance == 0 );
      REQUIRE( package_one->Duration == 5 );
      REQUIRE( package_one->Time == 0 );
      REQUIRE( package_one->Idle[0] == 60 );
      REQUIRE( package_one->Idle[1] == 20 );
      REQUIRE( package_one->Idle[2] == 10 );
      REQUIRE( package_one->Idle[3] == 10 );
      REQUIRE( package_one->Idle[4] == 0 );
      REQUIRE( package_one->Idle[5] == 10 );
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
      REQUIRE( record.AIData.has_value() );
      REQUIRE( record.AIData.value().Hello == 20 );
      REQUIRE( record.AIData.value().Unknown1 == 0 );
      REQUIRE( record.AIData.value().Fight == 10 );
      REQUIRE( record.AIData.value().Flee == 50 );
      REQUIRE( record.AIData.value().Alarm == 0 );
      REQUIRE( record.AIData.value().Unknown2 == 0x09 );
      REQUIRE( record.AIData.value().Unknown3 == 0x6D );
      REQUIRE( record.AIData.value().Unknown4 == 0 );
      REQUIRE( record.AIData.value().Flags == 0 );
      REQUIRE( record.AIPackages.size() == 1 );
      REQUIRE( record.AIPackages[0]->getPackageType() == PackageType::ptWander );
      const auto package_one = static_cast<NPC_AIWander*>(record.AIPackages[0]);
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

    SECTION("default: load record with spells")
    {
      const auto data = "NPC_\xCC\x01\0\0\0\0\0\0\0\x20\0\0NAME\x0C\0\0\0uvoo llaren\0FNAM\x0C\0\0\0Uvoo Llaren\0RNAM\x09\0\0\0Dark Elf\0CNAM\x0D\0\0\0Monk Service\0ANAM\x07\0\0\0Temple\0BNAM\x17\0\0\0b_n_dark elf_f_head_02\0KNAM\x17\0\0\0b_n_dark elf_f_hair_12\0SCRI\x0F\0\0\0templeStewards\0NPDT4\0\0\0\x13\0N6<_d)3(!\x07\x07\x07!\x0C\x07\x07\x35\x07\x11\x07\x07\x07\x0C!\x07\x30\x15>>4\x1F\x34\x0A\x15>\0\x83\0l\0\x12\x01\x32\x12\x08\0\0\0\0\0FLAG\x04\0\0\0\x09\0\0\0NPCO$\0\0\0\x01\0\0\0expensive_robe_02_a\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_shoes_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0balyna's soothing balm\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0Ogrul's_Quick_Again\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\x64\xECM\0\0@\0\0AI_W\x0E\0\0\0\x80\0\x05\0\0(\x14\x14\x0A\0\0\0\0\x01"sv;
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
      REQUIRE( record.getHeaderFlags() == 0x00002000 );
      // -- record data
      // stuff from NPCRecord
      REQUIRE( record.recordID == "uvoo llaren" );
      REQUIRE( record.Name == "Uvoo Llaren" );
      REQUIRE( record.ModelPath.empty() );
      REQUIRE( record.RaceID == "Dark Elf" );
      REQUIRE( record.FactionID == "Temple" );
      REQUIRE( record.HeadModel == "b_n_dark elf_f_head_02" );
      REQUIRE( record.ClassID == "Monk Service" );
      REQUIRE( record.HairModel == "b_n_dark elf_f_hair_12" );
      REQUIRE( record.ScriptID == "templeStewards" );
      REQUIRE( record.Level == 19 );
      REQUIRE( record.Strength == 78 );
      REQUIRE( record.Intelligence == 54 );
      REQUIRE( record.Willpower == 60 );
      REQUIRE( record.Agility == 95 );
      REQUIRE( record.Speed == 100 );
      REQUIRE( record.Endurance == 41 );
      REQUIRE( record.Personality == 51 );
      REQUIRE( record.Luck == 40 );
      REQUIRE( record.Skills[0] == 33 );
      REQUIRE( record.Skills[1] == 7 );
      REQUIRE( record.Skills[2] == 7 );
      REQUIRE( record.Skills[3] == 7 );
      REQUIRE( record.Skills[4] == 33 );
      REQUIRE( record.Skills[5] == 12 );
      REQUIRE( record.Skills[6] == 7 );
      REQUIRE( record.Skills[7] == 7 );
      REQUIRE( record.Skills[8] == 53 );
      REQUIRE( record.Skills[9] == 7 );
      REQUIRE( record.Skills[10] == 17 );
      REQUIRE( record.Skills[11] == 7 );
      REQUIRE( record.Skills[12] == 7 );
      REQUIRE( record.Skills[13] == 7 );
      REQUIRE( record.Skills[14] == 12 );
      REQUIRE( record.Skills[15] == 33 );
      REQUIRE( record.Skills[16] == 7 );
      REQUIRE( record.Skills[17] == 48 );
      REQUIRE( record.Skills[18] == 21 );
      REQUIRE( record.Skills[19] == 62 );
      REQUIRE( record.Skills[20] == 62 );
      REQUIRE( record.Skills[21] == 52 );
      REQUIRE( record.Skills[22] == 31 );
      REQUIRE( record.Skills[23] == 52 );
      REQUIRE( record.Skills[24] == 10 );
      REQUIRE( record.Skills[25] == 21 );
      REQUIRE( record.Skills[26] == 62 );
      REQUIRE( record.Reputation == 0 );
      REQUIRE( record.Health == 131 );
      REQUIRE( record.SpellPoints == 108 );
      REQUIRE( record.Fatigue == 274 );
      REQUIRE( record.Disposition == 50 );
      REQUIRE( record.Data_FactionID == 18 );
      REQUIRE( record.Rank == 8 );
      REQUIRE( record.Unknown1 == 0x00 );
      REQUIRE( record.Gold == 0 );
      REQUIRE( record.NPCDataType == NPDT_Type::ndt52Bytes );
      REQUIRE( record.NPC_Flag == 0x00000009 );
      // stuff from PreNPCRecord
      REQUIRE( record.Items.size() == 2 );
      REQUIRE( record.Items[0].Count == 1 );
      REQUIRE( record.Items[0].Item == "expensive_robe_02_a" );
      REQUIRE( record.Items[1].Count == 1 );
      REQUIRE( record.Items[1].Item == "common_shoes_02" );
      REQUIRE( record.NPC_Spells.size() == 2 );
      REQUIRE( record.NPC_Spells[0] == "balyna's soothing balm" );
      REQUIRE( record.NPC_Spells[1] == "Ogrul's_Quick_Again" );
      REQUIRE( record.AIData.has_value() );
      REQUIRE( record.AIData.value().Hello == 30 );
      REQUIRE( record.AIData.value().Unknown1 == 0 );
      REQUIRE( record.AIData.value().Fight == 30 );
      REQUIRE( record.AIData.value().Flee == 20 );
      REQUIRE( record.AIData.value().Alarm == 100 );
      REQUIRE( record.AIData.value().Unknown2 == 0xEC );
      REQUIRE( record.AIData.value().Unknown3 == 0x4D );
      REQUIRE( record.AIData.value().Unknown4 == 0 );
      REQUIRE( record.AIData.value().Flags == 0x00004000 );
      REQUIRE( record.AIPackages.size() == 1 );
      REQUIRE( record.AIPackages[0]->getPackageType() == PackageType::ptWander );
      const auto package_one = static_cast<NPC_AIWander*>(record.AIPackages[0]);
      REQUIRE( package_one->Distance == 128 );
      REQUIRE( package_one->Duration == 5 );
      REQUIRE( package_one->Time == 0 );
      REQUIRE( package_one->Idle[0] == 40 );
      REQUIRE( package_one->Idle[1] == 20 );
      REQUIRE( package_one->Idle[2] == 20 );
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

    SECTION("default: load record with travel destinations")
    {
      const auto data = "NPC_\x78\x01\0\0\0\0\0\0\0\0\0\0NAME\x0E\0\0\0dilami androm\0FNAM\x0E\0\0\0Dilami Androm\0RNAM\x09\0\0\0Dark Elf\0CNAM\x0A\0\0\0Caravaner\0ANAM\x01\0\0\0\0BNAM\x17\0\0\0b_n_dark elf_f_head_09\0KNAM\x17\0\0\0b_n_dark elf_f_hair_21\0NPDT\x0C\0\0\0\x01\0\x32\0\0\0\0\0\0\0\0\0FLAG\x04\0\0\0\x19\0\0\0NPCO$\0\0\0\x01\0\0\0common_shirt_01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_pants_01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x1E\0\0\0\0\0\0\0\0DODT\x18\0\0\0\0\xAAOG@f<\xC7\xD3\x08vD\0\0\0\0\0\0\0\0\xDB\x0FI@DODT\x18\0\0\0\x04\x9E\xFB\x46K\x10\x8D\xC7\xBC\xC7{D\0\0\0\0\0\0\0\0\xE4\xCB\x96@AI_W\x0E\0\0\0\0\0\x05\0\0<\x14\x0A\x0A\0\0\0\0\x01"sv;
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
      REQUIRE( record.recordID == "dilami androm" );
      REQUIRE( record.Name == "Dilami Androm" );
      REQUIRE( record.ModelPath.empty() );
      REQUIRE( record.RaceID == "Dark Elf" );
      REQUIRE( record.FactionID.empty() );
      REQUIRE( record.HeadModel == "b_n_dark elf_f_head_09" );
      REQUIRE( record.ClassID == "Caravaner" );
      REQUIRE( record.HairModel == "b_n_dark elf_f_hair_21" );
      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.Level == 1 );
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
      REQUIRE( record.Unknown1 == 0x00 );
      REQUIRE( record.Unknown2 == 0x00 );
      REQUIRE( record.Unknown3 == 0x00 );
      REQUIRE( record.Gold == 0 );
      REQUIRE( record.NPCDataType == NPDT_Type::ndt12Bytes );
      REQUIRE( record.NPC_Flag == 0x00000019 );
      // stuff from PreNPCRecord
      REQUIRE( record.Items.size() == 2 );
      REQUIRE( record.Items[0].Count == 1 );
      REQUIRE( record.Items[0].Item == "common_shirt_01" );
      REQUIRE( record.Items[1].Count == 1 );
      REQUIRE( record.Items[1].Item == "common_pants_01" );
      REQUIRE( record.NPC_Spells.empty() );
      REQUIRE( record.AIData.has_value() );
      REQUIRE( record.AIData.value().Hello == 30 );
      REQUIRE( record.AIData.value().Unknown1 == 0 );
      REQUIRE( record.AIData.value().Fight == 30 );
      REQUIRE( record.AIData.value().Flee == 30 );
      REQUIRE( record.AIData.value().Alarm == 0 );
      REQUIRE( record.AIData.value().Unknown2 == 0 );
      REQUIRE( record.AIData.value().Unknown3 == 0 );
      REQUIRE( record.AIData.value().Unknown4 == 0 );
      REQUIRE( record.AIData.value().Flags == 0x00000000 );
      REQUIRE( record.AIPackages.size() == 1 );
      REQUIRE( record.AIPackages[0]->getPackageType() == PackageType::ptWander );
      const auto package_one = static_cast<NPC_AIWander*>(record.AIPackages[0]);
      REQUIRE( package_one->Distance == 0 );
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
      REQUIRE( record.Destinations.size() == 2 );

      REQUIRE( record.Destinations[0].XPos == 53162.0f );
      REQUIRE( record.Destinations[0].YPos == -48230.25f );
      REQUIRE( record.Destinations[0].ZPos == 984.13787841797f );
      REQUIRE( record.Destinations[0].XRot == 0.0f );
      REQUIRE( record.Destinations[0].YRot == 0.0f );
      REQUIRE( record.Destinations[0].ZRot == 3.1415927410126f );
      REQUIRE( record.Destinations[0].CellName.empty() );

      REQUIRE( record.Destinations[1].XPos == 32207.0078125f );
      REQUIRE( record.Destinations[1].YPos == -72224.5859375f );
      REQUIRE( record.Destinations[1].ZPos == 1007.1208496094f );
      REQUIRE( record.Destinations[1].XRot == 0.0f );
      REQUIRE( record.Destinations[1].YRot == 0.0f );
      REQUIRE( record.Destinations[1].ZRot == 4.7123889923096f );
      REQUIRE( record.Destinations[1].CellName.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with AI follow package")
    {
      const auto data = "NPC_\x81\x01\0\0\0\0\0\0\0\0\0\0NAME\x0B\0\0\0dals sadri\0FNAM\x0B\0\0\0Dals Sadri\0RNAM\x09\0\0\0Dark Elf\0CNAM\x05\0\0\0Mage\0ANAM\x07\0\0\0Hlaalu\0BNAM\x17\0\0\0b_n_dark elf_m_head_12\0KNAM\x17\0\0\0b_n_dark elf_m_hair_12\0NPDT\x0C\0\0\0\x09\0\x32\x04\x01\0\0\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0NPCO$\0\0\0\x01\0\0\0common_robe_03_a\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_shoes_03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0l_m_wpn_melee_short blade\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x1EZ\0\0\0\0\0\0\0AI_F0\0\0\0\xFF\xFF\x7F\x7F\xFF\xFF\x7F\x7F\xFF\xFF\x7F\x7F\0\0mavon drenim\0\0\x0C\0\0\0\xA8\xD4\xE8\0\xF1HO\0$\xF6\x12\0\x0C\0\x01\0"sv;
      const auto data_clean = "NPC_\x81\x01\0\0\0\0\0\0\0\0\0\0NAME\x0B\0\0\0dals sadri\0FNAM\x0B\0\0\0Dals Sadri\0RNAM\x09\0\0\0Dark Elf\0CNAM\x05\0\0\0Mage\0ANAM\x07\0\0\0Hlaalu\0BNAM\x17\0\0\0b_n_dark elf_m_head_12\0KNAM\x17\0\0\0b_n_dark elf_m_hair_12\0NPDT\x0C\0\0\0\x09\0\x32\x04\x01\0\0\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0NPCO$\0\0\0\x01\0\0\0common_robe_03_a\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_shoes_03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0l_m_wpn_melee_short blade\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x1EZ\0\0\0\0\0\0\0AI_F0\0\0\0\xFF\xFF\x7F\x7F\xFF\xFF\x7F\x7F\xFF\xFF\x7F\x7F\0\0mavon drenim\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0"sv;
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
      REQUIRE( record.recordID == "dals sadri" );
      REQUIRE( record.Name == "Dals Sadri" );
      REQUIRE( record.ModelPath.empty() );
      REQUIRE( record.RaceID == "Dark Elf" );
      REQUIRE( record.FactionID == "Hlaalu" );
      REQUIRE( record.HeadModel == "b_n_dark elf_m_head_12" );
      REQUIRE( record.ClassID == "Mage" );
      REQUIRE( record.HairModel == "b_n_dark elf_m_hair_12" );
      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.Level == 9 );
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
      REQUIRE( record.Unknown1 == 0x00 );
      REQUIRE( record.Unknown2 == 0x00 );
      REQUIRE( record.Unknown3 == 0x00 );
      REQUIRE( record.Gold == 0 );
      REQUIRE( record.NPCDataType == NPDT_Type::ndt12Bytes );
      REQUIRE( record.NPC_Flag == 0x00000018 );
      // stuff from PreNPCRecord
      REQUIRE( record.Items.size() == 3 );
      REQUIRE( record.Items[0].Count == 1 );
      REQUIRE( record.Items[0].Item == "common_robe_03_a" );
      REQUIRE( record.Items[1].Count == 1 );
      REQUIRE( record.Items[1].Item == "common_shoes_03" );
      REQUIRE( record.Items[2].Count == 1 );
      REQUIRE( record.Items[2].Item == "l_m_wpn_melee_short blade" );
      REQUIRE( record.NPC_Spells.empty() );
      REQUIRE( record.AIData.has_value() );
      REQUIRE( record.AIData.value().Hello == 30 );
      REQUIRE( record.AIData.value().Unknown1 == 0 );
      REQUIRE( record.AIData.value().Fight == 30 );
      REQUIRE( record.AIData.value().Flee == 30 );
      REQUIRE( record.AIData.value().Alarm == 90 );
      REQUIRE( record.AIData.value().Unknown2 == 0 );
      REQUIRE( record.AIData.value().Unknown3 == 0 );
      REQUIRE( record.AIData.value().Unknown4 == 0 );
      REQUIRE( record.AIData.value().Flags == 0x00000000 );
      REQUIRE( record.AIPackages.size() == 1 );
      REQUIRE( record.AIPackages[0]->getPackageType() == PackageType::ptFollow );
      const auto package_one = static_cast<NPC_AIFollow*>(record.AIPackages[0]);
      REQUIRE( package_one->X == std::numeric_limits<float>::max() );
      REQUIRE( package_one->Y == std::numeric_limits<float>::max() );
      REQUIRE( package_one->Z == std::numeric_limits<float>::max() );
      REQUIRE( package_one->Duration == 0 );
      REQUIRE( package_one->TargetID == "mavon drenim" );
      REQUIRE( package_one->Reset == 1 );
      REQUIRE( package_one->CellName.empty() );
      REQUIRE( record.Destinations.size() == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data_clean );
    }

    SECTION("default: load record with AI escort package")
    {
      const auto data = "NPC_\x81\x01\0\0\0\0\0\0\0\0\0\0NAME\x0B\0\0\0dals sadri\0FNAM\x0B\0\0\0Dals Sadri\0RNAM\x09\0\0\0Dark Elf\0CNAM\x05\0\0\0Mage\0ANAM\x07\0\0\0Hlaalu\0BNAM\x17\0\0\0b_n_dark elf_m_head_12\0KNAM\x17\0\0\0b_n_dark elf_m_hair_12\0NPDT\x0C\0\0\0\x09\0\x32\x04\x01\0\0\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0NPCO$\0\0\0\x01\0\0\0common_robe_03_a\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_shoes_03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0l_m_wpn_melee_short blade\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x1EZ\0\0\0\0\0\0\0AI_E0\0\0\0\xFF\xFF\x7F\x7F\xFF\xFF\x7F\x7F\xFF\xFF\x7F\x7F\0\0mavon drenim\0\0\x0C\0\0\0\xA8\xD4\xE8\0\xF1HO\0$\xF6\x12\0\x0C\0\x01\0"sv;
      const auto data_clean = "NPC_\x81\x01\0\0\0\0\0\0\0\0\0\0NAME\x0B\0\0\0dals sadri\0FNAM\x0B\0\0\0Dals Sadri\0RNAM\x09\0\0\0Dark Elf\0CNAM\x05\0\0\0Mage\0ANAM\x07\0\0\0Hlaalu\0BNAM\x17\0\0\0b_n_dark elf_m_head_12\0KNAM\x17\0\0\0b_n_dark elf_m_hair_12\0NPDT\x0C\0\0\0\x09\0\x32\x04\x01\0\0\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0NPCO$\0\0\0\x01\0\0\0common_robe_03_a\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_shoes_03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0l_m_wpn_melee_short blade\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x1EZ\0\0\0\0\0\0\0AI_E0\0\0\0\xFF\xFF\x7F\x7F\xFF\xFF\x7F\x7F\xFF\xFF\x7F\x7F\0\0mavon drenim\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0"sv;
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
      REQUIRE( record.recordID == "dals sadri" );
      REQUIRE( record.Name == "Dals Sadri" );
      // Skip other stuff, except for package.
      REQUIRE( record.AIPackages.size() == 1 );
      REQUIRE( record.AIPackages[0]->getPackageType() == PackageType::ptEscort );
      const auto package_one = static_cast<NPC_AIEscort*>(record.AIPackages[0]);
      REQUIRE( package_one->X == std::numeric_limits<float>::max() );
      REQUIRE( package_one->Y == std::numeric_limits<float>::max() );
      REQUIRE( package_one->Z == std::numeric_limits<float>::max() );
      REQUIRE( package_one->Duration == 0 );
      REQUIRE( package_one->TargetID == "mavon drenim" );
      REQUIRE( package_one->Reset == 1 );
      REQUIRE( package_one->CellName.empty() );
      REQUIRE( record.Destinations.size() == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data_clean );
    }

    SECTION("default: load record with AI travel packages")
    {
      const auto data = "NPC_\x95\x01\0\0\0\0\0\0\0\x04\0\0NAME\x07\0\0\0indrel\0FNAM\x07\0\0\0Indrel\0RNAM\x09\0\0\0Wood Elf\0CNAM\x05\0\0\0Monk\0ANAM\x01\0\0\0\0BNAM\x17\0\0\0b_n_wood elf_f_head_05\0KNAM\x17\0\0\0b_n_wood elf_f_hair_04\0NPDT4\0\0\0\x05\0 )3++=\x1F(\x05\x05\x05\x05\x05\x05\x05\x10\x1F\x05\x05\x10\x05\x05\x05\x05\x0A\x1F\x06*$\x1A\x06.\x06\x10\x10\0\0\0\x14\0\xBB\0\x32\0\0\0\0\0\0\0FLAG\x04\0\0\0\x09\0\0\0NPCO$\0\0\0\x01\0\0\0common_robe_02_t\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_shoes_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0(2\x1E\0\0\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0<\x14\x0A\x0A\0\0\0\0\x01\x41I_T\x10\0\0\0\0\x80\x84\xC3\0\xE0;E\0\xE0\x8B\xC4\x01\x01\0\0AI_T\x10\0\0\0\0\0w\xC3\0 WE\0\xE0\x9B\xC4\x01\x01\0\0AI_T\x10\0\0\0\0\0\xC0\x42\0\x30XE\0\xE0\x9B\xC4\x01\x01\0\0"sv;
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
      REQUIRE( record.getHeaderFlags() == 0x00000400 );
      // -- record data
      REQUIRE( record.recordID == "indrel" );
      REQUIRE( record.Name == "Indrel" );
      // Skip other stuff, except for package.
      REQUIRE( record.AIPackages.size() == 4 );
      // First package is AI Wander, so we do not test it.
      // There are already other tests with AI Wander.
      REQUIRE( record.AIPackages[1]->getPackageType() == PackageType::ptTravel );
      const auto package_one = static_cast<NPC_AITravel*>(record.AIPackages[1]);
      REQUIRE( package_one->X == -265.0f );
      REQUIRE( package_one->Y == 3006.0f );
      REQUIRE( package_one->Z == -1119.0f );
      REQUIRE( package_one->Reset == 257 );
      REQUIRE( record.AIPackages[2]->getPackageType() == PackageType::ptTravel );
      const auto package_two = static_cast<NPC_AITravel*>(record.AIPackages[2]);
      REQUIRE( package_two->X == -247.0f );
      REQUIRE( package_two->Y == 3442.0f );
      REQUIRE( package_two->Z == -1247.0f );
      REQUIRE( package_two->Reset == 257 );
      REQUIRE( record.AIPackages[2]->getPackageType() == PackageType::ptTravel );
      const auto package_three = static_cast<NPC_AITravel*>(record.AIPackages[3]);
      REQUIRE( package_three->X == 96.0f );
      REQUIRE( package_three->Y == 3459.0f );
      REQUIRE( package_three->Z == -1247.0f );
      REQUIRE( package_three->Reset == 257 );
      REQUIRE( record.Destinations.size() == 0 );

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

    SECTION("corrupt data: stream ends before AI_A can be read")
    {
      const auto data = "NPC_\x95\x01\0\0\0\0\0\0\0\x04\0\0NAME\x07\0\0\0indrel\0FNAM\x07\0\0\0Indrel\0RNAM\x09\0\0\0Wood Elf\0CNAM\x05\0\0\0Monk\0ANAM\x01\0\0\0\0BNAM\x17\0\0\0b_n_wood elf_f_head_05\0KNAM\x17\0\0\0b_n_wood elf_f_hair_04\0NPDT4\0\0\0\x05\0 )3++=\x1F(\x05\x05\x05\x05\x05\x05\x05\x10\x1F\x05\x05\x10\x05\x05\x05\x05\x0A\x1F\x06*$\x1A\x06.\x06\x10\x10\0\0\0\x14\0\xBB\0\x32\0\0\0\0\0\0\0FLAG\x04\0\0\0\x09\0\0\0NPCO$\0\0\0\x01\0\0\0common_robe_02_t\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_shoes_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0(2\x1E\0\0\0\0\0\0\0AI_A\x21\0\0\0fail\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before AI_E can be read")
    {
      const auto data = "NPC_\x81\x01\0\0\0\0\0\0\0\0\0\0NAME\x0B\0\0\0dals sadri\0FNAM\x0B\0\0\0Dals Sadri\0RNAM\x09\0\0\0Dark Elf\0CNAM\x05\0\0\0Mage\0ANAM\x07\0\0\0Hlaalu\0BNAM\x17\0\0\0b_n_dark elf_m_head_12\0KNAM\x17\0\0\0b_n_dark elf_m_hair_12\0NPDT\x0C\0\0\0\x09\0\x32\x04\x01\0\0\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0NPCO$\0\0\0\x01\0\0\0common_robe_03_a\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_shoes_03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0l_m_wpn_melee_short blade\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x1EZ\0\0\0\0\0\0\0AI_E0\0\0\0\xFF\xFF\x7F\x7F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before AI_F can be read")
    {
      const auto data = "NPC_\x81\x01\0\0\0\0\0\0\0\0\0\0NAME\x0B\0\0\0dals sadri\0FNAM\x0B\0\0\0Dals Sadri\0RNAM\x09\0\0\0Dark Elf\0CNAM\x05\0\0\0Mage\0ANAM\x07\0\0\0Hlaalu\0BNAM\x17\0\0\0b_n_dark elf_m_head_12\0KNAM\x17\0\0\0b_n_dark elf_m_hair_12\0NPDT\x0C\0\0\0\x09\0\x32\x04\x01\0\0\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0NPCO$\0\0\0\x01\0\0\0common_robe_03_a\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_shoes_03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0l_m_wpn_melee_short blade\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x1EZ\0\0\0\0\0\0\0AI_F0\0\0\0\xFF\xFF\x7F\x7F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before AI_T can be read")
    {
      const auto data = "NPC_\x95\x01\0\0\0\0\0\0\0\x04\0\0NAME\x07\0\0\0indrel\0FNAM\x07\0\0\0Indrel\0RNAM\x09\0\0\0Wood Elf\0CNAM\x05\0\0\0Monk\0ANAM\x01\0\0\0\0BNAM\x17\0\0\0b_n_wood elf_f_head_05\0KNAM\x17\0\0\0b_n_wood elf_f_hair_04\0NPDT4\0\0\0\x05\0 )3++=\x1F(\x05\x05\x05\x05\x05\x05\x05\x10\x1F\x05\x05\x10\x05\x05\x05\x05\x0A\x1F\x06*$\x1A\x06.\x06\x10\x10\0\0\0\x14\0\xBB\0\x32\0\0\0\0\0\0\0FLAG\x04\0\0\0\x09\0\0\0NPCO$\0\0\0\x01\0\0\0common_robe_02_t\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_shoes_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0(2\x1E\0\0\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0<\x14\x0A\x0A\0\0\0\0\x01\x41I_T\x10\0\0\0\0\x80"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before AI_W can be read")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05"sv;
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

    SECTION("corrupt data: length of DODT is not 24")
    {
      {
        const auto data = "NPC_\x77\x01\0\0\0\0\0\0\0\0\0\0NAME\x0E\0\0\0dilami androm\0FNAM\x0E\0\0\0Dilami Androm\0RNAM\x09\0\0\0Dark Elf\0CNAM\x0A\0\0\0Caravaner\0ANAM\x01\0\0\0\0BNAM\x17\0\0\0b_n_dark elf_f_head_09\0KNAM\x17\0\0\0b_n_dark elf_f_hair_21\0NPDT\x0C\0\0\0\x01\0\x32\0\0\0\0\0\0\0\0\0FLAG\x04\0\0\0\x19\0\0\0NPCO$\0\0\0\x01\0\0\0common_shirt_01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_pants_01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x1E\0\0\0\0\0\0\0\0DODT\x18\0\0\0\0\xAAOG@f<\xC7\xD3\x08vD\0\0\0\0\0\0\0\0\xDB\x0FI@DODT\x17\0\0\0\x04\x9E\xFB\x46K\x10\x8D\xC7\xBC\xC7{D\0\0\0\0\0\0\0\xE4\xCB\x96@AI_W\x0E\0\0\0\0\0\x05\0\0<\x14\x0A\x0A\0\0\0\0\x01"sv;
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
        const auto data = "NPC_\x79\x01\0\0\0\0\0\0\0\0\0\0NAME\x0E\0\0\0dilami androm\0FNAM\x0E\0\0\0Dilami Androm\0RNAM\x09\0\0\0Dark Elf\0CNAM\x0A\0\0\0Caravaner\0ANAM\x01\0\0\0\0BNAM\x17\0\0\0b_n_dark elf_f_head_09\0KNAM\x17\0\0\0b_n_dark elf_f_hair_21\0NPDT\x0C\0\0\0\x01\0\x32\0\0\0\0\0\0\0\0\0FLAG\x04\0\0\0\x19\0\0\0NPCO$\0\0\0\x01\0\0\0common_shirt_01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_pants_01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x1E\0\0\0\0\0\0\0\0DODT\x18\0\0\0\0\xAAOG@f<\xC7\xD3\x08vD\0\0\0\0\0\0\0\0\xDB\x0FI@DODT\x19\0\0\0\x04\x9E\xFB\x46K\x10\x8D\xC7\xBC\xC7{D\0\0\0\0\0\0\0\0\xE4\xCB\x96@\0AI_W\x0E\0\0\0\0\0\x05\0\0<\x14\x0A\x0A\0\0\0\0\x01"sv;
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

    SECTION("corrupt data: stream ends before DODT can be read")
    {
      const auto data = "NPC_\x78\x01\0\0\0\0\0\0\0\0\0\0NAME\x0E\0\0\0dilami androm\0FNAM\x0E\0\0\0Dilami Androm\0RNAM\x09\0\0\0Dark Elf\0CNAM\x0A\0\0\0Caravaner\0ANAM\x01\0\0\0\0BNAM\x17\0\0\0b_n_dark elf_f_head_09\0KNAM\x17\0\0\0b_n_dark elf_f_hair_21\0NPDT\x0C\0\0\0\x01\0\x32\0\0\0\0\0\0\0\0\0FLAG\x04\0\0\0\x19\0\0\0NPCO$\0\0\0\x01\0\0\0common_shirt_01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_pants_01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x1E\0\0\0\0\0\0\0\0DODT\x18\0\0\0\0\xAAOG@f"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FLAG")
    {
      const auto data = "NPC_\xF7\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0listien bierles\0FNAM\x10\0\0\0Listien Bierles\0RNAM\x07\0\0\0Breton\0CNAM\x09\0\0\0Sorcerer\0ANAM\x0C\0\0\0Mages Guild\0BNAM\x15\0\0\0b_n_breton_m_head_02\0KNAM\x15\0\0\0b_n_breton_m_hair_03\0SCRI\x07\0\0\0nolore\0NPDT\x0C\0\0\0\x02\0\x32\x04\x01\xD6\xE8\0\0\0\0\0FAIL\x04\0\0\0\x18\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\0\xD6\xE8\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0(\x14\x14\x0A\0\0\x0A\0\x01"sv;
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

    SECTION("corrupt data: multiple MODLs")
    {
      const auto data = "NPC_\x5A\x01\0\0\0\0\0\0\0\0\0\0NAME\x0C\0\0\0j'othri_dar\0MODL\x11\0\0\0base_animKnA.nif\0MODL\x11\0\0\0base_animKnA.nif\0FNAM\x0C\0\0\0Jo'Thri-Dar\0RNAM\x08\0\0\0Khajiit\0CNAM\x06\0\0\0Thief\0ANAM\x01\0\0\0\0BNAM\x16\0\0\0b_n_khajiit_m_head_02\0KNAM\x15\0\0\0b_n_khajiit_m_hair01\0NPDT4\0\0\0\x03\0#-*094,(\x05\x05\x05\x05\x05\x05\x05\x05\x17\x05\x05\x05\x05\x05\x05\x05\x05\x05((2((\x14\x14\x14\x18\0:\0\x16\0\xB1\0\x32\0\0\0\0\0\0\0FLAG\x04\0\0\0\x08\0\0\0NPCO$\0\0\0\x01\0\0\0common_pants_04\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x1E\0\0\0\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0<\x14\x0A\x0A\0\x0A\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "NPC_\x41\x01\0\0\0\0\0\0\0\0\0\0NAME\x0C\0\0\0j'othri_dar\0MODL\x11\x01\0\0base_animKnA.nif\0FNAM\x0C\0\0\0Jo'Thri-Dar\0RNAM\x08\0\0\0Khajiit\0CNAM\x06\0\0\0Thief\0ANAM\x01\0\0\0\0BNAM\x16\0\0\0b_n_khajiit_m_head_02\0KNAM\x15\0\0\0b_n_khajiit_m_hair01\0NPDT4\0\0\0\x03\0#-*094,(\x05\x05\x05\x05\x05\x05\x05\x05\x17\x05\x05\x05\x05\x05\x05\x05\x05\x05((2((\x14\x14\x14\x18\0:\0\x16\0\xB1\0\x32\0\0\0\0\0\0\0FLAG\x04\0\0\0\x08\0\0\0NPCO$\0\0\0\x01\0\0\0common_pants_04\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x1E\0\0\0\0\0\0\0\0AI_W\x0E\0\0\0\0\0\x05\0\0<\x14\x0A\x0A\0\x0A\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip NPC_, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream end before MODL can be read completely")
    {
      const auto data = "NPC_\x41\x01\0\0\0\0\0\0\0\0\0\0NAME\x0C\0\0\0j'othri_dar\0MODL\x11\0\0\0base"sv;
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
        const auto data = "NPC_\x31\x01\0\0\0\0\0\0\0\0\0\0NAME\x0C\0\0\0galthragoth\0FNAM\x0C\0\0\0Galthragoth\0RNAM\x09\0\0\0Wood Elf\0CNAM\x07\0\0\0Pauper\0ANAM\x01\0\0\0\0BNAM\x17\0\0\0B_N_Wood Elf_M_Head_06\0KNAM\x17\0\0\0b_n_wood elf_m_hair_03\0NPDT\x0C\0\0\0\x05\0\x32\0\0\x09m\0\0\0\0\0FLAG\x04\0\0\0\x18\0\0\0NPCO$\0\0\0\x01\0\0\0common_shirt_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO\x25\0\0\0\x01\0\0\0common_pants_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x14\0\x0A\x32\0\x09m\0\0\0\0\0AI_W\x0E\0\0\0\0\x01\x05\0\0<\x14\x0A\x0A\0\0\0\0\x01"sv;
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

    SECTION("corrupt data: length of NPCS is not 32")
    {
      {
        const auto data = "NPC_\xCB\x01\0\0\0\0\0\0\0\x20\0\0NAME\x0C\0\0\0uvoo llaren\0FNAM\x0C\0\0\0Uvoo Llaren\0RNAM\x09\0\0\0Dark Elf\0CNAM\x0D\0\0\0Monk Service\0ANAM\x07\0\0\0Temple\0BNAM\x17\0\0\0b_n_dark elf_f_head_02\0KNAM\x17\0\0\0b_n_dark elf_f_hair_12\0SCRI\x0F\0\0\0templeStewards\0NPDT4\0\0\0\x13\0N6<_d)3(!\x07\x07\x07!\x0C\x07\x07\x35\x07\x11\x07\x07\x07\x0C!\x07\x30\x15>>4\x1F\x34\x0A\x15>\0\x83\0l\0\x12\x01\x32\x12\x08\0\0\0\0\0FLAG\x04\0\0\0\x09\0\0\0NPCO$\0\0\0\x01\0\0\0expensive_robe_02_a\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_shoes_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0balyna's soothing balm\0\0\0\0\0\0\0\0\0\0NPCS\x1F\0\0\0Ogrul's_Quick_Again\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\x64\xECM\0\0@\0\0AI_W\x0E\0\0\0\x80\0\x05\0\0(\x14\x14\x0A\0\0\0\0\x01"sv;
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
        const auto data = "NPC_\xCD\x01\0\0\0\0\0\0\0\x20\0\0NAME\x0C\0\0\0uvoo llaren\0FNAM\x0C\0\0\0Uvoo Llaren\0RNAM\x09\0\0\0Dark Elf\0CNAM\x0D\0\0\0Monk Service\0ANAM\x07\0\0\0Temple\0BNAM\x17\0\0\0b_n_dark elf_f_head_02\0KNAM\x17\0\0\0b_n_dark elf_f_hair_12\0SCRI\x0F\0\0\0templeStewards\0NPDT4\0\0\0\x13\0N6<_d)3(!\x07\x07\x07!\x0C\x07\x07\x35\x07\x11\x07\x07\x07\x0C!\x07\x30\x15>>4\x1F\x34\x0A\x15>\0\x83\0l\0\x12\x01\x32\x12\x08\0\0\0\0\0FLAG\x04\0\0\0\x09\0\0\0NPCO$\0\0\0\x01\0\0\0expensive_robe_02_a\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_shoes_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0balyna's soothing balm\0\0\0\0\0\0\0\0\0\0NPCS\x21\0\0\0Ogrul's_Quick_Again\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x0C\0\0\0\x1E\0\x1E\x14\x64\xECM\0\0@\0\0AI_W\x0E\0\0\0\x80\0\x05\0\0(\x14\x14\x0A\0\0\0\0\x01"sv;
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

    SECTION("corrupt data: stream ends before NPCS can be read")
    {
      const auto data = "NPC_\xCC\x01\0\0\0\0\0\0\0\x20\0\0NAME\x0C\0\0\0uvoo llaren\0FNAM\x0C\0\0\0Uvoo Llaren\0RNAM\x09\0\0\0Dark Elf\0CNAM\x0D\0\0\0Monk Service\0ANAM\x07\0\0\0Temple\0BNAM\x17\0\0\0b_n_dark elf_f_head_02\0KNAM\x17\0\0\0b_n_dark elf_f_hair_12\0SCRI\x0F\0\0\0templeStewards\0NPDT4\0\0\0\x13\0N6<_d)3(!\x07\x07\x07!\x0C\x07\x07\x35\x07\x11\x07\x07\x07\x0C!\x07\x30\x15>>4\x1F\x34\x0A\x15>\0\x83\0l\0\x12\x01\x32\x12\x08\0\0\0\0\0FLAG\x04\0\0\0\x09\0\0\0NPCO$\0\0\0\x01\0\0\0expensive_robe_02_a\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0common_shoes_02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0balyna's"sv;
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
