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

#include "../../locate_catch.hpp"
#include <sstream>
#include <string_view>
#include "../../../../lib/mw/records/AlchemyPotionRecord.hpp"

TEST_CASE("MWTP::AlchemyPotionRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    AlchemyPotionRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Value == 0 );
    REQUIRE( record.AutoCalc == 0 );
    REQUIRE( record.Enchs.empty() );
    REQUIRE( record.InventoryIcon.empty() );
    REQUIRE( record.ScriptID.empty() );
  }

  SECTION("equals")
  {
    AlchemyPotionRecord a;
    AlchemyPotionRecord b;

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
        a.Name = "Alice's item";
        b.Name = "Bob's item";

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

      SECTION("AutoCalc mismatch")
      {
        a.AutoCalc = 0;
        b.AutoCalc = 1;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("enchantments mismatch")
      {
        a.Enchs.push_back(EnchantmentData());
        b.Enchs.clear();

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

    SECTION("default: load potion with a single effect")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      AlchemyPotionRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "p_slowfall_s" );
      REQUIRE( record.ModelPath == "m\\Misc_Potion_Fresh_01.nif" );
      REQUIRE( record.Name == "Federflugtrank" );

      REQUIRE( record.Weight == 1.0f );
      REQUIRE( record.Value == 35 );
      REQUIRE( record.AutoCalc == 0 );
      REQUIRE( record.Enchs.size() == 1 );

      REQUIRE( record.Enchs[0].EffectID == 0x000B );
      REQUIRE( record.Enchs[0].SkillID == 0xFF );
      REQUIRE( record.Enchs[0].AttributeID == 0xFF );
      REQUIRE( record.Enchs[0].Range == RangeType::Self );
      REQUIRE( record.Enchs[0].Area == 0 );
      REQUIRE( record.Enchs[0].Duration == 15 );
      REQUIRE( record.Enchs[0].MagnitudeMin == 10 );
      REQUIRE( record.Enchs[0].MagnitudeMax == 10 );

      REQUIRE( record.InventoryIcon == "m\\Tx_potion_fresh_01.tga" );
      REQUIRE( record.ScriptID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load potion with multiple effects")
    {
      const auto data = "ALCH\xDD\0\0\0\0\0\0\0\0\0\0\0NAME\x19\0\0\0p_vintagecomberrybrandy1\0MODL \0\0\0n\\Potion_Comberry_brandy_01.NIF\0TEXT\x1C\0\0\0n\\Tx_Comberry_brandy_01.tga\0FNAM\x14\0\0\0Erlesener Weinbrand\0ALDT\x0C\0\0\0\0\0\x80?\xF4\x01\0\0\0\0\0\0ENAM\x18\0\0\0O\0\xFF\0\0\0\0\0\0\0\0\0h\x01\0\0\x1E\0\0\0\x1E\0\0\0ENAM\x18\0\0\0\x11\0\xFF\x03\0\0\0\0\0\0\0\0h\x01\0\0\x1E\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      AlchemyPotionRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "p_vintagecomberrybrandy1" );
      REQUIRE( record.ModelPath == "n\\Potion_Comberry_brandy_01.NIF" );
      REQUIRE( record.Name == "Erlesener Weinbrand" );

      REQUIRE( record.Weight == 1.0f );
      REQUIRE( record.Value == 500 );
      REQUIRE( record.AutoCalc == 0 );
      REQUIRE( record.Enchs.size() == 2 );

      REQUIRE( record.Enchs[0].EffectID == 79 );
      REQUIRE( record.Enchs[0].SkillID == 0xFF );
      REQUIRE( record.Enchs[0].AttributeID == 0x00 );
      REQUIRE( record.Enchs[0].Range == RangeType::Self );
      REQUIRE( record.Enchs[0].Area == 0 );
      REQUIRE( record.Enchs[0].Duration == 360 );
      REQUIRE( record.Enchs[0].MagnitudeMin == 30 );
      REQUIRE( record.Enchs[0].MagnitudeMax == 30 );

      REQUIRE( record.Enchs[1].EffectID == 0x0011 );
      REQUIRE( record.Enchs[1].SkillID == 0xFF );
      REQUIRE( record.Enchs[1].AttributeID == 0x03 );
      REQUIRE( record.Enchs[1].Range == RangeType::Self );
      REQUIRE( record.Enchs[1].Area == 0 );
      REQUIRE( record.Enchs[1].Duration == 360 );
      REQUIRE( record.Enchs[1].MagnitudeMin == 30 );
      REQUIRE( record.Enchs[1].MagnitudeMax == 30 );

      REQUIRE( record.InventoryIcon == "n\\Tx_Comberry_brandy_01.tga" );
      REQUIRE( record.ScriptID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load potion with a script")
    {
      const auto data = "ALCH\xB0\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0SCRI\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      AlchemyPotionRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "p_slowfall_s" );
      REQUIRE( record.ModelPath == "m\\Misc_Potion_Fresh_01.nif" );
      REQUIRE( record.Name == "Federflugtrank" );

      REQUIRE( record.Weight == 1.0f );
      REQUIRE( record.Value == 35 );
      REQUIRE( record.AutoCalc == 0 );
      REQUIRE( record.Enchs.size() == 1 );

      REQUIRE( record.Enchs[0].EffectID == 0x000B );
      REQUIRE( record.Enchs[0].SkillID == 0xFF );
      REQUIRE( record.Enchs[0].AttributeID == 0xFF );
      REQUIRE( record.Enchs[0].Range == RangeType::Self );
      REQUIRE( record.Enchs[0].Area == 0 );
      REQUIRE( record.Enchs[0].Duration == 15 );
      REQUIRE( record.Enchs[0].MagnitudeMin == 10 );
      REQUIRE( record.Enchs[0].MagnitudeMax == 10 );

      REQUIRE( record.InventoryIcon == "m\\Tx_potion_fresh_01.tga" );
      REQUIRE( record.ScriptID == "foo" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "ALCH\x91\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0FAIL\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\x01\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\xAD\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0FAIL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\x01\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\xAF\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty MODL")
    {
      const auto data = "ALCH\x8A\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x01\0\0\0\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no TEXT")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0FAIL\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple TEXT")
    {
      const auto data = "ALCH\xC5\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of TEXT > 256")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\x01\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of TEXT is beyond stream")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\xAD\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty TEXT")
    {
      const auto data = "ALCH\x8C\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x01\0\0\0\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FAIL\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple FNAM")
    {
      const auto data = "ALCH\xBB\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\x01\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\xAF\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty FNAM")
    {
      const auto data = "ALCH\x96\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x01\0\0\0\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ALDT")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0FAIL\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: missing ALDT")
    {
      const auto data = "ALCH\x90\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ALDT")
    {
      const auto data = "ALCH\xB8\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ALDT is not 12")
    {
      {
        const auto data = "ALCH\xA3\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0B\0\0\0\0\0\x80?#\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "ALCH\xA5\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0D\0\0\0\0\0\x80?#\0\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before ALDT can be read")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: incomplete ENAM")
    {
      const auto data = "ALCH\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "ALCH\xB0\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0FAIL\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple SCRI")
    {
      const auto data = "ALCH\xBC\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0SCRI\x04\0\0\0foo\0SCRI\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "ALCH\xB0\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0SCRI\x04\x01\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "ALCH\xB0\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0SCRI\x0F\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty SCRI")
    {
      const auto data = "ALCH\xAD\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0p_slowfall_s\0MODL\x1B\0\0\0m\\Misc_Potion_Fresh_01.nif\0TEXT\x19\0\0\0m\\Tx_potion_fresh_01.tga\0FNAM\x0F\0\0\0Federflugtrank\0ALDT\x0C\0\0\0\0\0\x80?#\0\0\0\0\0\0\0ENAM\x18\0\0\0\x0B\0\xFF\xFF\0\0\0\0\0\0\0\0\x0F\0\0\0\x0A\0\0\0\x0A\0\0\0SCRI\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
