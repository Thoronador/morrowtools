/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
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
#include "../../../../../sr/base/records/AlchemyPotionRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("AlchemyPotionRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    AlchemyPotionRecord record;

    REQUIRE( record.editorID.empty() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    REQUIRE( record.keywords.empty() );
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE_FALSE( record.unknownMODS.isPresent() );
    REQUIRE( record.pickupSoundFormID == 0 );
    REQUIRE( record.putdownSoundFormID == 0 );
    REQUIRE( record.equipTypeFormID == 0 );
    REQUIRE( record.weight == 0.0f );
    REQUIRE( record.value == 0 );
    REQUIRE( record.unknownThirdENIT == 0 );
    REQUIRE( record.addictionChance == 0.0f );
    REQUIRE( record.useSoundFormID == 0 );
    REQUIRE( record.effects.empty() );
  }

  SECTION("equals")
  {
    AlchemyPotionRecord a;
    AlchemyPotionRecord b;

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

      SECTION("OBND mismatch")
      {
        a.unknownOBND[11] = 12;
        b.unknownOBND[11] = 34;

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

      SECTION("equipTypeFormID mismatch")
      {
        a.equipTypeFormID = 0x01234567;
        b.equipTypeFormID = 0x0FEDCBA9;

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

      SECTION("value mismatch")
      {
        a.value = 1;
        b.value = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("unknownThirdENIT mismatch")
      {
        a.unknownThirdENIT = 1;
        b.unknownThirdENIT = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("addictionChance mismatch")
      {
        a.addictionChance = 1.0f;
        b.addictionChance = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("useSoundFormID mismatch")
      {
        a.useSoundFormID = 1;
        b.useSoundFormID = 2;

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
    AlchemyPotionRecord record;

    REQUIRE( record.getRecordType() == cALCH );
  }

  SECTION("getWriteSize")
  {
    AlchemyPotionRecord record;

    record.editorID = "foo";
    record.modelPath = "foo\\bar.nif";

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 82 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 94 );
    }

    SECTION("size adjusts when name is present")
    {
      REQUIRE( record.getWriteSize() == 82 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 92 );
    }

    SECTION("size adjusts with length of keywords")
    {
      REQUIRE( record.getWriteSize() == 82 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 102 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 106 );
    }

    SECTION("size adjusts with length of model path")
    {
      record.editorID = "foo";
      record.modelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 82 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 91 );
    }

    SECTION("size adjusts with length of MODT")
    {
      REQUIRE( record.getWriteSize() == 82 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 106 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 89 );
    }

    SECTION("size adjusts with length of MODS")
    {
      REQUIRE( record.getWriteSize() == 82 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODS\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 106 );

      {
        const std::string_view data = "MODS\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 89 );
    }

    SECTION("size adjusts when pickupSoundFormID is present")
    {
      REQUIRE( record.getWriteSize() == 82 );

      record.pickupSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 92 );
    }

    SECTION("size adjusts when putdownSoundFormID is present")
    {
      REQUIRE( record.getWriteSize() == 82 );

      record.putdownSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 92 );
    }

    SECTION("size adjusts when equipTypeFormID is present")
    {
      REQUIRE( record.getWriteSize() == 82 );

      record.equipTypeFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 92 );
    }

    SECTION("size adjusts with number of effects")
    {
      REQUIRE( record.getWriteSize() == 82 );

      record.effects.push_back(EffectBlock());
      REQUIRE( record.getWriteSize() == 110 );

      record.effects.push_back(EffectBlock());
      REQUIRE( record.getWriteSize() == 138 );
    }

    SECTION("deleted record has size zero")
    {
      record.headerFlags = BasicRecord::cDeletedFlag;
      REQUIRE( record.getWriteSize() == 0 );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    StringTable dummy_table;
    dummy_table.addString(0x000025A6, "foo");

    SECTION("default: load record with keyword + sounds")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      AlchemyPotionRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00034C5E );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000E );
      // -- record data
      REQUIRE( record.editorID == "Ale" );
      REQUIRE( record.unknownOBND[0] == 0xFD );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xFE );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0x02 );
      REQUIRE( record.unknownOBND[5] == 0x00 );
      REQUIRE( record.unknownOBND[6] == 0x03 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x03 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x18 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x000025A6 );
      REQUIRE( record.keywords.size() == 1 );
      REQUIRE( record.keywords[0] == 0x0008CDEA );
      REQUIRE( record.modelPath == "Clutter\\Ingredients\\Mead01.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5C"sv );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE( record.pickupSoundFormID == 0x0003EDBD );
      REQUIRE( record.putdownSoundFormID == 0x0003EDC0 );
      REQUIRE( record.equipTypeFormID == 0 );
      REQUIRE( record.weight == 0.5f );
      REQUIRE( record.value == 5 );
      REQUIRE( record.flags == 3 );
      REQUIRE( record.unknownThirdENIT == 0 );
      REQUIRE( record.useSoundFormID == 0x000B6435 );

      REQUIRE( record.effects.size() == 2 );

      REQUIRE( record.effects[0].effectFormID == 0x0003EB16 );
      REQUIRE( record.effects[0].magnitude == 15.0f );
      REQUIRE( record.effects[0].areaOfEffect == 0 );
      REQUIRE( record.effects[0].duration == 0 );
      REQUIRE( record.effects[0].unknownCTDA_CIS2s.empty() );

      REQUIRE( record.effects[1].effectFormID == 0x0010C645 );
      REQUIRE( record.effects[1].magnitude == 30.0f );
      REQUIRE( record.effects[1].areaOfEffect == 0 );
      REQUIRE( record.effects[1].duration == 30 );
      REQUIRE( record.effects[1].unknownCTDA_CIS2s.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0FAIL\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\x02Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x41\x01Ooh\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0FAIL\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const std::string_view data = "ALCH\x2F\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const std::string_view data = "ALCH\x53\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "ALCH\x40\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0B\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "ALCH\x42\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0D\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const std::string_view data = "ALCH\x4B\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple KSIZs")
    {
      const std::string_view data = "ALCH\x55\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KSIZ is not four")
    {
      {
        const std::string_view data = "ALCH\x40\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x03\0\x01\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "ALCH\x42\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x05\0\x01\0\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before KSIZ can be read completely")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\0\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero and KWDA is missing")
    {
      const std::string_view data = "ALCH\x37\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\0\0\0\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no KWDA")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0FAIL\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing KWDA")
    {
      const std::string_view data = "ALCH\x37\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KWDA is not a multiple of four or it's zero")
    {
      {
        const std::string_view data = "ALCH\x42\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x05\0\xEA\xCD\x08\0\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "ALCH\x3D\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\0\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of KWDA can be read")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\x02Glitter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple model paths")
    {
      const std::string_view data = "ALCH\x66\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: model path is empty string")
    {
      const std::string_view data = "ALCH\x23\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x01\0\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "ALCH\xB3\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CMODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODT can be read")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODS can be read")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CMODS\x21\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple YNAMs")
    {
      const std::string_view data = "ALCH\x4B\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0YNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of YNAM is not four")
    {
      {
        const std::string_view data = "ALCH\x40\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x03\0\xBD\xED\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x05\0\xBD\xED\x03\0\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of YNAM can be read")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: YNAM is zero")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\0\0\0\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ZNAMs")
    {
      const std::string_view data = "ALCH\x4B\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ZNAM is not four")
    {
      {
        const std::string_view data = "ALCH\x40\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x03\0\xC0\xED\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "ALCH\x42\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x05\0\xC0\xED\x03\0\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of ZNAM can be read")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: ZNAM is zero")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\0\0\0\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const std::string_view data = "ALCH\x4B\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x44\x41TA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not four")
    {
      {
        const std::string_view data = "ALCH\x40\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x03\0\0\0\0ENIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "ALCH\x42\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x05\0\0\0\0\x3F\0ENIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DATA sub record")
    {
      const std::string_view data = "ALCH\x37\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0ENIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ENITs")
    {
      const std::string_view data = "ALCH\x5B\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0ENIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ENIT is not 20")
    {
      {
        const std::string_view data = "ALCH\x40\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x13\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "ALCH\x42\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x15\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of ENIT can be read")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing ENIT sub record")
    {
      const std::string_view data = "ALCH\x27\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45\x46ID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EFID is not four")
    {
      {
        const std::string_view data = "ALCH\x40\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x03\0\x16\xEB\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "ALCH\x42\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x05\0\x16\xEB\x03\0\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of EFID can be read")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EFIT")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0FAIL\x0C\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EFIT is not twelve")
    {
      {
        const std::string_view data = "ALCH\x40\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0B\0\0\0\xF0\x41\0\0\0\0\x1E\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41\0\0\0\0\0\0\0\0EFID\x04\0\x45\xC6\x10\0EFIT\x0D\0\0\0\xF0\x41\0\0\0\0\x1E\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ALCH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AlchemyPotionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of EFIT can be read")
    {
      const std::string_view data = "ALCH\x41\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0EFID\x04\0\x16\xEB\x03\0EFIT\x0C\0\0\0\x70\x41"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: potion has no effects")
    {
      const std::string_view data = "ALCH\x09\x01\0\0\0\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0EDID\x04\0Ale\0OBND\x0C\0\xFD\xFF\xFE\xFF\x02\0\x03\0\x03\0\x18\0FULL\x04\0\xA6\x25\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEA\xCD\x08\0MODL\x1F\0Clutter\\Ingredients\\Mead01.nif\0MODT\x6C\0\x02\0\0\0\x08\0\0\0\0\0\0\0\x63\xDF\x5C\x01\x64\x64\x73\0\xBF\xFA\x25\xDA\xB9\xFB\x95\xCF\x64\x64\x73\0\xBF\xFA\x25\xDA\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0\xCE\x35\x3C\x09\x64\x64\x73\0\x26\x2C\x33\x3B\xFD\x5A\x85\x62\x64\x64\x73\0\xBF\xFA\x25\xDA\xEA\x21\x64\xA7\x64\x64\x73\0\x60\x4D\xDD\x5C\x98\xA1\xD1\x7F\x64\x64\x73\0\x60\x4D\xDD\x5C\xF9\x0E\x5C\x2E\x64\x64\x73\0\x60\x4D\xDD\x5CYNAM\x04\0\xBD\xED\x03\0ZNAM\x04\0\xC0\xED\x03\0DATA\x04\0\0\0\0\x3F\x45NIT\x14\0\x05\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\x35\x64\x0B\0"sv;
      REQUIRE( data.size() == 0x0109 + 24 );
      std::istringstream stream;
      stream.str(std::string(data));

      // read ALCH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AlchemyPotionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      AlchemyPotionRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0x00000020;
      record.headerFormID = 0x00034C5E;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x000E;
      // -- record data
      record.editorID = "FooDeletedBar";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "ALCH\0\0\0\0\x20\0\0\0\x5E\x4C\x03\0\x1B\x69\x55\0\x28\0\x0E\0"sv;
      REQUIRE( stream.str() == data );
    }
  }

  SECTION("flag queries")
  {
    AlchemyPotionRecord record;

    SECTION("doesAutoCalc")
    {
      REQUIRE( record.doesAutoCalc() );
      record.flags = AlchemyPotionRecord::cFlagNoAutoCalc;
      REQUIRE_FALSE( record.doesAutoCalc() );
    }

    SECTION("isFoodItem")
    {
      REQUIRE_FALSE( record.isFoodItem() );
      record.flags = AlchemyPotionRecord::cFlagFoodItem;
      REQUIRE( record.isFoodItem() );
    }

    SECTION("isMedicine")
    {
      REQUIRE_FALSE( record.isMedicine() );
      record.flags = AlchemyPotionRecord::cFlagMedicine;
      REQUIRE( record.isMedicine() );
    }

    SECTION("isPoison")
    {
      REQUIRE_FALSE( record.isPoison() );
      record.flags = AlchemyPotionRecord::cFlagPoison;
      REQUIRE( record.isPoison() );
    }
  }
}
