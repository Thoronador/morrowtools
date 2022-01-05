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
#include "../../../../../mw/base/records/ClothingRecord.hpp"

TEST_CASE("MWTP::ClothingRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    ClothingRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.ClothingType == 0 );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Value == 0 );
    REQUIRE( record.EnchantmentPoints == 0 );
    REQUIRE( record.InventoryIcon.empty() );
    REQUIRE( record.BodyParts.empty() );
    REQUIRE( record.ScriptID.empty() );
    REQUIRE( record.EnchantmentID.empty() );
  }

  SECTION("equals")
  {
    ClothingRecord a;
    ClothingRecord b;

    SECTION("equal")
    {
      a.recordID = "ClothingOne";
      a.ModelPath = "item01.nif";

      b.recordID = "ClothingOne";
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
        a.Name = "Alice's pants";
        b.Name = "Bob's pants";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ClothingType mismatch")
      {
        a.ClothingType = 1;
        b.ClothingType = 2;

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

      SECTION("EnchantmentPoints mismatch")
      {
        a.EnchantmentPoints = 15;
        b.EnchantmentPoints = 25;

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

      SECTION("BodyParts mismatch")
      {
        a.BodyParts.clear();
        b.BodyParts.push_back(BodyPartAssociation());

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

      SECTION("EnchantmentID mismatch")
      {
        a.EnchantmentID = "ThisOne";
        b.EnchantmentID = "ThatOne";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;

    SECTION("default: load record without body parts and script")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ClothingRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "ring_shashev_unique" );
      REQUIRE( record.ModelPath == "c\\C_Ring_exquisite_1.NIF" );
      REQUIRE( record.Name == "Shashevs Ring" );
      REQUIRE( record.ClothingType == 8 );
      REQUIRE( record.Weight == 0.1f );
      REQUIRE( record.Value == 22000 );
      REQUIRE( record.EnchantmentPoints == 300 );
      REQUIRE( record.InventoryIcon == "c\\tx_ring_exquisite01.tga" );
      REQUIRE( record.BodyParts.empty() );
      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.EnchantmentID == "devil's bite_en" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with some body parts")
    {
      const auto data = "CLOT\x9B\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0common_shirt_03_c\0MODL\x1E\0\0\0c\\C_M_Shirt_Common_3c_GND.NIF\0FNAM\x12\0\0\0Gew\xF6hnliches Hemd\0CTDT\x0C\0\0\0\x02\0\0\0\0\0\0@\x04\0\x14\0ITEX\x17\0\0\0c\\tx_shirtcomm_3_c.tga\0INDX\x01\0\0\0\x03\x42NAM\x15\0\0\0c_m_shirt_common_3c_cINDX\x01\0\0\0\x0E\x42NAM\x16\0\0\0c_m_shirt_common_3c_uaINDX\x01\0\0\0\x0D\x42NAM\x16\0\0\0c_m_shirt_common_3c_uaINDX\x01\0\0\0\x0C\x42NAM\x16\0\0\0c_m_shirt_common_3c_faINDX\x01\0\0\0\x0B\x42NAM\x16\0\0\0c_m_shirt_common_3c_faINDX\x01\0\0\0\x09\x42NAM\x15\0\0\0c_m_shirt_common_3c_wINDX\x01\0\0\0\x08\x42NAM\x15\0\0\0c_m_shirt_common_3c_w"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ClothingRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "common_shirt_03_c" );
      REQUIRE( record.ModelPath == "c\\C_M_Shirt_Common_3c_GND.NIF" );
      REQUIRE( record.Name == "Gew\xF6hnliches Hemd" );
      REQUIRE( record.ClothingType == 2 );
      REQUIRE( record.Weight == 2.0f );
      REQUIRE( record.Value == 4 );
      REQUIRE( record.EnchantmentPoints == 20 );
      REQUIRE( record.InventoryIcon == "c\\tx_shirtcomm_3_c.tga" );
      REQUIRE( record.BodyParts.size() == 7 );

      REQUIRE( record.BodyParts[0].Index == 3 );
      REQUIRE( record.BodyParts[0].MaleBodyPart == "c_m_shirt_common_3c_c" );
      REQUIRE( record.BodyParts[0].FemaleBodyPart.empty() );

      REQUIRE( record.BodyParts[1].Index == 14 );
      REQUIRE( record.BodyParts[1].MaleBodyPart == "c_m_shirt_common_3c_ua" );
      REQUIRE( record.BodyParts[1].FemaleBodyPart.empty() );

      REQUIRE( record.BodyParts[2].Index == 13 );
      REQUIRE( record.BodyParts[2].MaleBodyPart == "c_m_shirt_common_3c_ua" );
      REQUIRE( record.BodyParts[2].FemaleBodyPart.empty() );

      REQUIRE( record.BodyParts[3].Index == 12 );
      REQUIRE( record.BodyParts[3].MaleBodyPart == "c_m_shirt_common_3c_fa" );
      REQUIRE( record.BodyParts[3].FemaleBodyPart.empty() );

      REQUIRE( record.BodyParts[4].Index == 11 );
      REQUIRE( record.BodyParts[4].MaleBodyPart == "c_m_shirt_common_3c_fa" );
      REQUIRE( record.BodyParts[4].FemaleBodyPart.empty() );

      REQUIRE( record.BodyParts[5].Index == 9 );
      REQUIRE( record.BodyParts[5].MaleBodyPart == "c_m_shirt_common_3c_w" );
      REQUIRE( record.BodyParts[5].FemaleBodyPart.empty() );

      REQUIRE( record.BodyParts[6].Index == 8 );
      REQUIRE( record.BodyParts[6].MaleBodyPart == "c_m_shirt_common_3c_w" );
      REQUIRE( record.BodyParts[6].FemaleBodyPart.empty() );

      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.EnchantmentID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with female body parts (from plugin)")
    {
      const auto data = "CLOT\xDA\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0mand_divinerglovesfinal\0MODL\x1D\0\0\0Mand\\mand_diviner_gloGND.nif\0FNAM\x15\0\0\0G\xF6ttliche Handschuhe\0CTDT\x0C\0\0\0\x06\0\0\0\0\0\x80?P\0x\0ITEX\x1A\0\0\0Mand\\mand_diviner_glo.tga\0INDX\x01\0\0\0\x07\x43NAM\x10\0\0\0mand_diviner_gloINDX\x01\0\0\0\x06\x43NAM\x10\0\0\0mand_diviner_glo"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ClothingRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "mand_divinerglovesfinal" );
      REQUIRE( record.ModelPath == "Mand\\mand_diviner_gloGND.nif" );
      REQUIRE( record.Name == "G\xF6ttliche Handschuhe" );
      REQUIRE( record.ClothingType == 6 );
      REQUIRE( record.Weight == 1.0f );
      REQUIRE( record.Value == 80 );
      REQUIRE( record.EnchantmentPoints == 120 );
      REQUIRE( record.InventoryIcon == "Mand\\mand_diviner_glo.tga" );
      REQUIRE( record.BodyParts.size() == 2 );

      REQUIRE( record.BodyParts[0].Index == 7 );
      REQUIRE( record.BodyParts[0].MaleBodyPart.empty() );
      REQUIRE( record.BodyParts[0].FemaleBodyPart == "mand_diviner_glo" );

      REQUIRE( record.BodyParts[1].Index == 6 );
      REQUIRE( record.BodyParts[1].MaleBodyPart.empty() );
      REQUIRE( record.BodyParts[1].FemaleBodyPart == "mand_diviner_glo" );

      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.EnchantmentID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with a script")
    {
      const auto data = "CLOT\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Daedric_special01\0MODL\x16\0\0\0c\\Amulet_Madstone.NIF\0FNAM\x14\0\0\0Amulett von Tel Fyr\0CTDT\x0C\0\0\0\x09\0\0\0\0\0@@\x1C\x0C\x96\0SCRI\x13\0\0\0amuletTelFyrScript\0ITEX\x19\0\0\0c\\tx_amulet_Madstone.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ClothingRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "Daedric_special01" );
      REQUIRE( record.ModelPath == "c\\Amulet_Madstone.NIF" );
      REQUIRE( record.Name == "Amulett von Tel Fyr" );
      REQUIRE( record.ClothingType == 9 );
      REQUIRE( record.Weight == 3.0f );
      REQUIRE( record.Value == 3100 );
      REQUIRE( record.EnchantmentPoints == 150 );
      REQUIRE( record.InventoryIcon == "c\\tx_amulet_Madstone.tga" );
      REQUIRE( record.BodyParts.empty() );
      REQUIRE( record.ScriptID == "amuletTelFyrScript" );
      REQUIRE( record.EnchantmentID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "CLOT\x91\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0FAIL\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\x01\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\xA4\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0FAIL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\x01\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\xA9\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FAIL\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\x01\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\xAE\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no CTDT")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0FAIL\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of CTDT is not 12")
    {
      {
        const auto data = "CLOT\xA0\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0B\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CLOT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ClothingRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "CLOT\xA2\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0D\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01\0ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CLOT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ClothingRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before CTDT can be read")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ITEX")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01NONE\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ITEXes")
    {
      const auto data = "CLOT\xC3\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX > 256")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\x01\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX is beyond stream")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\xA1\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty ITEX")
    {
      const auto data = "CLOT\x88\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x01\0\0\0\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of INDX is not one")
    {
      {
        const auto data = "CLOT\x9A\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0common_shirt_03_c\0MODL\x1E\0\0\0c\\C_M_Shirt_Common_3c_GND.NIF\0FNAM\x12\0\0\0Gew\xF6hnliches Hemd\0CTDT\x0C\0\0\0\x02\0\0\0\0\0\0@\x04\0\x14\0ITEX\x17\0\0\0c\\tx_shirtcomm_3_c.tga\0INDX\0\0\0\0BNAM\x15\0\0\0c_m_shirt_common_3c_cINDX\x01\0\0\0\x0E\x42NAM\x16\0\0\0c_m_shirt_common_3c_uaINDX\x01\0\0\0\x0D\x42NAM\x16\0\0\0c_m_shirt_common_3c_uaINDX\x01\0\0\0\x0C\x42NAM\x16\0\0\0c_m_shirt_common_3c_faINDX\x01\0\0\0\x0B\x42NAM\x16\0\0\0c_m_shirt_common_3c_faINDX\x01\0\0\0\x09\x42NAM\x15\0\0\0c_m_shirt_common_3c_wINDX\x01\0\0\0\x08\x42NAM\x15\0\0\0c_m_shirt_common_3c_w"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CLOT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ClothingRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "CLOT\x9C\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0common_shirt_03_c\0MODL\x1E\0\0\0c\\C_M_Shirt_Common_3c_GND.NIF\0FNAM\x12\0\0\0Gew\xF6hnliches Hemd\0CTDT\x0C\0\0\0\x02\0\0\0\0\0\0@\x04\0\x14\0ITEX\x17\0\0\0c\\tx_shirtcomm_3_c.tga\0INDX\x02\0\0\0\x03\0BNAM\x15\0\0\0c_m_shirt_common_3c_cINDX\x01\0\0\0\x0E\x42NAM\x16\0\0\0c_m_shirt_common_3c_uaINDX\x01\0\0\0\x0D\x42NAM\x16\0\0\0c_m_shirt_common_3c_uaINDX\x01\0\0\0\x0C\x42NAM\x16\0\0\0c_m_shirt_common_3c_faINDX\x01\0\0\0\x0B\x42NAM\x16\0\0\0c_m_shirt_common_3c_faINDX\x01\0\0\0\x09\x42NAM\x15\0\0\0c_m_shirt_common_3c_wINDX\x01\0\0\0\x08\x42NAM\x15\0\0\0c_m_shirt_common_3c_w"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CLOT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ClothingRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before INDX can be read")
    {
      const auto data = "CLOT\x9B\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0common_shirt_03_c\0MODL\x1E\0\0\0c\\C_M_Shirt_Common_3c_GND.NIF\0FNAM\x12\0\0\0Gew\xF6hnliches Hemd\0CTDT\x0C\0\0\0\x02\0\0\0\0\0\0@\x04\0\x14\0ITEX\x17\0\0\0c\\tx_shirtcomm_3_c.tga\0INDX\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: BNAM without previous INDX")
    {
      const auto data = "CLOT\x92\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0common_shirt_03_c\0MODL\x1E\0\0\0c\\C_M_Shirt_Common_3c_GND.NIF\0FNAM\x12\0\0\0Gew\xF6hnliches Hemd\0CTDT\x0C\0\0\0\x02\0\0\0\0\0\0@\x04\0\x14\0ITEX\x17\0\0\0c\\tx_shirtcomm_3_c.tga\0BNAM\x15\0\0\0c_m_shirt_common_3c_cINDX\x01\0\0\0\x0E\x42NAM\x16\0\0\0c_m_shirt_common_3c_uaINDX\x01\0\0\0\x0D\x42NAM\x16\0\0\0c_m_shirt_common_3c_uaINDX\x01\0\0\0\x0C\x42NAM\x16\0\0\0c_m_shirt_common_3c_faINDX\x01\0\0\0\x0B\x42NAM\x16\0\0\0c_m_shirt_common_3c_faINDX\x01\0\0\0\x09\x42NAM\x15\0\0\0c_m_shirt_common_3c_wINDX\x01\0\0\0\x08\x42NAM\x15\0\0\0c_m_shirt_common_3c_w"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of BNAM > 256")
    {
      const auto data = "CLOT\x9B\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0common_shirt_03_c\0MODL\x1E\0\0\0c\\C_M_Shirt_Common_3c_GND.NIF\0FNAM\x12\0\0\0Gew\xF6hnliches Hemd\0CTDT\x0C\0\0\0\x02\0\0\0\0\0\0@\x04\0\x14\0ITEX\x17\0\0\0c\\tx_shirtcomm_3_c.tga\0INDX\x01\0\0\0\x03\x42NAM\x15\0\0\0c_m_shirt_common_3c_cINDX\x01\0\0\0\x0E\x42NAM\x16\x01\0\0c_m_shirt_common_3c_uaINDX\x01\0\0\0\x0D\x42NAM\x16\0\0\0c_m_shirt_common_3c_uaINDX\x01\0\0\0\x0C\x42NAM\x16\0\0\0c_m_shirt_common_3c_faINDX\x01\0\0\0\x0B\x42NAM\x16\0\0\0c_m_shirt_common_3c_faINDX\x01\0\0\0\x09\x42NAM\x15\0\0\0c_m_shirt_common_3c_wINDX\x01\0\0\0\x08\x42NAM\x15\0\0\0c_m_shirt_common_3c_w"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: CNAM without previous INDX")
    {
      const auto data = "CLOT\xD1\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0mand_divinerglovesfinal\0MODL\x1D\0\0\0Mand\\mand_diviner_gloGND.nif\0FNAM\x15\0\0\0G\xF6ttliche Handschuhe\0CTDT\x0C\0\0\0\x06\0\0\0\0\0\x80?P\0x\0ITEX\x1A\0\0\0Mand\\mand_diviner_glo.tga\0CNAM\x10\0\0\0mand_diviner_gloINDX\x01\0\0\0\x06\x43NAM\x10\0\0\0mand_diviner_glo"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of CNAM > 256")
    {
      const auto data = "CLOT\xDA\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0mand_divinerglovesfinal\0MODL\x1D\0\0\0Mand\\mand_diviner_gloGND.nif\0FNAM\x15\0\0\0G\xF6ttliche Handschuhe\0CTDT\x0C\0\0\0\x06\0\0\0\0\0\x80?P\0x\0ITEX\x1A\0\0\0Mand\\mand_diviner_glo.tga\0INDX\x01\0\0\0\x07\x43NAM\x10\x01\0\0mand_diviner_gloINDX\x01\0\0\0\x06\x43NAM\x10\0\0\0mand_diviner_glo"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ENAM")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0FAIL\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ENAMs")
    {
      const auto data = "CLOT\xB9\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\0\0\0devil's bite_en\0ENAM\x10\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ENAM > 256")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x10\x01\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ENAM is beyond stream")
    {
      const auto data = "CLOT\xA1\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x20\0\0\0devil's bite_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty ENAM")
    {
      const auto data = "CLOT\x92\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0ring_shashev_unique\0MODL\x19\0\0\0c\\C_Ring_exquisite_1.NIF\0FNAM\x0E\0\0\0Shashevs Ring\0CTDT\x0C\0\0\0\x08\0\0\0\xCD\xCC\xCC=\xF0U,\x01ITEX\x1A\0\0\0c\\tx_ring_exquisite01.tga\0ENAM\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "CLOT\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Daedric_special01\0MODL\x16\0\0\0c\\Amulet_Madstone.NIF\0FNAM\x14\0\0\0Amulett von Tel Fyr\0CTDT\x0C\0\0\0\x09\0\0\0\0\0@@\x1C\x0C\x96\0FAIL\x13\0\0\0amuletTelFyrScript\0ITEX\x19\0\0\0c\\tx_amulet_Madstone.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple SCRIs")
    {
      const auto data = "CLOT\xBF\0\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Daedric_special01\0MODL\x16\0\0\0c\\Amulet_Madstone.NIF\0FNAM\x14\0\0\0Amulett von Tel Fyr\0CTDT\x0C\0\0\0\x09\0\0\0\0\0@@\x1C\x0C\x96\0SCRI\x13\0\0\0amuletTelFyrScript\0SCRI\x13\0\0\0amuletTelFyrScript\0ITEX\x19\0\0\0c\\tx_amulet_Madstone.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "CLOT\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Daedric_special01\0MODL\x16\0\0\0c\\Amulet_Madstone.NIF\0FNAM\x14\0\0\0Amulett von Tel Fyr\0CTDT\x0C\0\0\0\x09\0\0\0\0\0@@\x1C\x0C\x96\0SCRI\x13\x01\0\0amuletTelFyrScript\0ITEX\x19\0\0\0c\\tx_amulet_Madstone.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "CLOT\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Daedric_special01\0MODL\x16\0\0\0c\\Amulet_Madstone.NIF\0FNAM\x14\0\0\0Amulett von Tel Fyr\0CTDT\x0C\0\0\0\x09\0\0\0\0\0@@\x1C\x0C\x96\0SCRI\x53\0\0\0amuletTelFyrScript\0ITEX\x19\0\0\0c\\tx_amulet_Madstone.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty SCRI")
    {
      const auto data = "CLOT\x92\0\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Daedric_special01\0MODL\x16\0\0\0c\\Amulet_Madstone.NIF\0FNAM\x14\0\0\0Amulett von Tel Fyr\0CTDT\x0C\0\0\0\x09\0\0\0\0\0@@\x1C\x0C\x96\0SCRI\x01\0\0\0\0ITEX\x19\0\0\0c\\tx_amulet_Madstone.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CLOT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ClothingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
