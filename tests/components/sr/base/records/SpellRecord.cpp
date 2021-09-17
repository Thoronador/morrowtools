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
#include "../../../../../sr/base/records/SpellRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("SpellRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    SpellRecord record;

    REQUIRE( record.editorID.empty() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.menuDisplayObjectFormID == 0 );
    REQUIRE( record.equipTypeFormID == 0 );
    REQUIRE_FALSE( record.description.isPresent() );
    REQUIRE( record.castingCost == 0 );
    REQUIRE( record.flags == 0 );
    REQUIRE( record.type == 0 );
    REQUIRE( record.chargeTime == 0.0f );
    REQUIRE( record.castingType == 0 );
    REQUIRE( record.delivery == 0 );
    REQUIRE( record.castDuration == 0 );
    REQUIRE( record.range == 0 );
    REQUIRE( record.castingPerkFormID == 0 );
    REQUIRE( record.effects.empty() );
  }

  SECTION("equals")
  {
    SpellRecord a;
    SpellRecord b;

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

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("menuDisplayObjectFormID mismatch")
      {
        a.menuDisplayObjectFormID = 0x01234567;
        b.menuDisplayObjectFormID = 0x0FEDCBA9;

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

      SECTION("description mismatch")
      {
        a.description = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("castingCost mismatch")
      {
        a.castingCost = 1;
        b.castingCost = 2;

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

      SECTION("type mismatch")
      {
        a.type = 1;
        b.type = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("chargeTime mismatch")
      {
        a.chargeTime = 1.0f;
        b.chargeTime = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("castingType mismatch")
      {
        a.castingType = 1;
        b.castingType = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("delivery mismatch")
      {
        a.delivery = 1;
        b.delivery = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("castDuration mismatch")
      {
        a.castDuration = 1.0f;
        b.castDuration = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("range mismatch")
      {
        a.range = 1.0f;
        b.range = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("castingPerkFormID mismatch")
      {
        a.castingPerkFormID = 0x01234567;
        b.castingPerkFormID = 0x0FEDCBA9;

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
    SpellRecord record;

    REQUIRE( record.getRecordType() == cSPEL );
  }

  SECTION("getWriteSize")
  {
    SpellRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 80 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 92 );
    }

    SECTION("size adjusts when name is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 80 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 90 );
    }

    SECTION("size adjusts when menuDisplayObjectFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 80 );

      record.menuDisplayObjectFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 90 );
    }

    SECTION("size adjusts when description is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 80 );

      record.description = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 90 );
    }

    SECTION("size adjusts with length of effects")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 80 );

      record.effects.push_back(EffectBlock());
      REQUIRE( record.getWriteSize() == 108 );

      record.effects.push_back(EffectBlock());
      REQUIRE( record.getWriteSize() == 136 );
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
    dummy_table.addString(0x00001C1B, "foo");

    SECTION("default: load record with keyword + sounds")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      SpellRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00012FCD );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID == "Flames" );
      REQUIRE( record.unknownOBND[0] == 0xFF );
      REQUIRE( record.unknownOBND[1] == 0xEE );
      REQUIRE( record.unknownOBND[2] == 0xDD );
      REQUIRE( record.unknownOBND[3] == 0xCC );
      REQUIRE( record.unknownOBND[4] == 0xBB );
      REQUIRE( record.unknownOBND[5] == 0xAA );
      REQUIRE( record.unknownOBND[6] == 0x99 );
      REQUIRE( record.unknownOBND[7] == 0x88 );
      REQUIRE( record.unknownOBND[8] == 0x77 );
      REQUIRE( record.unknownOBND[9] == 0x66 );
      REQUIRE( record.unknownOBND[10] == 0x55 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00001C1B );
      REQUIRE( record.menuDisplayObjectFormID == 0x0004E264 );
      REQUIRE( record.equipTypeFormID == 0x00013F44 );
      REQUIRE( record.description.isPresent() );
      REQUIRE( record.description.getType() == LocalizedString::Type::Index );
      REQUIRE( record.description.getIndex() == 0 );
      // SPIT\x24\0 \x0E\0\0\0 \0\0\0\0 \0\0\0\0 \0\0\0\0 \x02\0\0\0 \x02\0\0\0 \0\0\0\0 \0\0\0\0 \xA8\x2C\x0F\0
      REQUIRE( record.castingCost == 14 );
      REQUIRE( record.flags == 0 );
      REQUIRE( record.type == 0 );
      REQUIRE( record.chargeTime == 0.0f );
      REQUIRE( record.castingType == 2 );
      REQUIRE( record.delivery == 2 );
      REQUIRE( record.castDuration == 0.0f );
      REQUIRE( record.range == 0.0f );
      REQUIRE( record.castingPerkFormID == 0x000F2CA8 );

      // EFID\x04\0 \xA9\x3C\x01\0 EFIT\x0C\0 \0\0\0\x41 \0\0\0\0 \x01\0\0\0 EFID\x04\0 \x2F\x39\x0F\0 EFIT\x0C\0 \0\0\xC6\x42 \0\0\0\0 \x0F\0\0\0
      REQUIRE( record.effects.size() == 2 );
      REQUIRE( record.effects[0].effectFormID == 0x00013CA9 );
      REQUIRE( record.effects[0].magnitude == 8.0f );
      REQUIRE( record.effects[0].areaOfEffect == 0.0f );
      REQUIRE( record.effects[0].duration == 1 );
      REQUIRE( record.effects[1].effectFormID == 0x000F392F );
      REQUIRE( record.effects[1].magnitude == 99.0f );
      REQUIRE( record.effects[1].areaOfEffect == 0.0f );
      REQUIRE( record.effects[1].duration == 15 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "SPEL\0\0\0\0\x20\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      SpellRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00000020 );
      REQUIRE( record.headerFormID == 0x00012FCD );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID.empty() );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0FAIL\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\x02Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\xA9\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0FAIL\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const std::string_view data = "SPEL\x91\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const std::string_view data = "SPEL\xC1\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "SPEL\xA8\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0B\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SPEL\xAA\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0D\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const std::string_view data = "SPEL\xB3\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MDOBs")
    {
      const std::string_view data = "SPEL\xB3\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MDOB is not four")
    {
      {
        const std::string_view data = "SPEL\xA8\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x03\0\x64\xE2\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SPEL\xAA\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x05\0\x64\xE2\x04\0\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of MDOB can be read")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: MDOB is zero")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\0\0\0\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ETYPs")
    {
      const std::string_view data = "SPEL\xB3\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ETYP is not four")
    {
      {
        const std::string_view data = "SPEL\xA8\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x03\0\x44\x3F\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SPEL\xAA\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x05\0\x44\x3F\x01\0\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of ETYP can be read")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: ETYP is zero")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\0\0\0\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing ETYP")
    {
      const std::string_view data = "SPEL\x9F\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DESCs")
    {
      const std::string_view data = "SPEL\xB3\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DESC is not four")
    {
      {
        const std::string_view data = "SPEL\xA8\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x03\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SPEL\xAA\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x05\0\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before DESC can be read completely")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DESC")
    {
      const std::string_view data = "SPEL\x9F\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple SPITs")
    {
      const std::string_view data = "SPEL\xD3\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SPIT is not 36")
    {
      {
        const std::string_view data = "SPEL\xA8\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x23\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SPEL\xAA\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x25\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of SPIT can be read")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing SPIT sub record")
    {
      const std::string_view data = "SPEL\x7F\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EFID is not four")
    {
      {
        const std::string_view data = "SPEL\xA8\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x03\0\xA9\x3C\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SPEL\xAA\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x05\0\xA9\x3C\x01\0\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of EFID can be read")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EFIT")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0FAIL\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EFIT is not twelve")
    {
      {
        const std::string_view data = "SPEL\xA8\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0B\0\0\0\0\x41\0\0\0\0\x01\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SPEL\xAA\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0D\0\0\0\0\x41\0\0\0\0\x01\0\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    // const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41\0\0\0\0\x01\0\0\0EFID\x04\0\x2F\x39\x0F\0EFIT\x0C\0\0\0\xC6\x42\0\0\0\0\x0F\0\0\0"sv;

    SECTION("corrupt data: stream ends before all of EFIT can be read")
    {
      const std::string_view data = "SPEL\xA9\0\0\0\0\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x07\0Flames\0OBND\x0C\0\xFF\xEE\xDD\xCC\xBB\xAA\x99\x88\x77\x66\x55\0FULL\x04\0\x1B\x1C\0\0MDOB\x04\0\x64\xE2\x04\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0EFID\x04\0\xA9\x3C\x01\0EFIT\x0C\0\0\0\0\x41"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      SpellRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0x00000020;
      record.headerFormID = 0x00012FCD;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0001;
      // -- record data
      record.editorID = "FooDeletedBar";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "SPEL\0\0\0\0\x20\0\0\0\xCD\x2F\x01\0\x1B\x69\x55\0\x28\0\x01\0"sv;
      REQUIRE( stream.str() == data );
    }
  }

  SECTION("flags query methods")
  {
    SpellRecord record;

    SECTION("doesAutoCalc")
    {
      REQUIRE( record.doesAutoCalc() );
      record.flags = SpellRecord::cFlagNoAutoCalc;
      REQUIRE_FALSE( record.doesAutoCalc() );
    }

    SECTION("isPCStartSpell")
    {
      REQUIRE_FALSE( record.isPCStartSpell() );
      record.flags = SpellRecord::cFlagPCStartSpell;
      REQUIRE( record.isPCStartSpell() );
    }

    SECTION("areaEffectIgnoresLOS")
    {
      REQUIRE_FALSE( record.areaEffectIgnoresLOS() );
      record.flags = SpellRecord::cFlagAreaEffectIgnoresLOS;
      REQUIRE( record.areaEffectIgnoresLOS() );
    }

    SECTION("ignoresResistance")
    {
      REQUIRE_FALSE( record.ignoresResistance() );
      record.flags = SpellRecord::cFlagIgnoreResistance;
      REQUIRE( record.ignoresResistance() );
    }

    SECTION("disallowsAbsorbAndReflect")
    {
      REQUIRE_FALSE( record.disallowsAbsorbAndReflect() );
      record.flags = SpellRecord::cFlagDisallowAbsorbReflect;
      REQUIRE( record.disallowsAbsorbAndReflect() );
    }

    SECTION("noDualCastModifications")
    {
      REQUIRE_FALSE( record.noDualCastModifications() );
      record.flags = SpellRecord::cFlagNoDualCastModifications;
      REQUIRE( record.noDualCastModifications() );
    }
  }
}
