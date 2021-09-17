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
#include "../../../../../sr/base/records/KeyRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("KeyRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    KeyRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.unknownVMAD.isPresent() );
    const auto obnd = std::array<uint8_t, 12>({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    REQUIRE( record.unknownOBND == obnd );
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE( record.pickupSoundFormID == 0 );
    REQUIRE( record.putdownSoundFormID == 0 );
    REQUIRE( record.keywords.empty() );
    REQUIRE( record.value == 0 );
    REQUIRE( record.weight == 0 );
  }

  SECTION("equals")
  {
    KeyRecord a;
    KeyRecord b;

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
        b.unknownVMAD.setPresence(false);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("OBND mismatch")
      {
        a.unknownOBND[11] = 2;
        b.unknownOBND[11] = 3;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 0x000003B2, "");
        b.name = LocalizedString(LocalizedString::Type::Index, 0x00000345, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("model path mismatch")
      {
        a.modelPath = "foo.nif";
        b.modelPath = "bar.nif";

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

      SECTION("pickupSoundFormID mismatch")
      {
        a.pickupSoundFormID = 0x000003B2;
        b.pickupSoundFormID = 0x12345678;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("putdownSoundFormID mismatch")
      {
        a.putdownSoundFormID = 0x000003B2;
        b.putdownSoundFormID = 0x12345678;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("keyword array mismatch")
      {
        a.keywords.clear();
        b.keywords.push_back(0x12345678);
        b.keywords.push_back(0x90ABCDEF);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("value mismatch")
      {
        a.value = 3;
        b.value = 5;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("weight mismatch")
      {
        a.weight = 3.0f;
        b.weight = 5.0f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    KeyRecord record;

    REQUIRE( record.getRecordType() == cKEYM );
  }

  SECTION("getWriteSize")
  {
    KeyRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 42 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 54 );
    }

    SECTION("size adjusts with length of VMAD")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 42 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "VMAD\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 66 );

      {
        const std::string_view data = "VMAD\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 49 );
    }

    SECTION("size adjusts with presence of name")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 42 );

      record.name = LocalizedString(LocalizedString::Type::Index, 0x000003B2, "");
      REQUIRE( record.getWriteSize() == 52 );
    }

    SECTION("size adjusts with length of model path")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 42 );

      record.modelPath = "foo.nif";
      REQUIRE( record.getWriteSize() == 56 );
      record.modelPath = "foobar.nif";
      REQUIRE( record.getWriteSize() == 59 );
    }

    SECTION("size adjusts with length of MODT")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 42 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 66 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 49 );
    }

    SECTION("size adjusts with presence of sound form IDs")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 42 );

      record.pickupSoundFormID = 0x000003B2;
      REQUIRE( record.getWriteSize() == 52 );
      record.putdownSoundFormID = 0x000003B2;
      REQUIRE( record.getWriteSize() == 62 );
    }

    SECTION("size adjusts with presence of keywords")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 42 );

      record.keywords.push_back(0x00000001);
      REQUIRE( record.getWriteSize() == 62 );
      record.keywords.push_back(0x0000000F);
      REQUIRE( record.getWriteSize() == 66 );
      record.keywords.push_back(0x0000AFFE);
      REQUIRE( record.getWriteSize() == 70 );
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
    dummy_table.addString(0x0000A2BA, "foo");

    SECTION("default: load record + save it again")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream streamIn;
      streamIn.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      streamIn.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( streamIn.good() );

      // Reading should succeed.
      KeyRecord record;
      REQUIRE( record.loadFromStream(streamIn, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0001C250 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000A );
      // -- record data
      REQUIRE( record.editorID == "SR01Key" );
      REQUIRE( record.unknownOBND[0] == 0x00 );
      REQUIRE( record.unknownOBND[1] == 0x00 );
      REQUIRE( record.unknownOBND[2] == 0xF4 );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0xFE );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x00 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x0C );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x02 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000A2BA );
      REQUIRE( record.modelPath == "Clutter\\Key01.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      REQUIRE( record.unknownMODT.size() == 60 );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0"sv);
      REQUIRE( record.pickupSoundFormID == 0x0003ED75 );
      REQUIRE( record.putdownSoundFormID == 0x0003ED78 );
      REQUIRE( record.keywords.size() == 1 );
      REQUIRE( record.keywords[0] == 0x000914EF );
      REQUIRE( record.value == 0 );
      REQUIRE( record.weight == 0.0f );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0FAIL\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\x02SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "KEYM\xB5\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0FAIL\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of VMAD can be read")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0VMAD\x0C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0FAIL\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before OBND can be read")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0D\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read KEYM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        KeyRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0B\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read KEYM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        KeyRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before FULL can be read")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no MODL")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0FAIL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODLs")
    {
      const std::string_view data = "KEYM\xD2\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: MODL is empty string")
    {
      const std::string_view data = "KEYM\xA9\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x01\0\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\x02Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "KEYM\xFC\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before MODT can be read")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple YNAMs")
    {
      const std::string_view data = "KEYM\xC4\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of YNAM can be read")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: YNAM is zero")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\0\0\0\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ZNAMs")
    {
      const std::string_view data = "KEYM\xC4\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of ZNAM can be read")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: ZNAM is zero")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple KSIZ")
    {
      const std::string_view data = "KEYM\xCE\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KSIZ is not four")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x05\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of KSIZ can be read")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no KWDA after KSIZ")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0FAIL\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of KWDA can be read")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KWDA is not a multiple of four")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x03\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATA subrecords")
    {
      const std::string_view data = "KEYM\xC8\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0\0\0\0\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not eight")
    {
      {
        const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x07\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read KEYM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        KeyRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x09\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read KEYM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        KeyRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before DATA subrecord can be read")
    {
      const std::string_view data = "KEYM\xBA\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0DATA\x08\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DATA subrecord")
    {
      const std::string_view data = "KEYM\xAC\0\0\0\0\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x08\0SR01Key\0OBND\x0C\0\0\0\xF4\xFF\xFE\xFF\0\0\x0C\0\x02\0FULL\x04\0\xBA\xA2\0\0MODL\x12\0Clutter\\Key01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x7B\x24\xA2\x37\x64\x64\x73\0\xBF\xFA\x25\xDA\x8A\x7E\xE1\x67\x64\x64\x73\0\xBF\xFA\x25\xDA\x6B\xBB\x96\xD1\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0YNAM\x04\0\x75\xED\x03\0ZNAM\x04\0\x78\xED\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xEF\x14\x09\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KEYM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeyRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      KeyRecord record;
      // Fill data.
      // -- header
      record.headerFlags = BasicRecord::cDeletedFlag;
      record.headerFormID = 0x0001C250;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x000A;
      // -- record data
      record.editorID = "SR01Key";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "KEYM\0\0\0\0\x20\0\0\0\x50\xC2\x01\0\x1B\x69\x55\0\x28\0\x0A\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
