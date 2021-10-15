/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021  Thoronador

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
#include "../../../../../sr/base/records/WeaponRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("WeaponRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    WeaponRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.unknownVMAD.isPresent() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE_FALSE( record.unknownMODS.isPresent() );
    REQUIRE( record.enchantingFormID == 0 );
    REQUIRE_FALSE( record.enchantmentAmount.has_value() );
    REQUIRE( record.equipTypeFormID == 0 );
    REQUIRE( record.blockBashImpactDataSetFormID == 0 );
    REQUIRE( record.alternateBlockMaterialFormID == 0 );
    REQUIRE( record.keywords.empty() );
    REQUIRE_FALSE( record.description.isPresent() );
    REQUIRE( record.unknownNNAM.empty() );
    REQUIRE( record.impactDataSetFormID == 0 );
    REQUIRE( record.firstPersonModelObjectFormID == 0 );
    REQUIRE( record.attackSound2DFormID == 0 );
    REQUIRE( record.attackLoopSoundFormID == 0 );
    REQUIRE( record.attackFailSoundFormID == 0 );
    REQUIRE( record.idleSoundFormID == 0 );
    REQUIRE( record.equipSoundFormID == 0 );
    REQUIRE( record.unequipSoundFormID == 0 );
    REQUIRE( record.attackSoundFormID == 0 );
    REQUIRE( record.value == 0 );
    REQUIRE( record.weight == 0.0f );
    REQUIRE( record.baseDamage == 0 );
    for (unsigned int i = 0; i < 100; ++i)
    {
      REQUIRE( record.unknownDNAM[i] == 0 );
    }
    for (unsigned int i = 0; i < 16; ++i)
    {
      REQUIRE( record.unknownCRDT[i] == 0 );
    }
    REQUIRE( record.unknownVNAM == 0 );
    REQUIRE_FALSE( record.unknownCNAM.has_value() );
  }

  SECTION("equals")
  {
    WeaponRecord a;
    WeaponRecord b;

    SECTION("equal")
    {
      a.editorID = "foo";
      b.editorID = "foo";

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("editorID mismatch")
      {
        a.editorID = "foo";
        b.editorID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("VMAD mismatch")
      {
        a.unknownVMAD.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownVMAD.setPresence(false);
        b.unknownVMAD.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("OBND mismatch")
      {
        a.unknownOBND[11] = 12;
        b.unknownOBND[11] = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("model path mismatch")
      {
        a.modelPath = "foo\\bar.nif";
        b.modelPath = "foo2\\bar3.nif";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MODT mismatch")
      {
        a.unknownMODT.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownMODT.setPresence(false);
        b.unknownMODT.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MODS mismatch")
      {
        a.unknownMODS.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownMODS.setPresence(false);
        b.unknownMODS.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("enchantingFormID mismatch")
      {
        a.enchantingFormID = 0x01234567;
        b.enchantingFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("EAMT mismatch")
      {
        a.enchantmentAmount = 0;
        b.enchantmentAmount.reset();

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.enchantmentAmount.reset();
        b.enchantmentAmount = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.enchantmentAmount = 1;
        b.enchantmentAmount = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("equipTypeFormID mismatch")
      {
        a.equipTypeFormID = 0x01234567;
        b.equipTypeFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("blockBashImpactDataSetFormID mismatch")
      {
        a.blockBashImpactDataSetFormID = 0x01234567;
        b.blockBashImpactDataSetFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("alternateBlockMaterialFormID mismatch")
      {
        a.alternateBlockMaterialFormID = 0x01234567;
        b.alternateBlockMaterialFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("keywords mismatch")
      {
        a.keywords.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.keywords.push_back(0x01234567);
        b.keywords.push_back(0x09ABCDEF);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("description mismatch")
      {
        a.description = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NNAM mismatch")
      {
        a.unknownNNAM = "foo";
        b.unknownNNAM = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("impactDataSetFormID mismatch")
      {
        a.impactDataSetFormID = 0x01234567;
        b.impactDataSetFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("firstPersonModelObjectFormID mismatch")
      {
        a.firstPersonModelObjectFormID = 0x01234567;
        b.firstPersonModelObjectFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("attackSound2DFormID mismatch")
      {
        a.attackSound2DFormID = 0x01234567;
        b.attackSound2DFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("attackLoopSoundFormID mismatch")
      {
        a.attackLoopSoundFormID = 0x01234567;
        b.attackLoopSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("attackFailSoundFormID mismatch")
      {
        a.attackFailSoundFormID = 0x01234567;
        b.attackFailSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("idleSoundFormID mismatch")
      {
        a.idleSoundFormID = 0x01234567;
        b.idleSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("equipSoundFormID mismatch")
      {
        a.equipSoundFormID = 0x01234567;
        b.equipSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("unequipSoundFormID mismatch")
      {
        a.unequipSoundFormID = 0x01234567;
        b.unequipSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("attackSoundFormID mismatch")
      {
        a.attackSoundFormID = 0x01234567;
        b.attackSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("value mismatch")
      {
        a.value = 1;
        b.value = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("weight mismatch")
      {
        a.weight = 1.0f;
        b.weight = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("baseDamage mismatch")
      {
        a.baseDamage = 1;
        b.baseDamage = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("DNAM mismatch")
      {
        a.unknownDNAM[99] = 12;
        b.unknownDNAM[99] = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CRDT mismatch")
      {
        a.unknownCRDT[15] = 12;
        b.unknownCRDT[15] = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("unknownVNAM mismatch")
      {
        a.unknownVNAM = 1;
        b.unknownVNAM = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CNAM mismatch")
      {
        a.unknownCNAM = 0;
        b.unknownCNAM.reset();

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownCNAM.reset();
        b.unknownCNAM = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownCNAM = 1;
        b.unknownCNAM = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    WeaponRecord record;

    REQUIRE( record.getRecordType() == cWEAP );
  }

  SECTION("getWriteSize")
  {
    WeaponRecord record;

    record.editorID = "foo";

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 182 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 194 );
    }

    SECTION("size adjusts with length of VMAD")
    {
      REQUIRE( record.getWriteSize() == 182 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "VMAD\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 206 );

      {
        const std::string_view data = "VMAD\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 189 );
    }

    SECTION("size adjusts when name is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts with length of model path")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.modelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 200 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 209 );
    }

    SECTION("size adjusts with length of MODT")
    {
      REQUIRE( record.getWriteSize() == 182 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 206 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 189 );
    }

    SECTION("size adjusts with length of MODS")
    {
      REQUIRE( record.getWriteSize() == 182 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODS\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 206 );

      {
        const std::string_view data = "MODS\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 189 );
    }

    SECTION("size adjusts when enchantingFormID is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.enchantingFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts when EAMT is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.enchantmentAmount = 42;
      REQUIRE( record.getWriteSize() == 190 );
    }

    SECTION("size adjusts when equipTypeFormID is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.equipTypeFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts when blockBashImpactDataSetFormID is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.blockBashImpactDataSetFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts when alternateBlockMaterialFormID is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.alternateBlockMaterialFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts with length of keywords")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 202 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 206 );
    }

    SECTION("size adjusts when description is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.description = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts with length of NNAM")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.unknownNNAM = "foo1bar2baz";
      REQUIRE( record.getWriteSize() == 200 );

      record.unknownNNAM = "foo1barbarbarbar2baz";
      REQUIRE( record.getWriteSize() == 209 );
    }

    SECTION("size adjusts when impactDataSetFormID is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.impactDataSetFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts when firstPersonModelObjectFormID is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.firstPersonModelObjectFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts when attackSound2DFormID is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.attackSound2DFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts when attackLoopSoundFormID is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.attackLoopSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts when attackFailSoundFormID is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.attackFailSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts when idleSoundFormID is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.idleSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts when equipSoundFormID is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.equipSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts when unequipSoundFormID is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.unequipSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts when attackSoundFormID is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.attackSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 192 );
    }

    SECTION("size adjusts when CNAM is present")
    {
      REQUIRE( record.getWriteSize() == 182 );

      record.unknownCNAM = 42;
      REQUIRE( record.getWriteSize() == 192 );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    StringTable dummy_table;
    dummy_table.addString(0x00010499, "foo");
    dummy_table.addString(0x00012616, "bar");
    dummy_table.addString(0x000005C0, "baz");
    dummy_table.addString(0x0000AC9A, "quux");

    SECTION("default: load record with keywords")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      WeaponRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0004A38F );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0006 );
      // -- record data
      REQUIRE( record.editorID == "DA08EbonyBlade" );
      REQUIRE( record.unknownVMAD.isPresent() );
      const auto VMAD = std::string_view(reinterpret_cast<const char*>(record.unknownVMAD.data()), record.unknownVMAD.size());
      REQUIRE( VMAD == "\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0"sv );
      REQUIRE( record.unknownOBND[0] == 0xFB );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xEA );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0xFC );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x09 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x58 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x05 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00010499 );
      REQUIRE( record.modelPath == "Weapons\\EbonyBlade\\EbonyBlade.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;"sv );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE( record.enchantingFormID == 0x0010FAF0 );
      REQUIRE_FALSE( record.enchantmentAmount.has_value() );
      REQUIRE( record.equipTypeFormID == 0x00013F45 );
      REQUIRE( record.blockBashImpactDataSetFormID == 0x000183FF );
      REQUIRE( record.alternateBlockMaterialFormID == 0x00018401 );
      REQUIRE( record.keywords.size() == 4 );
      REQUIRE( record.keywords[0] == 0x0001E711 );
      REQUIRE( record.keywords[1] == 0x000C27BD );
      REQUIRE( record.keywords[2] == 0x000917E8 );
      REQUIRE( record.keywords[3] == 0x000A8668 );
      REQUIRE( record.description.isPresent() );
      REQUIRE( record.description.getType() == LocalizedString::Type::Index );
      REQUIRE( record.description.getIndex() == 0x00012616 );
      REQUIRE( record.unknownNNAM.empty() );
      REQUIRE( record.impactDataSetFormID == 0x00013CAC );
      REQUIRE( record.firstPersonModelObjectFormID == 0x000C1434 );
      REQUIRE( record.attackSound2DFormID == 0 );
      REQUIRE( record.attackLoopSoundFormID == 0 );
      REQUIRE( record.attackFailSoundFormID == 0x0003C730 );
      REQUIRE( record.idleSoundFormID == 0 );
      REQUIRE( record.equipSoundFormID == 0x0003C72E );
      REQUIRE( record.unequipSoundFormID == 0x0003C72F );
      REQUIRE( record.attackSoundFormID == 0 );
      REQUIRE( record.value == 2000 );
      REQUIRE( record.weight == 10.0f );
      REQUIRE( record.baseDamage == 11 );
      const auto DNAM = std::string_view(reinterpret_cast<const char*>(record.unknownDNAM.data()), record.unknownDNAM.size());
      REQUIRE( DNAM == "\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?"sv );
      const auto CRDT = std::string_view(reinterpret_cast<const char*>(record.unknownCRDT.data()), record.unknownCRDT.size());
      REQUIRE( CRDT == "\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0"sv );
      REQUIRE( record.unknownVNAM == 1 );
      REQUIRE_FALSE( record.unknownCNAM.has_value() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with EAMT, SNAM, XNAM, CNAM")
    {
      const auto data = "WEAP\xB4\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\x34\x8E\0\x02XNAM\x04\0\x35\x8E\0\x02TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x04\0\x01\x08\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      WeaponRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0200D0CA );
      REQUIRE( record.headerRevision == 0x0055711D );
      REQUIRE( record.headerVersion == 43 );
      REQUIRE( record.headerUnknown5 == 0x0006 );
      // -- record data
      REQUIRE( record.editorID == "DLC1EnchCrossbowFire1" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
      REQUIRE( record.unknownOBND[0] == 0xE5 );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xE5 );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0xFD );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x1B );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x1B );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x06 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x000005C0 );
      REQUIRE( record.modelPath == "DLC01\\Weapons\\Crossbow\\Crossbow.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\0\0\0\0\0\0\0\0"sv );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE( record.enchantingFormID == 0x00049BB7 );
      REQUIRE( record.enchantmentAmount.has_value() );
      REQUIRE( record.enchantmentAmount == 0x00C8 );
      REQUIRE( record.equipTypeFormID == 0x00013F45 );
      REQUIRE( record.blockBashImpactDataSetFormID == 0x000193C6 );
      REQUIRE( record.alternateBlockMaterialFormID == 0x000774B6 );
      REQUIRE( record.keywords.size() == 3 );
      REQUIRE( record.keywords[0] == 0x0001E715 );
      REQUIRE( record.keywords[1] == 0x0001E719 );
      REQUIRE( record.keywords[2] == 0x0008F958 );
      REQUIRE( record.description.isPresent() );
      REQUIRE( record.description.getType() == LocalizedString::Type::Index );
      REQUIRE( record.description.getIndex() == 0 );
      REQUIRE( record.unknownNNAM.empty() );
      REQUIRE( record.impactDataSetFormID == 0x0200A311 );
      REQUIRE( record.firstPersonModelObjectFormID == 0x02000802 );
      REQUIRE( record.attackSound2DFormID == 0x02008E35 );
      REQUIRE( record.attackLoopSoundFormID == 0 );
      REQUIRE( record.attackFailSoundFormID == 0x0003C730 );
      REQUIRE( record.idleSoundFormID == 0 );
      REQUIRE( record.equipSoundFormID == 0x02017EC9 );
      REQUIRE( record.unequipSoundFormID == 0x02017ECA );
      REQUIRE( record.attackSoundFormID == 0x02008E34 );
      REQUIRE( record.value == 120 );
      REQUIRE( record.weight == 14.0f );
      REQUIRE( record.baseDamage == 19 );
      const auto DNAM = std::string_view(reinterpret_cast<const char*>(record.unknownDNAM.data()), 100);
      REQUIRE( DNAM == "\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?"sv );
      const auto CRDT = std::string_view(reinterpret_cast<const char*>(record.unknownCRDT.data()), 16);
      REQUIRE( CRDT == "\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0"sv );
      REQUIRE( record.unknownVNAM == 2 );
      REQUIRE( record.unknownCNAM.has_value() );
      REQUIRE( record.unknownCNAM == 0x02000801 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with UNAM")
    {
      const auto data = "WEAP\xA5\x01\0\0\0\0\0\0\x5E\x8F\x05\0\x1B\x69\x55\0\x28\0\x05\0EDID\x15\0BoundWeaponBattleaxe\0OBND\x0C\0\xFE\xFF\x09\0\x06\0\xFF\xFF\x09\0\x07\0FULL\x04\0\x9A\xAC\0\0MODL(\0Weapons\\BoundWeapons\\BoundAxeHolder.nif\0MODT$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x35\xFC\x8B#dds\08\x97<\xEAw|Qpdds\0X,U3EITM\x04\0\xE2[\x0C\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC7\x93\x01\0BAMT\x04\0\xA9\x64\x0E\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x32\xD9\x06\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0[j\x03\0TNAM\x04\0\xACL\x0E\0UNAM\x04\0\xE6`\x09\0NAM9\x04\0j\x03\x06\0NAM8\x04\0\xD3\x05\x06\0DATA\x0A\0\0\0\0\0\0\0\0\0\x11\0DNAMd\0\x06\0\0\0\x33\x33\x33?ff\xA6?\xC8\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0!\0\0\0\0\x80?33\xB3?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x33\x33\x93?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      WeaponRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00058F5E );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0005 );
      // -- record data
      REQUIRE( record.editorID == "BoundWeaponBattleaxe" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
      REQUIRE( record.unknownOBND[0] == 0xFE );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0x09 );
      REQUIRE( record.unknownOBND[3] == 0x00 );
      REQUIRE( record.unknownOBND[4] == 0x06 );
      REQUIRE( record.unknownOBND[5] == 0x00 );
      REQUIRE( record.unknownOBND[6] == 0xFF );
      REQUIRE( record.unknownOBND[7] == 0xFF );
      REQUIRE( record.unknownOBND[8] == 0x09 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x07 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000AC9A );
      REQUIRE( record.modelPath == "Weapons\\BoundWeapons\\BoundAxeHolder.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x02\0\0\0\0\0\0\0\x35\xFC\x8B#dds\08\x97<\xEAw|Qpdds\0X,U3"sv );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE( record.enchantingFormID == 0x000C5BE2 );
      REQUIRE_FALSE( record.enchantmentAmount.has_value() );
      REQUIRE( record.equipTypeFormID == 0x00013F45 );
      REQUIRE( record.blockBashImpactDataSetFormID == 0x000193C7 );
      REQUIRE( record.alternateBlockMaterialFormID == 0x000E64A9 );
      REQUIRE( record.keywords.size() == 2 );
      REQUIRE( record.keywords[0] == 0x0006D932 );
      REQUIRE( record.keywords[1] == 0x0008F958 );
      REQUIRE( record.description.isPresent() );
      REQUIRE( record.description.getType() == LocalizedString::Type::Index );
      REQUIRE( record.description.getIndex() == 0 );
      REQUIRE( record.unknownNNAM.empty() );
      REQUIRE( record.impactDataSetFormID == 0x00036A5B );
      REQUIRE( record.firstPersonModelObjectFormID == 0 );
      REQUIRE( record.attackSound2DFormID == 0 );
      REQUIRE( record.attackLoopSoundFormID == 0 );
      REQUIRE( record.attackFailSoundFormID == 0x000E4CAC );
      REQUIRE( record.idleSoundFormID == 0x000960E6 );
      REQUIRE( record.equipSoundFormID == 0x0006036A );
      REQUIRE( record.unequipSoundFormID == 0x000605D3 );
      REQUIRE( record.attackSoundFormID == 0 );
      REQUIRE( record.value == 0 );
      REQUIRE( record.weight == 0.0f );
      REQUIRE( record.baseDamage == 17 );
      const auto DNAM = std::string_view(reinterpret_cast<const char*>(record.unknownDNAM.data()), 100);
      REQUIRE( DNAM == "\x06\0\0\0\x33\x33\x33?ff\xA6?\xC8\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0!\0\0\0\0\x80?33\xB3?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x33\x33\x93?"sv );
      const auto CRDT = std::string_view(reinterpret_cast<const char*>(record.unknownCRDT.data()), 16);
      REQUIRE( CRDT == "\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0"sv );
      REQUIRE( record.unknownVNAM == 1 );
      REQUIRE_FALSE( record.unknownCNAM.has_value() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with NNAM")
    {
      const auto data = "WEAP\xA5\x01\0\0\0\0\0\0\xE5\x3F\x07\0\x1B\x69\x55\0\x28\0\x06\0EDID\x11\0CWCatapultWeapon\0OBND\x0C\0\xEF\xFF\xC7\xFF\xFE\xFF\x04\0\x39\0\x02\0MODL\x1B\0Weapons\\Steel\\SteelBow.nif\0MODT<\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x9D\xC0\xB4%dds\0\x14\xF2@\xE6\x97\x1E\xA2\xC8\x64\x64s\0\x14\xF2@\xE6\xB8\xEC\x30{dds\0&,\x33;-O\xABQdds\0\x14\xF2@\xE6\x45TYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0X\xF9\x08\0DESC\x04\0\0\0\0\0NNAM\x0B\0BoltNode02\0INAM\x04\0\xAF\xC2\x01\0WNAM\x04\0Z;\x02\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\x8C\xD7\x03\0NAM8\x04\0\x82\xD8\x03\0DATA\x0A\0\x05\0\0\0\0\0\x80@\x0A\0DNAMd\0\x07\0\0\0\0\0\x80?\0\0\x80?\x88\0\x91\0\xCD\xCC\xCC>\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\xA0@\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      WeaponRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00073FE5 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0006 );
      // -- record data
      REQUIRE( record.editorID == "CWCatapultWeapon" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
      REQUIRE( record.unknownOBND[0] == 0xEF );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xC7 );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0xFE );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x04 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x39 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x02 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::None );
      REQUIRE( record.modelPath == "Weapons\\Steel\\SteelBow.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x04\0\0\0\0\0\0\0\x9D\xC0\xB4%dds\0\x14\xF2@\xE6\x97\x1E\xA2\xC8\x64\x64s\0\x14\xF2@\xE6\xB8\xEC\x30{dds\0&,\x33;-O\xABQdds\0\x14\xF2@\xE6"sv );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE( record.enchantingFormID == 0 );
      REQUIRE_FALSE( record.enchantmentAmount.has_value() );
      REQUIRE( record.equipTypeFormID == 0x00013F45 );
      REQUIRE( record.blockBashImpactDataSetFormID == 0x000193C6 );
      REQUIRE( record.alternateBlockMaterialFormID == 0x00018401 );
      REQUIRE( record.keywords.size() == 1 );
      REQUIRE( record.keywords[0] == 0x0008F958 );
      REQUIRE( record.description.isPresent() );
      REQUIRE( record.description.getType() == LocalizedString::Type::Index );
      REQUIRE( record.description.getIndex() == 0 );
      REQUIRE( record.unknownNNAM == "BoltNode02" );
      REQUIRE( record.impactDataSetFormID == 0x0001C2AF );
      REQUIRE( record.firstPersonModelObjectFormID == 0x00023B5A );
      REQUIRE( record.attackSoundFormID == 0 );
      REQUIRE( record.attackSound2DFormID == 0 );
      REQUIRE( record.attackLoopSoundFormID == 0 );
      REQUIRE( record.attackFailSoundFormID == 0x0003C730 );
      REQUIRE( record.idleSoundFormID == 0 );
      REQUIRE( record.equipSoundFormID == 0x0003D78C );
      REQUIRE( record.unequipSoundFormID == 0x0003D882 );
      REQUIRE( record.value == 5 );
      REQUIRE( record.weight == 4.0f );
      REQUIRE( record.baseDamage == 10 );
      const auto DNAM = std::string_view(reinterpret_cast<const char*>(record.unknownDNAM.data()), 100);
      REQUIRE( DNAM == "\x07\0\0\0\0\0\x80?\0\0\x80?\x88\0\x91\0\xCD\xCC\xCC>\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\xA0@\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0"sv );
      const auto CRDT = std::string_view(reinterpret_cast<const char*>(record.unknownCRDT.data()), 16);
      REQUIRE( CRDT == "\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0"sv );
      REQUIRE( record.unknownVNAM == 1 );
      REQUIRE_FALSE( record.unknownCNAM.has_value() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0FAIL\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\x02GA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\xFF\x01GA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of EDID can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\xFF\0DA08Eb"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple VMADs")
    {
      const auto data = "WEAP\x79\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of VMAD can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0FAIL\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const auto data = "WEAP\x19\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const auto data = "WEAP\x3D\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0B\0\xFB\xFF\xEA\xFF\xFC\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2C\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0D\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const auto data = "WEAP\x35\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\x02Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple model paths")
    {
      const auto data = "WEAP\x53\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODL can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\Ebony"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: model path is empty string")
    {
      const auto data = "WEAP\x09\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\x01\0\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const auto data = "WEAP\x91\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODT can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODS can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;MODS\x21\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple EITMs")
    {
      const auto data = "WEAP\x35\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EITM is not four")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x03\0\xF0\xFA\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2C\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x05\0\xF0\xFA\x10\0\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of EITM can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: EITM is zero")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\0\0\0\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple EAMTs")
    {
      const auto data = "WEAP\xBC\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\x34\x8E\0\x02XNAM\x04\0\x35\x8E\0\x02TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x04\0\x01\x08\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EAMT is not two")
    {
      {
        const auto data = "WEAP\xB3\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x01\0\xC8\x45TYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\x34\x8E\0\x02XNAM\x04\0\x35\x8E\0\x02TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x04\0\x01\x08\0\x02"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\xB4\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x03\0\xC8\0\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\x34\x8E\0\x02XNAM\x04\0\x35\x8E\0\x02TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x04\0\x01\x08\0\x02"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of EAMT can be read")
    {
      const auto data = "WEAP\xB4\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ETYPs")
    {
      const auto data = "WEAP\x35\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ETYP is not four")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x03\0E?\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2C\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x05\0E?\x01\0\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of ETYP can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: ETYP is zero")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0\0\0\0\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple BIDSs")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BIDS\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of BIDS is not four")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x03\0\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x05\0\xFF\x83\x01\0\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of BIDS can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: BIDS is zero")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\0\0\0\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple BAMTs")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BAMT\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of BAMT is not four")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x03\0\x01\x84\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2C\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x05\0\x01\x84\x01\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of BAMT can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: BAMT is zero")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\0\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple KSIZs")
    {
      const auto data = "WEAP\x4B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KSIZ is not four")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x03\0\x04\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2C\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x05\0\x04\0\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before KSIZ can be read completely")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\0\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero and KWDA is missing")
    {
      const auto data = "WEAP\x15\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\0\0\0\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no KWDA")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0FAIL\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing KWDA")
    {
      const auto data = "WEAP\x15\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KWDA is not a multiple of four or it's zero")
    {
      {
        const auto data = "WEAP\x2C\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x11\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x1B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\0\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of KWDA can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of DESC can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DESC entries")
    {
      const auto data = "WEAP\x35\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple NNAMs")
    {
      const auto data = "WEAP\xB6\x01\0\0\0\0\0\0\xE5\x3F\x07\0\x1B\x69\x55\0\x28\0\x06\0EDID\x11\0CWCatapultWeapon\0OBND\x0C\0\xEF\xFF\xC7\xFF\xFE\xFF\x04\0\x39\0\x02\0MODL\x1B\0Weapons\\Steel\\SteelBow.nif\0MODT<\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x9D\xC0\xB4%dds\0\x14\xF2@\xE6\x97\x1E\xA2\xC8\x64\x64s\0\x14\xF2@\xE6\xB8\xEC\x30{dds\0&,\x33;-O\xABQdds\0\x14\xF2@\xE6\x45TYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0X\xF9\x08\0DESC\x04\0\0\0\0\0NNAM\x0B\0BoltNode02\0NNAM\x0B\0BoltNode02\0INAM\x04\0\xAF\xC2\x01\0WNAM\x04\0Z;\x02\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\x8C\xD7\x03\0NAM8\x04\0\x82\xD8\x03\0DATA\x0A\0\x05\0\0\0\0\0\x80@\x0A\0DNAMd\0\x07\0\0\0\0\0\x80?\0\0\x80?\x88\0\x91\0\xCD\xCC\xCC>\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\xA0@\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of NNAM > 512")
    {
      const auto data = "WEAP\xA5\x01\0\0\0\0\0\0\xE5\x3F\x07\0\x1B\x69\x55\0\x28\0\x06\0EDID\x11\0CWCatapultWeapon\0OBND\x0C\0\xEF\xFF\xC7\xFF\xFE\xFF\x04\0\x39\0\x02\0MODL\x1B\0Weapons\\Steel\\SteelBow.nif\0MODT<\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x9D\xC0\xB4%dds\0\x14\xF2@\xE6\x97\x1E\xA2\xC8\x64\x64s\0\x14\xF2@\xE6\xB8\xEC\x30{dds\0&,\x33;-O\xABQdds\0\x14\xF2@\xE6\x45TYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0X\xF9\x08\0DESC\x04\0\0\0\0\0NNAM\x0B\x02GoltNode02\0INAM\x04\0\xAF\xC2\x01\0WNAM\x04\0Z;\x02\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\x8C\xD7\x03\0NAM8\x04\0\x82\xD8\x03\0DATA\x0A\0\x05\0\0\0\0\0\x80@\x0A\0DNAMd\0\x07\0\0\0\0\0\x80?\0\0\x80?\x88\0\x91\0\xCD\xCC\xCC>\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\xA0@\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of NNAM is beyond stream")
    {
      const auto data = "WEAP\xA5\x01\0\0\0\0\0\0\xE5\x3F\x07\0\x1B\x69\x55\0\x28\0\x06\0EDID\x11\0CWCatapultWeapon\0OBND\x0C\0\xEF\xFF\xC7\xFF\xFE\xFF\x04\0\x39\0\x02\0MODL\x1B\0Weapons\\Steel\\SteelBow.nif\0MODT<\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x9D\xC0\xB4%dds\0\x14\xF2@\xE6\x97\x1E\xA2\xC8\x64\x64s\0\x14\xF2@\xE6\xB8\xEC\x30{dds\0&,\x33;-O\xABQdds\0\x14\xF2@\xE6\x45TYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0X\xF9\x08\0DESC\x04\0\0\0\0\0NNAM\xDE\x01GoltNode02\0INAM\x04\0\xAF\xC2\x01\0WNAM\x04\0Z;\x02\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\x8C\xD7\x03\0NAM8\x04\0\x82\xD8\x03\0DATA\x0A\0\x05\0\0\0\0\0\x80@\x0A\0DNAMd\0\x07\0\0\0\0\0\x80?\0\0\x80?\x88\0\x91\0\xCD\xCC\xCC>\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\xA0@\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of NNAM can be read")
    {
      const auto data = "WEAP\xA5\x01\0\0\0\0\0\0\xE5\x3F\x07\0\x1B\x69\x55\0\x28\0\x06\0EDID\x11\0CWCatapultWeapon\0OBND\x0C\0\xEF\xFF\xC7\xFF\xFE\xFF\x04\0\x39\0\x02\0MODL\x1B\0Weapons\\Steel\\SteelBow.nif\0MODT<\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x9D\xC0\xB4%dds\0\x14\xF2@\xE6\x97\x1E\xA2\xC8\x64\x64s\0\x14\xF2@\xE6\xB8\xEC\x30{dds\0&,\x33;-O\xABQdds\0\x14\xF2@\xE6\x45TYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0X\xF9\x08\0DESC\x04\0\0\0\0\0NNAM\x0B\0Bolt"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: NNAM is empty")
    {
      const auto data = "WEAP\x9B\x01\0\0\0\0\0\0\xE5\x3F\x07\0\x1B\x69\x55\0\x28\0\x06\0EDID\x11\0CWCatapultWeapon\0OBND\x0C\0\xEF\xFF\xC7\xFF\xFE\xFF\x04\0\x39\0\x02\0MODL\x1B\0Weapons\\Steel\\SteelBow.nif\0MODT<\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x9D\xC0\xB4%dds\0\x14\xF2@\xE6\x97\x1E\xA2\xC8\x64\x64s\0\x14\xF2@\xE6\xB8\xEC\x30{dds\0&,\x33;-O\xABQdds\0\x14\xF2@\xE6\x45TYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0X\xF9\x08\0DESC\x04\0\0\0\0\0NNAM\x01\0\0INAM\x04\0\xAF\xC2\x01\0WNAM\x04\0Z;\x02\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\x8C\xD7\x03\0NAM8\x04\0\x82\xD8\x03\0DATA\x0A\0\x05\0\0\0\0\0\x80@\x0A\0DNAMd\0\x07\0\0\0\0\0\x80?\0\0\x80?\x88\0\x91\0\xCD\xCC\xCC>\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\xA0@\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple INAMs")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0INAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of INAM is not four")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x03\0<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2C\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x05\0\xAC<\x01\0\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of INAM can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: INAM is zero")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\0\0\0\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple WNAMs")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0WNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of WNAM is not four")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x03\0\x34\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2c\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x05\0\x34\x14\x0C\0\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of WNAM can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: WNAM is zero")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\0\0\0\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple SNAMs")
    {
      const auto data = "WEAP\xB4\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\x34\x8E\0\x02SNAM\x04\0\x35\x8E\0\x02TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x04\0\x01\x08\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SNAM is not four")
    {
      {
        const auto data = "WEAP\xB3\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x03\0\x34\x8E\0XNAM\x04\0\x35\x8E\0\x02TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x04\0\x01\x08\0\x02"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\xB4\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x05\0\x34\x8E\0\x02\0XNAM\x04\0\x35\x8E\0\x02TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x04\0\x01\x08\0\x02"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of SNAM can be read")
    {
      const auto data = "WEAP\xB4\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\x34"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: SNAM is zero")
    {
      const auto data = "WEAP\xB4\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\0\0\0\0XNAM\x04\0\x35\x8E\0\x02TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x04\0\x01\x08\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XNAMs")
    {
      const auto data = "WEAP\xB4\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02XNAM\x04\0\x34\x8E\0\x02XNAM\x04\0\x35\x8E\0\x02TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x04\0\x01\x08\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XNAM is not four")
    {
      {
        const auto data = "WEAP\xB3\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\x34\x8E\0\x02XNAM\x03\0\x35\x8E\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x04\0\x01\x08\0\x02"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\xB5\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\x34\x8E\0\x02XNAM\x05\0\x35\x8E\0\x02\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x04\0\x01\x08\0\x02"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XNAM can be read")
    {
      const auto data = "WEAP\xB4\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\x34\x8E\0\x02XNAM\x04\0\x35"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: XNAM is zero")
    {
      const auto data = "WEAP\xB4\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\x34\x8E\0\x02XNAM\x04\0\0\0\0\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x04\0\x01\x08\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple TNAMs")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0TNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of TNAM is not four")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x03\0\x30\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2C\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x05\0\x30\xC7\x03\0\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of TNAM can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: TNAM is zero")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\0\0\0\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple UNAMs")
    {
      const auto data = "WEAP\xA5\x01\0\0\0\0\0\0\x5E\x8F\x05\0\x1B\x69\x55\0\x28\0\x05\0EDID\x15\0BoundWeaponBattleaxe\0OBND\x0C\0\xFE\xFF\x09\0\x06\0\xFF\xFF\x09\0\x07\0FULL\x04\0\x9A\xAC\0\0MODL(\0Weapons\\BoundWeapons\\BoundAxeHolder.nif\0MODT$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x35\xFC\x8B#dds\08\x97<\xEAw|Qpdds\0X,U3EITM\x04\0\xE2[\x0C\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC7\x93\x01\0BAMT\x04\0\xA9\x64\x0E\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x32\xD9\x06\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0[j\x03\0UNAM\x04\0\xACL\x0E\0UNAM\x04\0\xE6`\x09\0NAM9\x04\0j\x03\x06\0NAM8\x04\0\xD3\x05\x06\0DATA\x0A\0\0\0\0\0\0\0\0\0\x11\0DNAMd\0\x06\0\0\0\x33\x33\x33?ff\xA6?\xC8\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0!\0\0\0\0\x80?33\xB3?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x33\x33\x93?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of UNAM is not four")
    {
      {
        const auto data = "WEAP\xA4\x01\0\0\0\0\0\0\x5E\x8F\x05\0\x1B\x69\x55\0\x28\0\x05\0EDID\x15\0BoundWeaponBattleaxe\0OBND\x0C\0\xFE\xFF\x09\0\x06\0\xFF\xFF\x09\0\x07\0FULL\x04\0\x9A\xAC\0\0MODL(\0Weapons\\BoundWeapons\\BoundAxeHolder.nif\0MODT$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x35\xFC\x8B#dds\08\x97<\xEAw|Qpdds\0X,U3EITM\x04\0\xE2[\x0C\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC7\x93\x01\0BAMT\x04\0\xA9\x64\x0E\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x32\xD9\x06\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0[j\x03\0TNAM\x04\0\xACL\x0E\0UNAM\x03\0\xE6`\0NAM9\x04\0j\x03\x06\0NAM8\x04\0\xD3\x05\x06\0DATA\x0A\0\0\0\0\0\0\0\0\0\x11\0DNAMd\0\x06\0\0\0\x33\x33\x33?ff\xA6?\xC8\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0!\0\0\0\0\x80?33\xB3?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x33\x33\x93?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\xA6\x01\0\0\0\0\0\0\x5E\x8F\x05\0\x1B\x69\x55\0\x28\0\x05\0EDID\x15\0BoundWeaponBattleaxe\0OBND\x0C\0\xFE\xFF\x09\0\x06\0\xFF\xFF\x09\0\x07\0FULL\x04\0\x9A\xAC\0\0MODL(\0Weapons\\BoundWeapons\\BoundAxeHolder.nif\0MODT$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x35\xFC\x8B#dds\08\x97<\xEAw|Qpdds\0X,U3EITM\x04\0\xE2[\x0C\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC7\x93\x01\0BAMT\x04\0\xA9\x64\x0E\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x32\xD9\x06\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0[j\x03\0TNAM\x04\0\xACL\x0E\0UNAM\x05\0\xE6`\x09\0\0NAM9\x04\0j\x03\x06\0NAM8\x04\0\xD3\x05\x06\0DATA\x0A\0\0\0\0\0\0\0\0\0\x11\0DNAMd\0\x06\0\0\0\x33\x33\x33?ff\xA6?\xC8\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0!\0\0\0\0\x80?33\xB3?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x33\x33\x93?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of UNAM can be read")
    {
      const auto data = "WEAP\xA5\x01\0\0\0\0\0\0\x5E\x8F\x05\0\x1B\x69\x55\0\x28\0\x05\0EDID\x15\0BoundWeaponBattleaxe\0OBND\x0C\0\xFE\xFF\x09\0\x06\0\xFF\xFF\x09\0\x07\0FULL\x04\0\x9A\xAC\0\0MODL(\0Weapons\\BoundWeapons\\BoundAxeHolder.nif\0MODT$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x35\xFC\x8B#dds\08\x97<\xEAw|Qpdds\0X,U3EITM\x04\0\xE2[\x0C\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC7\x93\x01\0BAMT\x04\0\xA9\x64\x0E\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x32\xD9\x06\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0[j\x03\0TNAM\x04\0\xACL\x0E\0UNAM\x04\0\xE6"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: UNAM is zero")
    {
      const auto data = "WEAP\xA5\x01\0\0\0\0\0\0\x5E\x8F\x05\0\x1B\x69\x55\0\x28\0\x05\0EDID\x15\0BoundWeaponBattleaxe\0OBND\x0C\0\xFE\xFF\x09\0\x06\0\xFF\xFF\x09\0\x07\0FULL\x04\0\x9A\xAC\0\0MODL(\0Weapons\\BoundWeapons\\BoundAxeHolder.nif\0MODT$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x35\xFC\x8B#dds\08\x97<\xEAw|Qpdds\0X,U3EITM\x04\0\xE2[\x0C\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC7\x93\x01\0BAMT\x04\0\xA9\x64\x0E\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x32\xD9\x06\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0[j\x03\0TNAM\x04\0\xACL\x0E\0UNAM\x04\0\0\0\0\0NAM9\x04\0j\x03\x06\0NAM8\x04\0\xD3\x05\x06\0DATA\x0A\0\0\0\0\0\0\0\0\0\x11\0DNAMd\0\x06\0\0\0\x33\x33\x33?ff\xA6?\xC8\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0!\0\0\0\0\x80?33\xB3?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x33\x33\x93?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple NAM9s")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM9\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of NAM9 is not four")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x03\0.\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2C\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x05\0.\xC7\x03\0\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of NAM9 can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: NAM9 is zero")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\0\0\0\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple NAM8s")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM8\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of NAM8 is not four")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x03\0/\xC7\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2C\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x05\0/\xC7\x03\0\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of NAM8 can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: NAM8 is zero")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0\0\0\0\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const auto data = "WEAP\xC5\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not ten")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x09\0\xD0\x07\0\0\0\0\x20\x41\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2C\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0B\0\xD0\x07\0\0\0\0\x20\x41\x0B\0\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no DNAM")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0FAILd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DNAM is not 100")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMc\0\x05\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2C\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMe\0\x05\0\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DNAM can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no CRDT")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?FAIL\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of CRDT is not 16")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x0F\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2C\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x11\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0\0VNAM\x04\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of CRDT can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no VNAM")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0FAIL\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of VNAM is not four")
    {
      {
        const auto data = "WEAP\x2A\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x03\0\x01\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\x2C\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x05\0\x01\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of VNAM can be read")
    {
      const auto data = "WEAP\x2B\x02\0\0\0\0\0\0\x8F\xA3\x04\0\x1B\x69\x55\0\x28\0\x06\0EDID\x0F\0DA08EbonyBlade\0VMADH\0\x05\0\x02\0\x01\0\x14\0DA08EbonyBladeScript\0\x02\0\x04\0DA08\x01\x01\0\0\xFF\xFF{\xA3\x04\0\x0D\0InitialPickup\x01\x01\0\0\xFF\xFF\xF0,\x06\0OBND\x0C\0\xFB\xFF\xEA\xFF\xFC\xFF\x09\0X\0\x05\0FULL\x04\0\x99\x04\x01\0MODL\"\0Weapons\\EbonyBlade\\EbonyBlade.nif\0MODT`\0\x02\0\0\0\x07\0\0\0\0\0\0\0O\xE0uddds\0\xA0\xB8\x63\x05\x33\x16\x8C\x07\x64\x64s\0\xA0\xB8\x63\x05\x03\xD8\xC2@dds\0\xA0\xB8\x63\x05\x45Ij\\dds\0\x7F\xD1Y\x13\xDA\x13'\x15\x64\x64s\0\x7F\xD1Y\x13\xB6]\x9E\x1F\x64\x64s\0&,\x33;\xF6\x38\x90\xAC\x64\x64s\0&,\x33;EITM\x04\0\xF0\xFA\x10\0ETYP\x04\0E?\x01\0BIDS\x04\0\xFF\x83\x01\0BAMT\x04\0\x01\x84\x01\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\x11\xE7\x01\0\xBD'\x0C\0\xE8\x17\x09\0h\x86\x0A\0DESC\x04\0\x16&\x01\0INAM\x04\0\xAC<\x01\0WNAM\x04\0\x34\x14\x0C\0TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0.\xC7\x03\0NAM8\x04\0/\xC7\x03\0DATA\x0A\0\xD0\x07\0\0\0\0\x20\x41\x0B\0DNAMd\0\x05\0\0\0\0\0\x80?\0\0\x80?\0\0\x91\0\0\0\0\0\0\0\0\0\0\xFF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\xCD\xCCL?\0\0\0?\0\0\x80?\xC3\xF5\xA8>\0\0\0\0\0\0\0\0\0\0\0\0\x07\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\0\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple CNAMs")
    {
      const auto data = "WEAP\xBE\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\x34\x8E\0\x02XNAM\x04\0\x35\x8E\0\x02TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x04\0\x01\x08\0\x02\x43NAM\x04\0\x01\x08\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of CNAM is not four")
    {
      {
        const auto data = "WEAP\xB3\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\x34\x8E\0\x02XNAM\x04\0\x35\x8E\0\x02TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x03\0\x01\x08\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WEAP\xB5\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\x34\x8E\0\x02XNAM\x04\0\x35\x8E\0\x02TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x05\0\x01\x08\0\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WEAP, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WeaponRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of CNAM can be read")
    {
      const auto data = "WEAP\xB4\x01\0\0\0\0\0\0\xCA\xD0\0\x02\x1D\x71\x55\0\x2B\0\x06\0EDID\x16\0DLC1EnchCrossbowFire1\0OBND\x0C\0\xE5\xFF\xE5\xFF\xFD\xFF\x1B\0\x1B\0\x06\0FULL\x04\0\xC0\x05\0\0MODL$\0DLC01\\Weapons\\Crossbow\\Crossbow.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0EITM\x04\0\xB7\x9B\x04\0EAMT\x02\0\xC8\0ETYP\x04\0E?\x01\0BIDS\x04\0\xC6\x93\x01\0BAMT\x04\0\xB6t\x07\0KSIZ\x04\0\x03\0\0\0KWDA\x0C\0\x15\xE7\x01\0\x19\xE7\x01\0X\xF9\x08\0DESC\x04\0\0\0\0\0INAM\x04\0\x11\xA3\0\x02WNAM\x04\0\x02\x08\0\x02SNAM\x04\0\x34\x8E\0\x02XNAM\x04\0\x35\x8E\0\x02TNAM\x04\0\x30\xC7\x03\0NAM9\x04\0\xC9~\x01\x02NAM8\x04\0\xCA~\x01\x02\x44\x41TA\x0A\0x\0\0\0\0\0`A\x13\0DNAMd\0\x09\0\0\0\0\0\x80?\0\0\x80?\0\0\x93\0\0\0\0\0\0\0\0\0\x05\xFF\x01\0\0\0\xFA\x43\0\0\xFA\x44\0\0\0\0\0\0\0\0\0\0\x80?\0\0\x80?\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0@?CRDT\x10\0\x09\0\0\0\0\0\x80?\x01\xFF\xFF\xFF\0\0\0\0VNAM\x04\0\x02\0\0\0CNAM\x04\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WEAP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WeaponRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }
}
