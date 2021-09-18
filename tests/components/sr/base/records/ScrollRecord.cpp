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
#include "../../../../../sr/base/records/ScrollRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("ScrollRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    ScrollRecord record;

    REQUIRE( record.editorID.empty() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.keywordArray.empty() );
    REQUIRE( record.menuDisplayObjectFormID == 0 );
    REQUIRE( record.equipTypeFormID == 0 );
    REQUIRE_FALSE( record.description.isPresent() );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE( record.value == 0 );
    REQUIRE( record.weight == 0.0f );
    for (unsigned int i = 0; i < 36; ++i)
    {
      REQUIRE( record.unknownSPIT[i] == 0 );
    }
    REQUIRE( record.effects.empty() );
  }

  SECTION("equals")
  {
    ScrollRecord a;
    ScrollRecord b;

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

      SECTION("modelPath mismatch")
      {
        a.modelPath = "foo\\bar.nif";
        b.modelPath = "foo\\baz.nif";;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MODT mismatch")
      {
        a.unknownMODT.setPresence(true);
        b.unknownMODT.setPresence(false);

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

      SECTION("SPIT mismatch")
      {
        a.unknownSPIT[35] = 12;
        b.unknownSPIT[35] = 34;

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
    ScrollRecord record;

    REQUIRE( record.getRecordType() == cSCRL );
  }

  SECTION("getWriteSize")
  {
    ScrollRecord record;

    record.editorID = "foo";
    record.modelPath = "foo\\bar.nif";

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 122 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 134 );
    }

    SECTION("size adjusts when name is present")
    {
      REQUIRE( record.getWriteSize() == 122 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 132 );
    }

    SECTION("size adjusts with length of keywords")
    {
      REQUIRE( record.getWriteSize() == 122 );

      record.keywordArray.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 142 );

      record.keywordArray.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 146 );
    }

    SECTION("size adjusts with length of MODT")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 122 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 146 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 129 );
    }

    SECTION("size adjusts with length of effects")
    {
      REQUIRE( record.getWriteSize() == 122 );

      record.effects.push_back(EffectBlock());
      REQUIRE( record.getWriteSize() == 150 );

      record.effects.push_back(EffectBlock());
      REQUIRE( record.getWriteSize() == 178 );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    StringTable dummy_table;
    dummy_table.addString(0x00002355, "foo");

    SECTION("default: load record")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ScrollRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0009659E );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000C );
      // -- record data
      REQUIRE( record.editorID == "RaiseZombieScroll" );
      // FULL\x04\0 \x55\x23\0\0
      REQUIRE( record.unknownOBND[0] == 0xF0 );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xFC );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0xFC );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x10 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x04 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x04 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00002355 );
      REQUIRE( record.keywordArray.size() == 1 );
      REQUIRE( record.keywordArray[0] == 0x000A0E57 );
      REQUIRE( record.menuDisplayObjectFormID == 0x00076E8F );
      REQUIRE( record.equipTypeFormID == 0x00013F44 );
      REQUIRE( record.description.isPresent() );
      REQUIRE( record.description.getType() == LocalizedString::Type::Index );
      REQUIRE( record.description.getIndex() == 0 );
      REQUIRE( record.modelPath == "Clutter\\Common\\Scroll01.nif" );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92"sv );
      REQUIRE( record.value == 25 );
      REQUIRE( record.weight == 0.5f );
      //const auto SPIT = std::string_view(reinterpret_cast<const char*>(record.unknownSPIT.data()), record.unknownSPIT.size());
      const auto SPIT = std::string_view(reinterpret_cast<const char*>(record.unknownSPIT), 36);
      REQUIRE( SPIT == "\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv );

      REQUIRE( record.effects.size() == 1 );
      REQUIRE( record.effects[0].effectFormID == 0x00065BD6 );
      REQUIRE( record.effects[0].magnitude == 4.0f );
      REQUIRE( record.effects[0].areaOfEffect == 0.0f );
      REQUIRE( record.effects[0].duration == 60 );
      REQUIRE( record.effects[0].unknownCTDA_CIS2s.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    // There seems to be no record with CTDA + CIS2 in the main ESM files, so no test.

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0FAIL\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\x02RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x06\x01RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0FAIL\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const std::string_view data = "SCRL\xF4\0\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const std::string_view data = "SCRL\x18\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "SCRL\x05\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0B\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SCRL\x07\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0D\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const std::string_view data = "SCRL\x10\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple KSIZs")
    {
      const std::string_view data = "SCRL\x1A\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KSIZ is not four")
    {
      {
        const std::string_view data = "SCRL\x05\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x03\0\x01\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SCRL\x07\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x05\0\x01\0\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before KSIZ can be read completely")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\0\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero and KWDA is missing")
    {
      const std::string_view data = "SCRL\xFC\0\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\0\0\0\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no KWDA")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0FAIL\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing KWDA")
    {
      const std::string_view data = "SCRL\xFC\0\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KWDA is not a multiple of four or it's zero")
    {
      {
        const std::string_view data = "SCRL\x04\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x02\0\x57\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SCRL\x02\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\0\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of KWDA can be read")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MDOBs")
    {
      const std::string_view data = "SCRL\x10\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MDOB is not four")
    {
      {
        const std::string_view data = "SCRL\x05\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x03\0\x8F\x6E\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SCRL\x07\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x05\0\x8F\x6E\x07\0\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of MDOB can be read")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: MDOB is zero")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\0\0\0\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ETYPs")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ETYP is not four")
    {
      {
        const std::string_view data = "SCRL\x05\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x03\0\x44\x3F\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SCRL\x07\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x05\0\x44\x3F\x01\0\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of ETYP can be read")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: ETYP is zero")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\0\0\0\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing ETYP")
    {
      const std::string_view data = "SCRL\xFC\0\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DESCs")
    {
      const std::string_view data = "SCRL\x10\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DESC is not four")
    {
      {
        const std::string_view data = "SCRL\x05\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x03\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SCRL\x07\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x05\0\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before DESC can be read completely")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DESC")
    {
      const std::string_view data = "SCRL\xFC\0\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODLs")
    {
      const std::string_view data = "SCRL\x28\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\x02Glitter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\x01Glitter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: MODL is empty")
    {
      const std::string_view data = "SCRL\xE5\0\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x01\0\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "SCRL\x30\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FMODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before MODT can be read completely")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const std::string_view data = "SCRL\x14\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3F\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no DATA")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x46\x41IL\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not eight")
    {
      {
        const std::string_view data = "SCRL\x05\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x07\0\x19\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SCRL\x07\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x09\0\x19\0\0\0\0\0\0\x3F\0SPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before DATA can be read completely")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DATA subrecord")
    {
      const std::string_view data = "SCRL\xF8\0\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92SPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple SPITs")
    {
      const std::string_view data = "SCRL\x30\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0SPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SPIT is not 36")
    {
      {
        const std::string_view data = "SCRL\x05\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x23\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SCRL\x07\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x25\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of SPIT can be read")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing SPIT sub record")
    {
      const std::string_view data = "SCRL\xDC\0\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EFID is not four")
    {
      {
        const std::string_view data = "SCRL\x05\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x03\0\xD6\x5B\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SCRL\x07\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x05\0\xD6\x5B\x06\0\0EFIT\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of EFID can be read")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EFIT")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0FAIL\x0C\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EFIT is not twelve")
    {
      {
        const std::string_view data = "SCRL\x05\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0B\0\0\0\x80\x40\0\0\0\0\x3C\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SCRL\x07\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0D\0\0\0\x80\x40\0\0\0\0\x3C\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SCRL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScrollRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of EFIT can be read")
    {
      const std::string_view data = "SCRL\x06\x01\0\0\0\0\0\0\x9E\x65\x09\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x12\0RaiseZombieScroll\0OBND\x0C\0\xF0\xFF\xFC\xFF\xFC\xFF\x10\0\x04\0\x04\0FULL\x04\0\x55\x23\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x57\x0E\x0A\0MDOB\x04\0\x8F\x6E\x07\0ETYP\x04\0\x44\x3F\x01\0DESC\x04\0\0\0\0\0MODL\x1C\0Clutter\\Common\\Scroll01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x6B\xC0\xAC\x7B\x64\x64\x73\0\x8E\xAE\xA7\x92\xB4\x06\x44\x8A\x64\x64\x73\0\x8E\xAE\xA7\x92\x44\x41TA\x08\0\x19\0\0\0\0\0\0\x3FSPIT\x24\0\x31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0EFID\x04\0\xD6\x5B\x06\0EFIT\x0C\0\0\0\x80\x40"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SCRL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScrollRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }
}
