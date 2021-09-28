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
#include "../../../../../sr/base/records/ArmourRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("ArmourRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    ArmourRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.unknownVMAD.isPresent() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.enchantingFormID == 0 );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMO2T.isPresent() );
    REQUIRE_FALSE( record.unknownMO2S.isPresent() );
    REQUIRE( record.mod4Path.empty() );
    REQUIRE_FALSE( record.unknownMO4T.isPresent() );
    REQUIRE_FALSE( record.unknownMO4S.isPresent() );
    REQUIRE_FALSE( record.unknownBODT.isPresent() );
    REQUIRE_FALSE( record.unknownBOD2.isPresent() );
    REQUIRE( record.equipTypeFormID == 0 );
    REQUIRE( record.blockBashImpactDataSetFormID == 0 );
    REQUIRE( record.alternateBlockMaterialFormID == 0 );
    REQUIRE( record.pickupSoundFormID == 0 );
    REQUIRE( record.putdownSoundFormID == 0 );
    REQUIRE( record.unknownRNAM == 0 );
    REQUIRE( record.keywordArray.empty() );
    REQUIRE_FALSE( record.description.isPresent() );
    REQUIRE( record.models.empty() );
    REQUIRE( record.value == 0 );
    REQUIRE( record.weight == 0.0f );
    REQUIRE( record.unknownDNAM == 0 );
    REQUIRE( record.templateArmorFormID == 0 );
  }

  SECTION("equals")
  {
    ArmourRecord a;
    ArmourRecord b;

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

      SECTION("enchantingFormID mismatch")
      {
        a.enchantingFormID = 0x01234567;
        b.enchantingFormID = 0x0FEDCBA9;

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

      SECTION("MO2T mismatch")
      {
        a.unknownMO2T.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownMO2T.setPresence(false);
        b.unknownMO2T.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MO2S mismatch")
      {
        a.unknownMO2S.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownMO2S.setPresence(false);
        b.unknownMO2S.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("mod4Path mismatch")
      {
        a.mod4Path = "foo\\bar.dds";
        b.mod4Path = "foo2\\bar3.dds";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MO4T mismatch")
      {
        a.unknownMO4T.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownMO4T.setPresence(false);
        b.unknownMO4T.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MO4S mismatch")
      {
        a.unknownMO4S.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownMO4S.setPresence(false);
        b.unknownMO4S.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("BODT mismatch")
      {
        a.unknownBODT.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownBODT.setPresence(false);
        b.unknownBODT.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("BOD2 mismatch")
      {
        a.unknownBOD2.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownBOD2.setPresence(false);
        b.unknownBOD2.setPresence(true);

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

      SECTION("pickupSoundFormID mismatch")
      {
        a.pickupSoundFormID = 0x01234567;
        b.pickupSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("putdownSoundFormID mismatch")
      {
        a.putdownSoundFormID = 0x01234567;
        b.putdownSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("RNAM mismatch")
      {
        a.unknownRNAM = 0x01234567;
        b.unknownRNAM = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("keywords mismatch")
      {
        a.keywordArray.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.keywordArray.push_back(0x01234567);
        b.keywordArray.push_back(0x09ABCDEF);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("description mismatch")
      {
        a.description = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("models mismatch")
      {
        a.models.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.models.push_back(0x01234567);
        b.models.push_back(0x09ABCDEF);

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

      SECTION("DNAM mismatch")
      {
        a.unknownDNAM = 0x01234567;
        b.unknownDNAM = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("templateArmorFormID mismatch")
      {
        a.templateArmorFormID = 0x01234567;
        b.templateArmorFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    ArmourRecord record;

    REQUIRE( record.getRecordType() == cARMO );
  }

  SECTION("getWriteSize")
  {
    ArmourRecord record;

    record.editorID = "foo";

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 62 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 74 );
    }

    SECTION("size adjusts with length of VMAD")
    {
      REQUIRE( record.getWriteSize() == 62 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "VMAD\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 86 );

      {
        const std::string_view data = "VMAD\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts when name is present")
    {
      REQUIRE( record.getWriteSize() == 62 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 72 );
    }

    SECTION("size adjusts when enchantingFormID is present")
    {
      REQUIRE( record.getWriteSize() == 62 );

      record.enchantingFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 72 );
    }

    SECTION("size adjusts with length of model path")
    {
      REQUIRE( record.getWriteSize() == 62 );

      record.modelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 80 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 89 );
    }

    SECTION("size adjusts with length of MO2T")
    {
      REQUIRE( record.getWriteSize() == 62 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MO2T\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMO2T.loadFromStream(stream, cMO2T, true) );
      }
      REQUIRE( record.getWriteSize() == 86 );

      {
        const std::string_view data = "MO2T\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMO2T.loadFromStream(stream, cMO2T, true) );
      }
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts with length of MO2S")
    {
      REQUIRE( record.getWriteSize() == 62 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MO2S\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMO2S.loadFromStream(stream, cMO2S, true) );
      }
      REQUIRE( record.getWriteSize() == 86 );

      {
        const std::string_view data = "MO2S\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMO2S.loadFromStream(stream, cMO2S, true) );
      }
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts with length of MOD4 path")
    {
      REQUIRE( record.getWriteSize() == 62 );

      record.mod4Path = "foo\\bar.dds";
      REQUIRE( record.getWriteSize() == 80 );

      record.mod4Path = "foo\\barbarbarbar.dds";
      REQUIRE( record.getWriteSize() == 89 );
    }

    SECTION("size adjusts with length of MO4T")
    {
      REQUIRE( record.getWriteSize() == 62 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MO4T\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMO4T.loadFromStream(stream, cMO4T, true) );
      }
      REQUIRE( record.getWriteSize() == 86 );

      {
        const std::string_view data = "MO4T\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMO4T.loadFromStream(stream, cMO4T, true) );
      }
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts with length of MO4S")
    {
      REQUIRE( record.getWriteSize() == 62 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MO4S\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMO4S.loadFromStream(stream, cMO4S, true) );
      }
      REQUIRE( record.getWriteSize() == 86 );

      {
        const std::string_view data = "MO4S\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMO4S.loadFromStream(stream, cMO4S, true) );
      }
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts with length of BODT")
    {
      REQUIRE( record.getWriteSize() == 62 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "BODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownBODT.loadFromStream(stream, cBODT, true) );
      }
      REQUIRE( record.getWriteSize() == 86 );

      {
        const std::string_view data = "BODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownBODT.loadFromStream(stream, cBODT, true) );
      }
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts with length of BOD2")
    {
      REQUIRE( record.getWriteSize() == 62 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "BOD2\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownBOD2.loadFromStream(stream, cBOD2, true) );
      }
      REQUIRE( record.getWriteSize() == 86 );

      {
        const std::string_view data = "BOD2\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownBOD2.loadFromStream(stream, cBOD2, true) );
      }
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts when equipTypeFormID is present")
    {
      REQUIRE( record.getWriteSize() == 62 );

      record.equipTypeFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 72 );
    }

    SECTION("size adjusts when blockBashImpactDataSetFormID is present")
    {
      REQUIRE( record.getWriteSize() == 62 );

      record.blockBashImpactDataSetFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 72 );
    }

    SECTION("size adjusts when alternateBlockMaterialFormID is present")
    {
      REQUIRE( record.getWriteSize() == 62 );

      record.alternateBlockMaterialFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 72 );
    }

    SECTION("size adjusts when pickupSoundFormID is present")
    {
      REQUIRE( record.getWriteSize() == 62 );

      record.pickupSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 72 );
    }

    SECTION("size adjusts when putdownSoundFormID is present")
    {
      REQUIRE( record.getWriteSize() == 62 );

      record.putdownSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 72 );
    }

    SECTION("size adjusts with length of keywords")
    {
      REQUIRE( record.getWriteSize() == 62 );

      record.keywordArray.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 82 );

      record.keywordArray.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 86 );
    }

    SECTION("size adjusts when description is present")
    {
      REQUIRE( record.getWriteSize() == 62 );

      record.description = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 72 );
    }

    SECTION("size adjusts with length of models")
    {
      REQUIRE( record.getWriteSize() == 62 );

      record.models.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 72 );

      record.models.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 82 );

      record.models.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 92 );
    }

    SECTION("size adjusts when templateArmorFormID is present")
    {
      REQUIRE( record.getWriteSize() == 62 );

      record.templateArmorFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 72 );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    StringTable dummy_table;
    dummy_table.addString(0x0000D057, "foo");

    SECTION("default: load record with name, keywords + description")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ArmourRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00012E49 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0009 );
      // -- record data
      REQUIRE( record.editorID == "ArmorIronCuirass" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
      REQUIRE( record.unknownOBND[0] == 0xEF );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xF0 );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0x00 );
      REQUIRE( record.unknownOBND[5] == 0x00 );
      REQUIRE( record.unknownOBND[6] == 0x11 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x10 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x0E );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000D057 );
      REQUIRE( record.enchantingFormID == 0 );
      REQUIRE( record.modelPath == "Armor\\Iron\\Male\\CuirassLightGND.nif" );
      REQUIRE( record.unknownMO2T.isPresent() );
      const auto MO2T = std::string_view(reinterpret_cast<const char*>(record.unknownMO2T.data()), record.unknownMO2T.size());
      REQUIRE( MO2T == "\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6"sv );
      REQUIRE_FALSE( record.unknownMO2S.isPresent() );
      REQUIRE( record.mod4Path == "Armor\\Iron\\F\\CuirassLightGND.nif" );
      REQUIRE( record.unknownMO4T.isPresent() );
      const auto MO4T = std::string_view(reinterpret_cast<const char*>(record.unknownMO4T.data()), record.unknownMO4T.size());
      REQUIRE( MO4T == "\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6"sv );
      REQUIRE_FALSE( record.unknownMO4S.isPresent() );
      REQUIRE( record.unknownBODT.isPresent() );
      const auto BODT = std::string_view(reinterpret_cast<const char*>(record.unknownBODT.data()), record.unknownBODT.size());
      REQUIRE( BODT == "\x04\0\0\0\0\x64s\0\x01\0\0\0"sv );
      REQUIRE_FALSE( record.unknownBOD2.isPresent() );
      REQUIRE( record.equipTypeFormID == 0 );
      REQUIRE( record.blockBashImpactDataSetFormID == 0 );
      REQUIRE( record.alternateBlockMaterialFormID == 0 );
      REQUIRE( record.pickupSoundFormID == 0 );
      REQUIRE( record.putdownSoundFormID == 0 );
      REQUIRE( record.unknownRNAM == 0x00000019 );
      REQUIRE( record.keywordArray.size() == 4 );
      REQUIRE( record.keywordArray[0] == 0x0006BBD2 );
      REQUIRE( record.keywordArray[1] == 0x0006BBE3 );
      REQUIRE( record.keywordArray[2] == 0x0006C0EC );
      REQUIRE( record.keywordArray[3] == 0x0008F959 );
      REQUIRE( record.description.isPresent() );
      REQUIRE( record.description.getType() == LocalizedString::Type::Index );
      REQUIRE( record.description.getIndex() == 0 );
      REQUIRE( record.models.size() == 1 );
      REQUIRE( record.models[0] == 0x00012E48 );
      REQUIRE( record.value == 125 );
      REQUIRE( record.weight == 30.0f );
      REQUIRE( record.unknownDNAM == 0x000009C4 );
      REQUIRE( record.templateArmorFormID == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0FAIL\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\x02GrmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x40\x01GrmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of VMAD can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0VMAD\x64\0\xFE"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0FAIL\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const auto data = "ARMO\x2E\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const auto data = "ARMO\x52\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const auto data = "ARMO\x3F\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0B\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ARMO, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "ARMO\x41\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0D\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ARMO, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of OBND can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const auto data = "ARMO\x4A\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MOD2 > 512")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\x02Grmor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MOD2 model paths")
    {
      const auto data = "ARMO\x6A\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: MOD2 model path is empty string")
    {
      const auto data = "ARMO\x1D\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x01\0\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MO2Ts")
    {
      const auto data = "ARMO\x6A\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MO2T can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MO2S can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MO2S\x21\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MOD4 > 512")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4\x01\x02Grmor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MOD4 model paths")
    {
      const auto data = "ARMO\x67\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: MOD4 model path is empty string")
    {
      const auto data = "ARMO\x1D\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4\x01\0\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MO4Ts")
    {
      const auto data = "ARMO\x6A\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MO4T can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MO4S can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MO4S\x21\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple BODTs")
    {
      const auto data = "ARMO\x52\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0BODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of BODT can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of BOD2 can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0BOD2\x0C\0\x04\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple YNAMs")
    {
      const auto data = "ARMO\x54\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0YNAM\x04\0\x52\xE9\x03\0YNAM\x04\0\x52\xE9\x03\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of YNAM can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0YNAM\x04\0\x52\xE9"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: YNAM is zero")
    {
      const auto data = "ARMO\x4A\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0YNAM\x04\0\0\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ZNAMs")
    {
      const auto data = "ARMO\x54\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0ZNAM\x04\0\x55\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of ZNAM can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0ZNAM\x04\0\x55\xE9"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: ZNAM is zero")
    {
      const auto data = "ARMO\x4A\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0ZNAM\x04\0\0\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple RNAMs")
    {
      const auto data = "ARMO\x4A\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of RNAM is not four")
    {
      {
        const auto data = "ARMO\x3F\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x03\0\x19\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ARMO, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "ARMO\x41\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x05\0\x19\0\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ARMO, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of RNAM can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing RNAM")
    {
      const auto data = "ARMO\x36\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple KSIZs")
    {
      const auto data = "ARMO\x60\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KSIZ is not four")
    {
      {
        const auto data = "ARMO\x3F\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x03\0\x04\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ARMO, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "ARMO\x41\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x05\0\x04\0\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ARMO, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before KSIZ can be read completely")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\0\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero and KWDA is missing")
    {
      const auto data = "ARMO\x2A\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\0\0\0\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no KWDA")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0FAIL\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing KWDA")
    {
      const auto data = "ARMO\x2A\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KWDA is not a multiple of four or it's zero")
    {
      {
        const auto data = "ARMO\x41\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x11\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ARMO, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "ARMO\x30\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\0\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ARMO, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of KWDA can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL is not four")
    {
      {
        const auto data = "ARMO\x3F\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x03\0H\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ARMO, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "ARMO\x41\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x05\0H.\x01\0\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ARMO, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of MODL can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0Hi"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const auto data = "ARMO\x4D\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44\x41TA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not eight")
    {
      {
        const auto data = "ARMO\x3F\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x07\0\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ARMO, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "ARMO\x41\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x09\0}\0\0\0\0\0\xF0\x41\0DNAM\x04\0\xC4\x09\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ARMO, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DATA sub record")
    {
      const auto data = "ARMO\x32\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DNAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DNAMs")
    {
      const auto data = "ARMO\x4A\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09\0\0DNAM\x04\0\xC4\x09\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DNAM is not four")
    {
      {
        const auto data = "ARMO\x3F\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x03\0\xC4\x09\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ARMO, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "ARMO\x41\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x05\0\xC4\x09\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ARMO, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ArmourRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DNAM can be read")
    {
      const auto data = "ARMO\x40\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41\x44NAM\x04\0\xC4\x09"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DNAM sub record")
    {
      const auto data = "ARMO\x36\x01\0\0\0\0\0\0\x49\x2E\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x11\0ArmorIronCuirass\0OBND\x0C\0\xEF\xFF\xF0\xFF\0\0\x11\0\x10\0\x0E\0FULL\x04\0W\xD0\0\0MOD2\x24\0Armor\\Iron\\Male\\CuirassLightGND.nif\0MO2T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6MOD4!\0Armor\\Iron\\F\\CuirassLightGND.nif\0MO4T$\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x1A\xB1\xD5\xD8\x64\x64s\0\xC1\xD3\x0E\xB6\xD0\xAC\x62\x9B\x64\x64s\0\xC1\xD3\x0E\xB6\x42ODT\x0C\0\x04\0\0\0\0\x64s\0\x01\0\0\0RNAM\x04\0\x19\0\0\0KSIZ\x04\0\x04\0\0\0KWDA\x10\0\xD2\xBB\x06\0\xE3\xBB\x06\0\xEC\xC0\x06\0Y\xF9\x08\0DESC\x04\0\0\0\0\0MODL\x04\0H.\x01\0DATA\x08\0}\0\0\0\0\0\xF0\x41"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ARMO, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ArmourRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }
}
