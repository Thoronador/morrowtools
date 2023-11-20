/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2022, 2023  Dirk Stolle

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
#include "../../../../lib/mw/records/ArmourRecord.hpp"

TEST_CASE("MWTP::ArmourRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    ArmourRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.ArmourType == 0 );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Value == 0 );
    REQUIRE( record.Health == 0 );
    REQUIRE( record.EnchantmentPoints == 0 );
    REQUIRE( record.ArmourRating == 0 );
    REQUIRE( record.InventoryIcon.empty() );
    REQUIRE( record.BodyParts.empty() );
    REQUIRE( record.ScriptID.empty() );
    REQUIRE( record.EnchantmentID.empty() );
  }

  SECTION("equals")
  {
    ArmourRecord a;
    ArmourRecord b;

    SECTION("equal")
    {
      a.recordID = "ArmourOne";
      a.ModelPath = "a01.nif";

      b.recordID = "ArmourOne";
      b.ModelPath = "a01.nif";

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

      SECTION("ArmourType mismatch")
      {
        a.ArmourType = 1;
        b.ArmourType = 2;

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

      SECTION("Health mismatch")
      {
        a.Health = 150;
        b.Health = 250;

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

      SECTION("ArmourRating mismatch")
      {
        a.ArmourRating = 50;
        b.ArmourRating = 100;

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
    SECTION("default: load record without script")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ArmourRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "bloodworm_helm_unique" );
      REQUIRE( record.ModelPath == "a\\A_Trollbone_Helm.NIF" );
      REQUIRE( record.Name == "Helm des Blutwurms" );
      REQUIRE( record.ArmourType == 0 );
      REQUIRE( record.Weight == 8.0f );
      REQUIRE( record.Value == 34000 );
      REQUIRE( record.Health == 600 );
      REQUIRE( record.EnchantmentPoints == 1500 );
      REQUIRE( record.ArmourRating == 18 );
      REQUIRE( record.InventoryIcon == "a\\tx_Trollbone_helm.tga" );
      REQUIRE( record.BodyParts.size() == 1 );

      REQUIRE( record.BodyParts[0].Index == 1 );
      REQUIRE( record.BodyParts[0].MaleBodyPart == "a_trollbone_helm" );
      REQUIRE( record.BodyParts[0].FemaleBodyPart.empty() );

      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.EnchantmentID == "grip of death" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with female body parts (from plugin)")
    {
      const auto data = "ARMO\xE9\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0mand_aph_rose_gauntlets\0MODL\x1D\0\0\0Mand\\mand_rose_glovesGND.nif\0FNAM\x16\0\0\0Aphrodites Handschuhe\0AODT\x18\0\0\0\x06\0\0\0\0\0@Ap\x17\0\0\x0E\x01\0\0\xA4\x01\0\0:\0\0\0ITEX\x1C\0\0\0Mand\\mand_rose_gauntlet.tga\0INDX\x01\0\0\0\x07\x43NAM\x10\0\0\0mand_rose_glovesINDX\x01\0\0\0\x06\x43NAM\x10\0\0\0mand_rose_gloves"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ArmourRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "mand_aph_rose_gauntlets" );
      REQUIRE( record.ModelPath == "Mand\\mand_rose_glovesGND.nif" );
      REQUIRE( record.Name == "Aphrodites Handschuhe" );
      REQUIRE( record.ArmourType == 6 );
      REQUIRE( record.Weight == 12.0f );
      REQUIRE( record.Value == 6000 );
      REQUIRE( record.Health == 270 );
      REQUIRE( record.EnchantmentPoints == 420 );
      REQUIRE( record.ArmourRating == 58 );
      REQUIRE( record.InventoryIcon == "Mand\\mand_rose_gauntlet.tga" );
      REQUIRE( record.BodyParts.size() == 2 );

      REQUIRE( record.BodyParts[0].Index == 7 );
      REQUIRE( record.BodyParts[0].MaleBodyPart.empty() );
      REQUIRE( record.BodyParts[0].FemaleBodyPart == "mand_rose_gloves" );

      REQUIRE( record.BodyParts[1].Index == 6 );
      REQUIRE( record.BodyParts[1].MaleBodyPart.empty() );
      REQUIRE( record.BodyParts[1].FemaleBodyPart == "mand_rose_gloves" );

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
      const auto data = "ARMO\xDE\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0indoril cuirass\0MODL\x1E\0\0\0a\\A_Indoril_M_Cuirass_GND.nif\0FNAM\x1C\0\0\0Indorilischer Brustharnisch\0SCRI\x11\0\0\0OrdinatorUniform\0AODT\x18\0\0\0\x01\0\0\0\0\0\xD8\x41X\x1B\0\0F\x05\0\0\xB4\0\0\0-\0\0\0ITEX\x19\0\0\0a\\Tx_Indoril_curaiss.tga\0INDX\x01\0\0\0\x03\x42NAM\x11\0\0\0a_indoril_m_chest"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ArmourRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "indoril cuirass" );
      REQUIRE( record.ModelPath == "a\\A_Indoril_M_Cuirass_GND.nif" );
      REQUIRE( record.Name == "Indorilischer Brustharnisch" );
      REQUIRE( record.ArmourType == 1 );
      REQUIRE( record.Weight == 27.0f );
      REQUIRE( record.Value == 7000 );
      REQUIRE( record.Health == 1350 );
      REQUIRE( record.EnchantmentPoints == 180 );
      REQUIRE( record.ArmourRating == 45 );
      REQUIRE( record.InventoryIcon == "a\\Tx_Indoril_curaiss.tga" );
      REQUIRE( record.BodyParts.size() == 1 );

      REQUIRE( record.BodyParts[0].Index == 3 );
      REQUIRE( record.BodyParts[0].MaleBodyPart == "a_indoril_m_chest" );
      REQUIRE( record.BodyParts[0].FemaleBodyPart.empty() );

      REQUIRE( record.ScriptID == "OrdinatorUniform" );
      REQUIRE( record.EnchantmentID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "ARMO\x91\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0FAIL\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\x01\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\xDA\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0FAIL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\x01\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\xD7\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FAIL\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\x01\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\xCF\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no AODT")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0FAIL\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of AODT is not 24")
    {
      {
        const auto data = "ARMO\xCE\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x17\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip ARMO, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "ARMO\xD0\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x19\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip ARMO, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before CTDT can be read")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ITEX")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0FAIL\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ITEXes")
    {
      const auto data = "ARMO\xEF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX > 256")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\x01\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX is beyond stream")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\xC8\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty ITEX")
    {
      const auto data = "ARMO\xB8\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x01\0\0\0\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of INDX is not one")
    {
      {
        const auto data = "ARMO\xCE\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\0\0\0\0BNAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip ARMO, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "ARMO\xD0\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x02\0\0\0\x01\0BNAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip ARMO, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before INDX can be read")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: BNAM without previous INDX")
    {
      const auto data = "ARMO\xC6\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0BNAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of BNAM > 256")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\x01\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: CNAM without previous INDX")
    {
      const auto data = "ARMO\xE0\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0mand_aph_rose_gauntlets\0MODL\x1D\0\0\0Mand\\mand_rose_glovesGND.nif\0FNAM\x16\0\0\0Aphrodites Handschuhe\0AODT\x18\0\0\0\x06\0\0\0\0\0@Ap\x17\0\0\x0E\x01\0\0\xA4\x01\0\0:\0\0\0ITEX\x1C\0\0\0Mand\\mand_rose_gauntlet.tga\0CNAM\x10\0\0\0mand_rose_glovesINDX\x01\0\0\0\x06\x43NAM\x10\0\0\0mand_rose_gloves"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of CNAM > 256")
    {
      const auto data = "ARMO\xE9\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0mand_aph_rose_gauntlets\0MODL\x1D\0\0\0Mand\\mand_rose_glovesGND.nif\0FNAM\x16\0\0\0Aphrodites Handschuhe\0AODT\x18\0\0\0\x06\0\0\0\0\0@Ap\x17\0\0\x0E\x01\0\0\xA4\x01\0\0:\0\0\0ITEX\x1C\0\0\0Mand\\mand_rose_gauntlet.tga\0INDX\x01\0\0\0\x07\x43NAM\x10\x01\0\0mand_rose_glovesINDX\x01\0\0\0\x06\x43NAM\x10\0\0\0mand_rose_gloves"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ENAM")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmFAIL\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ENAMs")
    {
      const auto data = "ARMO\xE5\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\0\0\0grip of death\0ENAM\x0E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ENAM > 256")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x0E\x01\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ENAM is beyond stream")
    {
      const auto data = "ARMO\xCF\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x1E\0\0\0grip of death\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty ENAM")
    {
      const auto data = "ARMO\xC2\0\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0bloodworm_helm_unique\0MODL\x17\0\0\0a\\A_Trollbone_Helm.NIF\0FNAM\x13\0\0\0Helm des Blutwurms\0AODT\x18\0\0\0\0\0\0\0\0\0\0A\xD0\x84\0\0X\x02\0\0\xDC\x05\0\0\x12\0\0\0ITEX\x18\0\0\0a\\tx_Trollbone_helm.tga\0INDX\x01\0\0\0\x01\x42NAM\x10\0\0\0a_trollbone_helmENAM\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "ARMO\xDE\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0indoril cuirass\0MODL\x1E\0\0\0a\\A_Indoril_M_Cuirass_GND.nif\0FNAM\x1C\0\0\0Indorilischer Brustharnisch\0FAIL\x11\0\0\0OrdinatorUniform\0AODT\x18\0\0\0\x01\0\0\0\0\0\xD8\x41X\x1B\0\0F\x05\0\0\xB4\0\0\0-\0\0\0ITEX\x19\0\0\0a\\Tx_Indoril_curaiss.tga\0INDX\x01\0\0\0\x03\x42NAM\x11\0\0\0a_indoril_m_chest"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "ARMO\xDE\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0indoril cuirass\0MODL\x1E\0\0\0a\\A_Indoril_M_Cuirass_GND.nif\0FNAM\x1C\0\0\0Indorilischer Brustharnisch\0SCRI\x11\x01\0\0OrdinatorUniform\0AODT\x18\0\0\0\x01\0\0\0\0\0\xD8\x41X\x1B\0\0F\x05\0\0\xB4\0\0\0-\0\0\0ITEX\x19\0\0\0a\\Tx_Indoril_curaiss.tga\0INDX\x01\0\0\0\x03\x42NAM\x11\0\0\0a_indoril_m_chest"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "ARMO\xDE\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0indoril cuirass\0MODL\x1E\0\0\0a\\A_Indoril_M_Cuirass_GND.nif\0FNAM\x1C\0\0\0Indorilischer Brustharnisch\0SCRI\xD1\0\0\0OrdinatorUniform\0AODT\x18\0\0\0\x01\0\0\0\0\0\xD8\x41X\x1B\0\0F\x05\0\0\xB4\0\0\0-\0\0\0ITEX\x19\0\0\0a\\Tx_Indoril_curaiss.tga\0INDX\x01\0\0\0\x03\x42NAM\x11\0\0\0a_indoril_m_chest"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty SCRI")
    {
      const auto data = "ARMO\xCE\0\0\0\0\0\0\0\0\0\0\0NAME\x10\0\0\0indoril cuirass\0MODL\x1E\0\0\0a\\A_Indoril_M_Cuirass_GND.nif\0FNAM\x1C\0\0\0Indorilischer Brustharnisch\0SCRI\x01\0\0\0\0AODT\x18\0\0\0\x01\0\0\0\0\0\xD8\x41X\x1B\0\0F\x05\0\0\xB4\0\0\0-\0\0\0ITEX\x19\0\0\0a\\Tx_Indoril_curaiss.tga\0INDX\x01\0\0\0\x03\x42NAM\x11\0\0\0a_indoril_m_chest"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ARMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
