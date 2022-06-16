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

#include "../../../locate_catch.hpp"
#include <sstream>
#include <string_view>
#include "../../../../../mw/base/records/IngredientRecord.hpp"

TEST_CASE("MWTP::IngredientRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    IngredientRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Value == 0 );
    REQUIRE( record.EffectID[0] == 0 );
    REQUIRE( record.EffectID[1] == 0 );
    REQUIRE( record.EffectID[2] == 0 );
    REQUIRE( record.EffectID[3] == 0 );
    REQUIRE( record.SkillID[0] == 0 );
    REQUIRE( record.SkillID[1] == 0 );
    REQUIRE( record.SkillID[2] == 0 );
    REQUIRE( record.SkillID[3] == 0 );
    REQUIRE( record.AttributeID[0] == 0 );
    REQUIRE( record.AttributeID[1] == 0 );
    REQUIRE( record.AttributeID[2] == 0 );
    REQUIRE( record.AttributeID[3] == 0 );
    REQUIRE( record.InventoryIcon.empty() );
    REQUIRE( record.ScriptID.empty() );
  }

  SECTION("equals")
  {
    IngredientRecord a;
    IngredientRecord b;

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

      SECTION("EffectID_0 mismatch")
      {
        a.EffectID[0] = 0;
        b.EffectID[0] = 1;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("EffectID_1 mismatch")
      {
        a.EffectID[1] = 0;
        b.EffectID[1] = 1;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("EffectID_2 mismatch")
      {
        a.EffectID[2] = 0;
        b.EffectID[2] = 1;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("EffectID_3 mismatch")
      {
        a.EffectID[3] = 0;
        b.EffectID[3] = 1;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SkillID 0 mismatch")
      {
        a.SkillID[0] = 0;
        b.SkillID[0] = 1;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SkillID 1 mismatch")
      {
        a.SkillID[1] = 0;
        b.SkillID[1] = 1;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SkillID 2 mismatch")
      {
        a.SkillID[2] = 0;
        b.SkillID[2] = 1;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SkillID 3 mismatch")
      {
        a.SkillID[3] = 0;
        b.SkillID[3] = 1;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AttributeID 0 mismatch")
      {
        a.AttributeID[0] = 0;
        b.AttributeID[0] = 1;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AttributeID 1 mismatch")
      {
        a.AttributeID[1] = 0;
        b.AttributeID[1] = 1;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AttributeID 2 mismatch")
      {
        a.AttributeID[2] = 0;
        b.AttributeID[2] = 1;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AttributeID 3 mismatch")
      {
        a.AttributeID[3] = 0;
        b.AttributeID[3] = 1;

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

    SECTION("default: load ingredient")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      IngredientRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "ingred_comberry_01" );
      REQUIRE( record.ModelPath == "n\\Ingred_Comberry_01.nif" );
      REQUIRE( record.Name == "Cohmbeere" );

      REQUIRE( record.Weight == 0.1f );
      REQUIRE( record.Value == 2 );
      REQUIRE( record.EffectID[0] == 20 );
      REQUIRE( record.EffectID[1] == 76 );
      REQUIRE( record.EffectID[2] == 4 );
      REQUIRE( record.EffectID[3] == 68 );
      REQUIRE( record.SkillID[0] == 0 );
      REQUIRE( record.SkillID[1] == 0 );
      REQUIRE( record.SkillID[2] == 0 );
      REQUIRE( record.SkillID[3] == 0 );
      REQUIRE( record.AttributeID[0] == 0 );
      REQUIRE( record.AttributeID[1] == 0 );
      REQUIRE( record.AttributeID[2] == 0 );
      REQUIRE( record.AttributeID[3] == 0 );
      REQUIRE( record.InventoryIcon == "n\\Tx_comberry_01.tga" );
      REQUIRE( record.ScriptID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load ingredient with attribute-related effects")
    {
      const auto data = "INGR\xA9\0\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0ingred_heather_01\0MODL\x18\0\0\0n\\Ingred_Heather_01.nif\0FNAM\x0B\0\0\0Heidekraut\0IRDT8\0\0\0\xCD\xCC\xCC=\x01\0\0\0J\0\0\0\x08\0\0\0\x11\0\0\0\x11\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x06\0\0\0\0\0\0\0\x04\0\0\0\x06\0\0\0ITEX\x14\0\0\0n\\Tx_heather_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      IngredientRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "ingred_heather_01" );
      REQUIRE( record.ModelPath == "n\\Ingred_Heather_01.nif" );
      REQUIRE( record.Name == "Heidekraut" );

      REQUIRE( record.Weight == 0.1f );
      REQUIRE( record.Value == 1 );
      REQUIRE( record.EffectID[0] == 74 );
      REQUIRE( record.EffectID[1] == 8 );
      REQUIRE( record.EffectID[2] == 17 );
      REQUIRE( record.EffectID[3] == 17 );
      REQUIRE( record.SkillID[0] == 0 );
      REQUIRE( record.SkillID[1] == 0 );
      REQUIRE( record.SkillID[2] == 0 );
      REQUIRE( record.SkillID[3] == 0 );
      REQUIRE( record.AttributeID[0] == 6 );
      REQUIRE( record.AttributeID[1] == 0 );
      REQUIRE( record.AttributeID[2] == 4 );
      REQUIRE( record.AttributeID[3] == 6 );
      REQUIRE( record.InventoryIcon == "n\\Tx_heather_01.tga" );
      REQUIRE( record.ScriptID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load ingredient with a script")
    {
      const auto data = "INGR\xE0\0\0\0\0\0\0\0\0\0\0\0NAME\x1F\0\0\0ingred_cursed_daedras_heart_01\0MODL\x1D\0\0\0n\\Ingred_Daedra_heart_01.NIF\0FNAM\x0C\0\0\0Daedra-Herz\0IRDT8\0\0\0\0\0\x80?\xC8\0\0\0L\0\0\0O\0\0\0\x11\0\0\0+\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x05\0\0\0\x03\0\0\0\xFF\xFF\xFF\xFFSCRI\x17\0\0\0BILL_MarksDaedraSummon\0ITEX\x19\0\0\0n\\Tx_daedra_heart_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      IngredientRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "ingred_cursed_daedras_heart_01" );
      REQUIRE( record.ModelPath == "n\\Ingred_Daedra_heart_01.NIF" );
      REQUIRE( record.Name == "Daedra-Herz" );

      REQUIRE( record.Weight == 1.0f );
      REQUIRE( record.Value == 200 );
      REQUIRE( record.EffectID[0] == 76 );
      REQUIRE( record.EffectID[1] == 79 );
      REQUIRE( record.EffectID[2] == 17 );
      REQUIRE( record.EffectID[3] == 43 );
      REQUIRE( record.SkillID[0] == 0xFFFFFFFF );
      REQUIRE( record.SkillID[1] == 0 );
      REQUIRE( record.SkillID[2] == 0 );
      REQUIRE( record.SkillID[3] == 0xFFFFFFFF );
      REQUIRE( record.AttributeID[0] == 0xFFFFFFFF );
      REQUIRE( record.AttributeID[1] == 5 );
      REQUIRE( record.AttributeID[2] == 3 );
      REQUIRE( record.AttributeID[3] == 0xFFFFFFFF );
      REQUIRE( record.InventoryIcon == "n\\Tx_daedra_heart_01.tga" );
      REQUIRE( record.ScriptID == "BILL_MarksDaedraSummon" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "INGR\x91\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0FAIL\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0NAME\x13\x01\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0NAME\xAD\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0FAIL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\x01\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\xAD\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FAIL\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\x01\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\xAD\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no IRDT")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0FAIL8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of IRDT is not 56")
    {
      {
        const auto data = "INGR\xAA\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT7\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "INGR\xAC\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT9\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before IRDT can be read")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "INGR\xE0\0\0\0\0\0\0\0\0\0\0\0NAME\x1F\0\0\0ingred_cursed_daedras_heart_01\0MODL\x1D\0\0\0n\\Ingred_Daedra_heart_01.NIF\0FNAM\x0C\0\0\0Daedra-Herz\0IRDT8\0\0\0\0\0\x80?\xC8\0\0\0L\0\0\0O\0\0\0\x11\0\0\0+\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x05\0\0\0\x03\0\0\0\xFF\xFF\xFF\xFFGONE\x17\0\0\0BILL_MarksDaedraSummon\0ITEX\x19\0\0\0n\\Tx_daedra_heart_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple SCRI")
    {
      const auto data = "INGR\xFF\0\0\0\0\0\0\0\0\0\0\0NAME\x1F\0\0\0ingred_cursed_daedras_heart_01\0MODL\x1D\0\0\0n\\Ingred_Daedra_heart_01.NIF\0FNAM\x0C\0\0\0Daedra-Herz\0IRDT8\0\0\0\0\0\x80?\xC8\0\0\0L\0\0\0O\0\0\0\x11\0\0\0+\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x05\0\0\0\x03\0\0\0\xFF\xFF\xFF\xFFSCRI\x17\0\0\0BILL_MarksDaedraSummon\0SCRI\x17\0\0\0BILL_MarksDaedraSummon\0ITEX\x19\0\0\0n\\Tx_daedra_heart_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "INGR\xE0\0\0\0\0\0\0\0\0\0\0\0NAME\x1F\0\0\0ingred_cursed_daedras_heart_01\0MODL\x1D\0\0\0n\\Ingred_Daedra_heart_01.NIF\0FNAM\x0C\0\0\0Daedra-Herz\0IRDT8\0\0\0\0\0\x80?\xC8\0\0\0L\0\0\0O\0\0\0\x11\0\0\0+\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x05\0\0\0\x03\0\0\0\xFF\xFF\xFF\xFFSCRI\x17\x01\0\0BILL_MarksDaedraSummon\0ITEX\x19\0\0\0n\\Tx_daedra_heart_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "INGR\xE0\0\0\0\0\0\0\0\0\0\0\0NAME\x1F\0\0\0ingred_cursed_daedras_heart_01\0MODL\x1D\0\0\0n\\Ingred_Daedra_heart_01.NIF\0FNAM\x0C\0\0\0Daedra-Herz\0IRDT8\0\0\0\0\0\x80?\xC8\0\0\0L\0\0\0O\0\0\0\x11\0\0\0+\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x05\0\0\0\x03\0\0\0\xFF\xFF\xFF\xFFSCRI\xAD\0\0\0BILL_MarksDaedraSummon\0ITEX\x19\0\0\0n\\Tx_daedra_heart_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty SCRI")
    {
      const auto data = "INGR\xCA\0\0\0\0\0\0\0\0\0\0\0NAME\x1F\0\0\0ingred_cursed_daedras_heart_01\0MODL\x1D\0\0\0n\\Ingred_Daedra_heart_01.NIF\0FNAM\x0C\0\0\0Daedra-Herz\0IRDT8\0\0\0\0\0\x80?\xC8\0\0\0L\0\0\0O\0\0\0\x11\0\0\0+\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x05\0\0\0\x03\0\0\0\xFF\xFF\xFF\xFFSCRI\x01\0\0\0\0ITEX\x19\0\0\0n\\Tx_daedra_heart_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ITEX")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0FAIL\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: missing ITEX")
    {
      const auto data = "INGR\x8E\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ITEX")
    {
      const auto data = "INGR\xC8\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0ITEX\x15\0\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX > 256")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x15\x01\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX is beyond stream")
    {
      const auto data = "INGR\xAB\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x35\0\0\0n\\Tx_comberry_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty ITEX")
    {
      const auto data = "INGR\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0ingred_comberry_01\0MODL\x19\0\0\0n\\Ingred_Comberry_01.nif\0FNAM\x0A\0\0\0Cohmbeere\0IRDT8\0\0\0\xCD\xCC\xCC=\x02\0\0\0\x14\0\0\0L\0\0\0\x04\0\0\0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0ITEX\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
