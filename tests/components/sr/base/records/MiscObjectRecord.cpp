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
#include "../../../../../sr/base/records/MiscObjectRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("MiscObjectRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    MiscObjectRecord record;

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
    REQUIRE( record.iconPath.empty() );
    REQUIRE( record.keywords.empty() );
    REQUIRE( record.pickupSoundFormID == 0 );
    REQUIRE( record.putdownSoundFormID == 0 );
    REQUIRE( record.value == 0 );
    REQUIRE( record.weight == 0.0f );
  }

  SECTION("equals")
  {
    MiscObjectRecord a;
    MiscObjectRecord b;

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

      SECTION("icon path mismatch")
      {
        a.iconPath = "foo\\bar.dds";
        b.iconPath = "foo2\\bar3.dds";

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
    }
  }

  SECTION("getRecordType")
  {
    MiscObjectRecord record;

    REQUIRE( record.getRecordType() == cMISC );
  }

  SECTION("getWriteSize")
  {
    MiscObjectRecord record;

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

    SECTION("size adjusts when name is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 42 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 52 );
    }

    SECTION("size adjusts with length of model path")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 42 );

      record.modelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 60 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 69 );
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

    SECTION("size adjusts with length of MODS")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 42 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODS\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 66 );

      {
        const std::string_view data = "MODS\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 49 );
    }

    SECTION("size adjusts with length of icon path")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 42 );

      record.iconPath = "foo\\bar.dds";
      REQUIRE( record.getWriteSize() == 60 );

      record.iconPath = "foo\\barbarbarbar.dds";
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts when pickupSoundFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 42 );

      record.pickupSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 52 );
    }

    SECTION("size adjusts when putdownSoundFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 42 );

      record.putdownSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 52 );
    }

    SECTION("size adjusts with length of keywords")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 42 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 62 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 66 );
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
    dummy_table.addString(0x0000D502, "foo");

    SECTION("default: load record with keyword + sounds")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      MiscObjectRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0000000F );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000C );
      // -- record data
      REQUIRE( record.editorID == "Gold001" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
      REQUIRE( record.unknownOBND[0] == 0xFE );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xFE );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0x00 );
      REQUIRE( record.unknownOBND[5] == 0x00 );
      REQUIRE( record.unknownOBND[6] == 0x02 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x02 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x00 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000D502 );
      REQUIRE( record.modelPath == "Clutter\\Coin01.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0"sv );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE( record.iconPath == "Clutter\\Coin01.dds" );
      REQUIRE( record.pickupSoundFormID == 0x0003E952 );
      REQUIRE( record.putdownSoundFormID == 0x0003E955 );
      REQUIRE( record.keywords.size() == 1 );
      REQUIRE( record.keywords[0] == 0x000914E9 );
      REQUIRE( record.value == 1 );
      REQUIRE( record.weight == 0.0f );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "MISC\0\0\0\0\x20\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      MiscObjectRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00000020 );
      REQUIRE( record.headerFormID == 0x0000000F );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000C );
      // -- record data
      REQUIRE( record.editorID.empty() );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0FAIL\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\x02Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\xD4\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of VMAD can be read")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0VMAD\x64\0\xFE"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0FAIL\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const std::string_view data = "MISC\xBC\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const std::string_view data = "MISC\xE6\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "MISC\xD3\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0B\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MISC, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MiscObjectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "MISC\xD5\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0D\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MISC, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MiscObjectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const std::string_view data = "MISC\xDE\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\x02Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple model paths")
    {
      const std::string_view data = "MISC\xEF\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: model path is empty string")
    {
      const std::string_view data = "MISC\xC2\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x01\0\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "MISC\x04\x01\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODT can be read")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ICON > 512")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\x02Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple icon paths")
    {
      const std::string_view data = "MISC\xED\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: icon path is empty string")
    {
      const std::string_view data = "MISC\xC2\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x01\0\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple YNAMs")
    {
      const std::string_view data = "MISC\xDE\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of YNAM is not four")
    {
      {
        const std::string_view data = "MISC\xD3\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x03\0\x52\xE9\x03ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MISC, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MiscObjectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "MISC\xD5\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x05\0\x52\xE9\x03\0\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MISC, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MiscObjectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of YNAM can be read")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: YNAM is zero")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\0\0\0\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ZNAMs")
    {
      const std::string_view data = "MISC\xDE\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ZNAM is not four")
    {
      {
        const std::string_view data = "MISC\xD3\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x03\0\x55\xE9\x03KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MISC, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MiscObjectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x05\0\x55\xE9\x03\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MISC, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MiscObjectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of ZNAM can be read")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: ZNAM is zero")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple KSIZs")
    {
      const std::string_view data = "MISC\xE8\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KSIZ is not four")
    {
      {
        const std::string_view data = "MISC\xD3\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x03\0\x01\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MISC, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MiscObjectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "MISC\xD5\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x05\0\x01\0\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MISC, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MiscObjectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: KSIZ is zero")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\0\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero and KWDA is missing")
    {
      const std::string_view data = "MISC\xCA\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\0\0\0\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no KWDA")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0FAIL\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing KWDA")
    {
      const std::string_view data = "MISC\xCA\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KWDA is not a multiple of four or it's zero")
    {
      {
        const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x05\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MISC, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MiscObjectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "MISC\xD0\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\0\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MISC, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MiscObjectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of KWDA can be read")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const std::string_view data = "MISC\xE0\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0\0\0DATA\x08\0\x01\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not eight")
    {
      {
        const std::string_view data = "MISC\xD3\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x07\0\x01\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MISC, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MiscObjectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x09\0\x01\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MISC, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MiscObjectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const std::string_view data = "MISC\xD4\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0DATA\x08\0\x01\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DATA sub record")
    {
      const std::string_view data = "MISC\xC8\0\0\0\0\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x08\0Gold001\0OBND\x0C\0\xFE\xFF\xFE\xFF\0\0\x02\0\x02\0\0\0FULL\x04\0\x02\xD5\0\0MODL\x13\0Clutter\\Coin01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x02\xF5\xFD\x1F\x64\x64\x73\0\xBF\xFA\x25\xDA\x28\x76\xC8\x68\x64\x64\x73\0\xBF\xFA\x25\xDA\x18\x8F\xFC\x7C\x64\x64\x73\0\x26\x2C\x33\x3B\xFA\xE0\xBB\xA4\x64\x64\x73\0\x7F\x66\xA5\xC0ICON\x13\0Clutter\\Coin01.dds\0YNAM\x04\0\x52\xE9\x03\0ZNAM\x04\0\x55\xE9\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE9\x14\x09\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      MiscObjectRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0x00000020;
      record.headerFormID = 0x0000000F;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x000C;
      // -- record data
      record.editorID = "FooDeletedBar";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "MISC\0\0\0\0\x20\0\0\0\x0F\0\0\0\x1B\x69\x55\0\x28\0\x0C\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
