/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021  Dirk Stolle

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
#include "../../../../../mw/base/records/WeaponRecord.hpp"

TEST_CASE("MWTP::WeaponRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    WeaponRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Value == 0 );
    REQUIRE( record.Type == 0 );
    REQUIRE( record.Health == 0 );
    REQUIRE( record.Speed == 0.0f );
    REQUIRE( record.Reach == 0.0f );
    REQUIRE( record.EnchantPts == 0 );
    REQUIRE( record.ChopMin == 0 );
    REQUIRE( record.ChopMax == 0 );
    REQUIRE( record.SlashMin == 0 );
    REQUIRE( record.SlashMax == 0 );
    REQUIRE( record.ThrustMin == 0 );
    REQUIRE( record.ThrustMax == 0 );
    REQUIRE( record.WeaponFlags == 0 );
    REQUIRE( record.InventoryIcon.empty() );
    REQUIRE( record.EnchantmentID.empty() );
    REQUIRE( record.ScriptID.empty() );
  }

  SECTION("equals")
  {
    WeaponRecord a;
    WeaponRecord b;

    SECTION("equal")
    {
      a.recordID = "ItemOne";
      a.ModelPath = "item01.nif";

      b.recordID = "ItemOne";
      b.ModelPath = "item01.nif";

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("recordID mismatch")
      {
        a.recordID = "foo";
        b.recordID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ModelPath mismatch")
      {
        a.ModelPath = "mesh01.nif";
        b.ModelPath = "mesh02.nif";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Name mismatch")
      {
        a.Name = "Alice's weapon";
        b.Name = "Bob's weapon";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Weight mismatch")
      {
        a.Weight = 1.5f;
        b.Weight = 2.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Value mismatch")
      {
        a.Value = 15;
        b.Value = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Type mismatch")
      {
        a.Type = 1;
        b.Type = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Health mismatch")
      {
        a.Health = 500;
        b.Health = 1000;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Speed mismatch")
      {
        a.Speed = 1.5f;
        b.Speed = 2.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Reach mismatch")
      {
        a.Reach = 0.5f;
        b.Reach = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("EnchantPts mismatch")
      {
        a.EnchantPts = 15;
        b.EnchantPts = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ChopMin mismatch")
      {
        a.ChopMin = 10;
        b.ChopMin = 15;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ChopMax mismatch")
      {
        a.ChopMax = 20;
        b.ChopMax = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SlashMin mismatch")
      {
        a.SlashMin = 10;
        b.SlashMin = 15;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SlashMax mismatch")
      {
        a.SlashMax = 20;
        b.SlashMax = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ThrustMin mismatch")
      {
        a.ThrustMin = 10;
        b.ThrustMin = 15;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ThrustMax mismatch")
      {
        a.ThrustMax = 20;
        b.ThrustMax = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("WeaponFlags mismatch")
      {
        a.WeaponFlags = 0x00000020;
        b.WeaponFlags = 0x00000040;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("InventoryIcon mismatch")
      {
        a.InventoryIcon = "icon01.dds";
        b.InventoryIcon = "other_icon.dds";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("EnchantmentID mismatch")
      {
        a.EnchantmentID = "Enchantment_A";
        b.EnchantmentID = "Enchantment_B";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ScriptID mismatch")
      {
        a.ScriptID = "ThisScript";
        b.ScriptID = "ThatScript";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;

    SECTION("default: load record without script")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      WeaponRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "daedric dagger_soultrap" );
      REQUIRE( record.ModelPath == "w\\W_Dagger_daedric.nif" );
      REQUIRE( record.Name == "Seelentrinker" );
      REQUIRE( record.Weight == 9.0f );
      REQUIRE( record.Value == 11000 );
      REQUIRE( record.Type == 0 );
      REQUIRE( record.Health == 700 );
      REQUIRE( record.Speed == 2.5f );
      REQUIRE( record.Reach == 1.0f );
      REQUIRE( record.EnchantPts == 60 );
      REQUIRE( record.ChopMin == 8 );
      REQUIRE( record.ChopMax == 12 );
      REQUIRE( record.SlashMin == 8 );
      REQUIRE( record.SlashMax == 12 );
      REQUIRE( record.ThrustMin == 8 );
      REQUIRE( record.ThrustMax == 12 );
      REQUIRE( record.WeaponFlags == 0x00000001 );
      REQUIRE( record.InventoryIcon == "w\\Tx_Dagger_daedric.tga" );
      REQUIRE( record.EnchantmentID == "we_soultrap_en" );
      REQUIRE( record.ScriptID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with a script")
    {
      const auto data = "WEAP\xCB\0\0\0\0\0\0\0\0\0\0\0NAME\x1C\0\0\0ebony broadsword_Dae_cursed\0MODL\x19\0\0\0w\\W_broadsword_ebony.nif\0FNAM\x15\0\0\0Ebenerz-Breitschwert\0WPDT \0\0\0\0\0\xC0\x41\x98:\0\0\x01\0\x08\x07\0\0\xA0?\0\0\x80?d\0\x04\x1A\x02\x1A\x02\x1A\x01\0\0\0SCRI\x17\0\0\0BILL_MarksDaedraSummon\0ITEX\x1A\0\0\0w\\Tx_Broadsword_ebony.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      WeaponRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "ebony broadsword_Dae_cursed" );
      REQUIRE( record.ModelPath == "w\\W_broadsword_ebony.nif" );
      REQUIRE( record.Name == "Ebenerz-Breitschwert" );
      REQUIRE( record.Weight == 24.0f );
      REQUIRE( record.Value == 15000 );
      REQUIRE( record.Type == 1 );
      REQUIRE( record.Health == 1800 );
      REQUIRE( record.Speed == 1.25f );
      REQUIRE( record.Reach == 1.0f );
      REQUIRE( record.EnchantPts == 100 );
      REQUIRE( record.ChopMin == 4 );
      REQUIRE( record.ChopMax == 26 );
      REQUIRE( record.SlashMin == 2 );
      REQUIRE( record.SlashMax == 26 );
      REQUIRE( record.ThrustMin == 2 );
      REQUIRE( record.ThrustMax == 26 );
      REQUIRE( record.WeaponFlags == 0x00000001 );
      REQUIRE( record.InventoryIcon == "w\\Tx_Broadsword_ebony.tga" );
      REQUIRE( record.EnchantmentID.empty() );
      REQUIRE( record.ScriptID == "BILL_MarksDaedraSummon" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "WEAP\x91\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0FAIL\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\x01\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\xBF\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0FAIL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\x01\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\xB7\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FAIL\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\x01\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\xBE\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no WPDT")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0FAIL \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of WPDT is not 32")
    {
      {
        const auto data = "WEAP\xB3\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT\x1F\0\0\0\0\0\x10\x41\xF8*\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "WEAP\xB5\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT\x21\0\0\0\0\0\x10\x41\xF8*\0\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before WPDT can be read")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "WEAP\xCB\0\0\0\0\0\0\0\0\0\0\0NAME\x1C\0\0\0ebony broadsword_Dae_cursed\0MODL\x19\0\0\0w\\W_broadsword_ebony.nif\0FNAM\x15\0\0\0Ebenerz-Breitschwert\0WPDT \0\0\0\0\0\xC0\x41\x98:\0\0\x01\0\x08\x07\0\0\xA0?\0\0\x80?d\0\x04\x1A\x02\x1A\x02\x1A\x01\0\0\0FAIL\x17\0\0\0BILL_MarksDaedraSummon\0ITEX\x1A\0\0\0w\\Tx_Broadsword_ebony.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple SCRI")
    {
      const auto data = "WEAP\xEA\0\0\0\0\0\0\0\0\0\0\0NAME\x1C\0\0\0ebony broadsword_Dae_cursed\0MODL\x19\0\0\0w\\W_broadsword_ebony.nif\0FNAM\x15\0\0\0Ebenerz-Breitschwert\0WPDT \0\0\0\0\0\xC0\x41\x98:\0\0\x01\0\x08\x07\0\0\xA0?\0\0\x80?d\0\x04\x1A\x02\x1A\x02\x1A\x01\0\0\0SCRI\x17\0\0\0BILL_MarksDaedraSummon\0SCRI\x17\0\0\0BILL_MarksDaedraSummon\0ITEX\x1A\0\0\0w\\Tx_Broadsword_ebony.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "WEAP\xCB\0\0\0\0\0\0\0\0\0\0\0NAME\x1C\0\0\0ebony broadsword_Dae_cursed\0MODL\x19\0\0\0w\\W_broadsword_ebony.nif\0FNAM\x15\0\0\0Ebenerz-Breitschwert\0WPDT \0\0\0\0\0\xC0\x41\x98:\0\0\x01\0\x08\x07\0\0\xA0?\0\0\x80?d\0\x04\x1A\x02\x1A\x02\x1A\x01\0\0\0SCRI\x17\x01\0\0BILL_MarksDaedraSummon\0ITEX\x1A\0\0\0w\\Tx_Broadsword_ebony.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "WEAP\xCB\0\0\0\0\0\0\0\0\0\0\0NAME\x1C\0\0\0ebony broadsword_Dae_cursed\0MODL\x19\0\0\0w\\W_broadsword_ebony.nif\0FNAM\x15\0\0\0Ebenerz-Breitschwert\0WPDT \0\0\0\0\0\xC0\x41\x98:\0\0\x01\0\x08\x07\0\0\xA0?\0\0\x80?d\0\x04\x1A\x02\x1A\x02\x1A\x01\0\0\0SCRI\x57\0\0\0BILL_MarksDaedraSummon\0ITEX\x1A\0\0\0w\\Tx_Broadsword_ebony.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty SCRI")
    {
      const auto data = "WEAP\xB5\0\0\0\0\0\0\0\0\0\0\0NAME\x1C\0\0\0ebony broadsword_Dae_cursed\0MODL\x19\0\0\0w\\W_broadsword_ebony.nif\0FNAM\x15\0\0\0Ebenerz-Breitschwert\0WPDT \0\0\0\0\0\xC0\x41\x98:\0\0\x01\0\x08\x07\0\0\xA0?\0\0\x80?d\0\x04\x1A\x02\x1A\x02\x1A\x01\0\0\0SCRI\x01\0\0\0\0ITEX\x1A\0\0\0w\\Tx_Broadsword_ebony.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ITEX")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0FAIL\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ITEX")
    {
      const auto data = "WEAP\xD4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX > 256")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\x01\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX is beyond stream")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\xAD\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty ITEX")
    {
      const auto data = "WEAP\x9D\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x01\0\0\0\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ENAM")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0FAIL\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ENAM")
    {
      const auto data = "WEAP\xCB\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\0\0\0we_soultrap_en\0ENAM\x0F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ENAM > 256")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x0F\x01\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ENAM is beyond stream")
    {
      const auto data = "WEAP\xB4\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x1F\0\0\0we_soultrap_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty ENAM")
    {
      const auto data = "WEAP\xA6\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0daedric dagger_soultrap\0MODL\x17\0\0\0w\\W_Dagger_daedric.nif\0FNAM\x0E\0\0\0Seelentrinker\0WPDT \0\0\0\0\0\x10\x41\xF8*\0\0\0\0\xBC\x02\0\0 @\0\0\x80?<\0\x08\x0C\x08\x0C\x08\x0C\x01\0\0\0ITEX\x18\0\0\0w\\Tx_Dagger_daedric.tga\0ENAM\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
