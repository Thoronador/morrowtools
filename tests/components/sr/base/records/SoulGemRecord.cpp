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
#include "../../../../../sr/base/records/SoulGemRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("SoulGemRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    SoulGemRecord record;

    REQUIRE( record.editorID.empty() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE( record.keywords.empty() );
    REQUIRE( record.value == 0 );
    REQUIRE( record.weight == 0.0f );
    REQUIRE( record.soulInside == 0 );
    REQUIRE( record.capacity == 0 );
    REQUIRE( record.linkedToFormID == 0 );
  }

  SECTION("equals")
  {
    SoulGemRecord a;
    SoulGemRecord b;

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

      SECTION("soulInside mismatch")
      {
        a.soulInside = 1;
        b.soulInside = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("capacity mismatch")
      {
        a.capacity = 1;
        b.capacity = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("linkedToFormID mismatch")
      {
        a.linkedToFormID = 0x01234567;
        b.linkedToFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    SoulGemRecord record;

    REQUIRE( record.getRecordType() == cSLGM );
  }

  SECTION("getWriteSize")
  {
    SoulGemRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 56 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 68 );
    }

    SECTION("size adjusts when name is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 66 );
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

    SECTION("size adjusts with length of keywords")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 76 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 80 );
    }

    SECTION("size adjusts when linkedToFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.linkedToFormID = 0x01234567;
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
    dummy_table.addString(0x0000593A, "foo");

    SECTION("default: load record with NAM0")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      SoulGemRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00020000 );
      REQUIRE( record.headerFormID == 0x0002E500 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000C );
      // -- record data
      REQUIRE( record.editorID == "SoulGemBlack" );
      REQUIRE( record.unknownOBND[0] == 0xFD );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xFD );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0xF5 );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x03 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x03 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x0B );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000593A );
      REQUIRE( record.modelPath == "Clutter\\SoulGem\\SoulGemBlack01.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3B"sv );
      REQUIRE( record.keywords.size() == 1 );
      REQUIRE( record.keywords[0] == 0x000937A3 );
      REQUIRE( record.value == 300 );
      REQUIRE( record.weight == 1.0f );
      REQUIRE( record.soulInside == 0 );
      REQUIRE( record.capacity == 5 );
      REQUIRE( record.linkedToFormID == 0x0002E504 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "SLGM\0\0\0\0\x20\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      SoulGemRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00020020 );
      REQUIRE( record.headerFormID == 0x0002E500 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000C );
      // -- record data
      REQUIRE( record.editorID.empty() );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0FAIL\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\x02SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\xC8\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0FAIL\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const std::string_view data = "SLGM\xB6\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const std::string_view data = "SLGM\xE0\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0B\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SLGM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SoulGemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0D\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SLGM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SoulGemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const std::string_view data = "SLGM\xD2\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\x02Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple model paths")
    {
      const std::string_view data = "SLGM\xF1\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: model path is empty string")
    {
      const std::string_view data = "SLGM\xA6\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x01\0\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "SLGM\xFE\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BMODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODT can be read")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple KSIZs")
    {
      const std::string_view data = "SLGM\xDC\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KSIZ is not four")
    {
      {
        const std::string_view data = "SLGM\xC7\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x03\0\x01\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SLGM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SoulGemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SLGM\xC9\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x05\0\x01\0\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SLGM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SoulGemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: KSIZ is zero")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\0\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero and KWDA is missing")
    {
      const std::string_view data = "SLGM\xBE\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\0\0\0\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no KWDA")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0FAIL\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing KWDA")
    {
      const std::string_view data = "SLGM\xBE\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KWDA is not a multiple of four or it's zero")
    {
      {
        const std::string_view data = "SLGM\xC9\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x05\0\xA3\x37\x09\0\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SLGM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SoulGemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SLGM\xC4\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x00\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SLGM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SoulGemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of KWDA can be read")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const std::string_view data = "SLGM\xD6\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3F\x44\x41TA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not eight")
    {
      {
        const std::string_view data = "SLGM\xC7\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x07\0\x2C\x01\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SLGM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SoulGemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SLGM\xC9\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x09\0\x2C\x01\0\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SLGM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SoulGemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple SOULs")
    {
      const std::string_view data = "SLGM\xCF\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SOUL is not one")
    {
      {
        const std::string_view data = "SLGM\xC7\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SLGM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SoulGemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SLGM\xC9\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x02\0\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SLGM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SoulGemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of SOUL can be read")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple SLCPs")
    {
      const std::string_view data = "SLGM\xCF\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SLCP is not one")
    {
      {
        const std::string_view data = "SLGM\xC7\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\0\0NAM0\x04\0\x04\xE5\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SLGM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SoulGemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SLGM\xC9\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x02\0\x05\0NAM0\x04\0\x04\xE5\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SLGM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SoulGemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of SLCP can be read")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple NAM0s")
    {
      const std::string_view data = "SLGM\xD2\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5\x02\0NAM0\x04\0\x04\xE5\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of NAM0 can be read")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\x04\xE5"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: NAM0 is zero")
    {
      const std::string_view data = "SLGM\xC8\0\0\0\0\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0D\0SoulGemBlack\0OBND\x0C\0\xFD\xFF\xFD\xFF\xF5\xFF\x03\0\x03\0\x0B\0FULL\x04\0\x3A\x59\0\0MODL\x23\0Clutter\\SoulGem\\SoulGemBlack01.nif\0MODT\x30\0\x02\0\0\0\x03\0\0\0\0\0\0\0\xC7\x96\xA2\xC2\x64\x64\x73\0\xF7\xBB\x84\x5B\xFF\x77\x91\xE4\x64\x64\x73\0\xF7\xBB\x84\x5B\xA8\x37\x08\xC1\x64\x64\x73\0\x26\x2C\x33\x3BKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA3\x37\x09\0DATA\x08\0\x2C\x01\0\0\0\0\x80\x3FSOUL\x01\0\0SLCP\x01\0\x05NAM0\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SLGM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoulGemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      SoulGemRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0x00020020;
      record.headerFormID = 0x0002E500;
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
      const std::string_view data = "SLGM\0\0\0\0\x20\0\x02\0\0\xE5\x02\0\x1B\x69\x55\0\x28\0\x0C\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
