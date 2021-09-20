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
#include "../../../../../sr/base/records/IngredientRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("IngredientRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    IngredientRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.unknownVMAD.isPresent() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.keywords.empty() );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE_FALSE( record.unknownMODS.isPresent() );
    REQUIRE( record.pickupSoundFormID == 0 );
    REQUIRE( record.putdownSoundFormID == 0 );
    REQUIRE( record.value == 0 );
    REQUIRE( record.weight == 0.0f );
    REQUIRE( record.baseCost == 0 );
    REQUIRE( record.flags == 0 );
    REQUIRE( record.effects.empty() );
  }

  SECTION("equals")
  {
    IngredientRecord a;
    IngredientRecord b;

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

      SECTION("keywords mismatch")
      {
        a.keywords.push_back(0x01234567);

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

      SECTION("baseCost mismatch")
      {
        a.baseCost = 1;
        b.baseCost = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("flags mismatch")
      {
        a.flags = 1;
        b.flags = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("effects mismatch")
      {
        a.effects.push_back(EffectBlock());
        b.effects.clear();

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    IngredientRecord record;

    REQUIRE( record.getRecordType() == cINGR );
  }

  SECTION("getWriteSize")
  {
    IngredientRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 56 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 68 );
    }

    SECTION("size adjusts with length of VMAD")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "VMAD\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 80 );

      {
        const std::string_view data = "VMAD\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 63 );
    }

    SECTION("size adjusts when name is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 66 );
    }

    SECTION("size adjusts with length of keywords")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 76 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 80 );
    }

    SECTION("size adjusts with length of model path")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.modelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 74 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 83 );
    }

    SECTION("size adjusts with length of MODT")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 80 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 63 );
    }

    SECTION("size adjusts with length of MODS")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODS\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 80 );

      {
        const std::string_view data = "MODS\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 63 );
    }

    SECTION("size adjusts when pickupSoundFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.pickupSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 66 );
    }

    SECTION("size adjusts when putdownSoundFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.putdownSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 66 );
    }

    SECTION("size adjusts when effects are present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.effects.push_back(EffectBlock());
      REQUIRE( record.getWriteSize() == 84 );

      record.effects.push_back(EffectBlock());
      REQUIRE( record.getWriteSize() == 112 );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    StringTable dummy_table;
    dummy_table.addString(0x00007D9C, "foo");

    SECTION("default: load record with keyword + sounds")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      IngredientRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x000134AA );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0008 );
      // -- record data
      REQUIRE( record.editorID == "Thistle01" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
      REQUIRE( record.unknownOBND[0] == 0xF6 );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xFE );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0xFF );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x0B );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x1A );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x0B );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00007D9C );
      REQUIRE( record.keywords.size() == 1 );
      REQUIRE( record.keywords[0] == 0x0008CDEB );
      REQUIRE( record.modelPath == "Plants\\Thistle01.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA"sv );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE( record.pickupSoundFormID == 0x000519D5 );
      REQUIRE( record.putdownSoundFormID == 0x000519D1 );
      REQUIRE( record.value == 1 );
      REQUIRE( record.weight == 0.1f );
      REQUIRE( record.baseCost == 23 );
      REQUIRE( record.flags == 0 );

      REQUIRE( record.effects.size() == 4 );

      REQUIRE( record.effects[0].effectFormID == 0x0003EAEB );
      REQUIRE( record.effects[0].magnitude == 3.0f );
      REQUIRE( record.effects[0].areaOfEffect == 0 );
      REQUIRE( record.effects[0].duration == 60 );
      REQUIRE( record.effects[0].unknownCTDA_CIS2s.empty() );

      REQUIRE( record.effects[1].effectFormID == 0x00073F23 );
      REQUIRE( record.effects[1].magnitude == 2.0f );
      REQUIRE( record.effects[1].areaOfEffect == 0 );
      REQUIRE( record.effects[1].duration == 10 );
      REQUIRE( record.effects[1].unknownCTDA_CIS2s.empty() );

      REQUIRE( record.effects[2].effectFormID == 0x00090041 );
      REQUIRE( record.effects[2].magnitude == 3.0f );
      REQUIRE( record.effects[2].areaOfEffect == 0 );
      REQUIRE( record.effects[2].duration == 0 );
      REQUIRE( record.effects[2].unknownCTDA_CIS2s.empty() );

      REQUIRE( record.effects[3].effectFormID == 0x0003EB1E );
      REQUIRE( record.effects[3].magnitude == 2.0f );
      REQUIRE( record.effects[3].areaOfEffect == 0 );
      REQUIRE( record.effects[3].duration == 60 );
      REQUIRE( record.effects[3].unknownCTDA_CIS2s.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0FAIL\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\x02Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x25\x01Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of VMAD can be read")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0VMAD\x64\0\xFE"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0FAIL\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const std::string_view data = "INGR\x13\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const std::string_view data = "INGR\x37\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "INGR\x24\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0B\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "INGR\x26\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0D\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const std::string_view data = "INGR\x2F\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple KSIZs")
    {
      const std::string_view data = "INGR\x39\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KSIZ is not four")
    {
      {
        const std::string_view data = "INGR\x24\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x03\0\x01\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x05\0\x01\0\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before KSIZ can be read completely")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\0\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero and KWDA is missing")
    {
      const std::string_view data = "INGR\x1B\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\0\0\0\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no KWDA")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0FAIL\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing KWDA")
    {
      const std::string_view data = "INGR\x1B\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KWDA is not a multiple of four or it's zero")
    {
      {
        const std::string_view data = "INGR\x26\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x05\0\xEB\xCD\x08\0\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "INGR\x21\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\0\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of KWDA can be read")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\x02Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple model paths")
    {
      const std::string_view data = "INGR\x40\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAMODL\x15\0Plants\\Thistle01.nif\0YNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: model path is empty string")
    {
      const std::string_view data = "INGR\x11\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x01\0\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "INGR\x4F\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAMODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODT can be read")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODS can be read")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAMODS\x21\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple YNAMs")
    {
      const std::string_view data = "INGR\x2F\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0YNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of YNAM is not four")
    {
      {
        const std::string_view data = "INGR\x24\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x03\0\xD5\x19\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "INGR\x26\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x05\0\xD5\x19\x05\0\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of YNAM can be read")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: YNAM is zero")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\0\0\0\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ZNAMs")
    {
      const std::string_view data = "INGR\x2F\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ZNAM is not four")
    {
      {
        const std::string_view data = "INGR\x24\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x03\0\xD1\x19\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x05\0\xD1\x19\x05\0\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of ZNAM can be read")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: ZNAM is zero")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\0\0\0\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const std::string_view data = "INGR\x33\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not eight")
    {
      {
        const std::string_view data = "INGR\x24\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x07\0\x01\0\0\0\xCD\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x09\0\x01\0\0\0\xCD\xCC\xCC\x3D\0\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DATA sub record")
    {
      const std::string_view data = "INGR\x17\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0ENIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ENITs")
    {
      const std::string_view data = "INGR\x33\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0ENIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ENIT is not eight")
    {
      {
        const std::string_view data = "INGR\x24\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x07\0\x17\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "INGR\x26\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x09\0\x17\0\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of ENIT can be read")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing ENIT subrecord")
    {
      const std::string_view data = "INGR\x17\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45\x46ID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EFID is not four")
    {
      {
        const std::string_view data = "INGR\x24\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x03\0\x41\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x05\0\x41\0\x09\0\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of EFID can be read")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: record has not exactly four effect blocks")
    {
      {
        const std::string_view data = "INGR\x09\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "INGR\x41\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x1E\xEB\x09\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: no EFIT")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0FAIL\x0C\0\0\0\0\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EFIT is not twelve")
    {
      {
        const std::string_view data = "INGR\x24\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0B\0\0\0\0\x40\0\0\0\0\x3C\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "INGR\x26\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\x3C\0\0\0EFID\x04\0\x23\x3F\x07\0EFIT\x0C\0\0\0\0\x40\0\0\0\0\x0A\0\0\0EFID\x04\0\x41\0\x09\0EFIT\x0C\0\0\0\x40\x40\0\0\0\0\0\0\0\0EFID\x04\0\x1E\xEB\x03\0EFIT\x0D\0\0\0\0\x40\0\0\0\0\x3C\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read INGR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        IngredientRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of EFIT can be read")
    {
      const std::string_view data = "INGR\x25\x01\0\0\0\0\0\0\xAA\x34\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x0A\0Thistle01\0OBND\x0C\0\xF6\xFF\xFE\xFF\xFF\xFF\x0B\0\x1A\0\x0B\0FULL\x04\0\x9C\x7D\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEB\xCD\x08\0MODL\x15\0Plants\\Thistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAYNAM\x04\0\xD5\x19\x05\0ZNAM\x04\0\xD1\x19\x05\0DATA\x08\0\x01\0\0\0\xCD\xCC\xCC\x3D\x45NIT\x08\0\x17\0\0\0\0\0\0\0EFID\x04\0\xEB\xEA\x03\0EFIT\x0C\0\0\0\x40\x40"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read INGR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      IngredientRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("flag queries")
  {
    IngredientRecord record;

    SECTION("doesAutoCalc")
    {
      REQUIRE( record.doesAutoCalc() );
      record.flags = IngredientRecord::cFlagNoAutoCalc;
      REQUIRE_FALSE( record.doesAutoCalc() );
    }

    SECTION("isFoodItem")
    {
      REQUIRE_FALSE( record.isFoodItem() );
      record.flags = IngredientRecord::cFlagFoodItem;
      REQUIRE( record.isFoodItem() );
    }
  }
}
