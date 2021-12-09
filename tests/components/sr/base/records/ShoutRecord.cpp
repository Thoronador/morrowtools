/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
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

#include <catch.hpp>
#include <sstream>
#include <string_view>
#include "../../../../../sr/base/records/ShoutRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("ShoutRecord")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    ShoutRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.menuDisplayObjectFormID == 0 );
    REQUIRE_FALSE( record.description.isPresent() );
    REQUIRE( record.words.empty() );
  }

  SECTION("equals")
  {
    ShoutRecord a;
    ShoutRecord b;

    SECTION("equal")
    {
      a.editorID = "foo";
      a.name = LocalizedString(LocalizedString::Type::Index, 123, "");
      a.menuDisplayObjectFormID = 0x78563421;
      a.description = LocalizedString(LocalizedString::Type::Index, 456, "");
      a.words = { { 1, 2, 5.0f }, { 3, 4, 6.0f } };

      b.editorID = "foo";
      b.name = LocalizedString(LocalizedString::Type::Index, 123, "");
      b.menuDisplayObjectFormID = 0x78563421;
      b.description = LocalizedString(LocalizedString::Type::Index, 456, "");
      b.words = { { 1, 2, 5.0f }, { 3, 4, 6.0f } };

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

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 123, "");
        b.name = LocalizedString(LocalizedString::Type::Index, 987, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MDOB mismatch")
      {
        a.menuDisplayObjectFormID = 0x78563421;
        b.menuDisplayObjectFormID = 0x77777777;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("description mismatch")
      {
        a.description = LocalizedString(LocalizedString::Type::Index, 123, "");
        b.description = LocalizedString(LocalizedString::Type::Index, 987, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("words mismatch")
      {
        a.words = { { 1, 2, 5.0f }, { 3, 4, 6.0f } };
        b.words = { { 11, 2, 5.0f }, { 3, 4, 6.0f } };

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    ShoutRecord record;

    REQUIRE( record.getRecordType() == cSHOU );
  }

  SECTION("getWriteSize")
  {
    ShoutRecord record;
    record.description = LocalizedString(LocalizedString::Type::Index, 123, "");

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 32 );

      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 20 );
    }

    SECTION("size increases when name is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 20 );

      record.name = LocalizedString(LocalizedString::Type::Index, 123, "");
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size increases with menu display object ID")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 20 );

      record.menuDisplayObjectFormID = 0x00112233;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size increases with number of words")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 20 );

      record.words.push_back(ShoutRecord::WordEntry());
      REQUIRE( record.getWriteSize() == 38 );
      record.words.push_back(ShoutRecord::WordEntry());
      REQUIRE( record.getWriteSize() == 56 );
      record.words.push_back(ShoutRecord::WordEntry());
      REQUIRE( record.getWriteSize() == 74 );
    }

    SECTION("deleted record has size zero")
    {
      record.headerFlags = BasicRecord::cDeletedFlag;
      REQUIRE( record.getWriteSize() == 0 );
    }
  }

  SECTION("loadFromStream")
  {
    using namespace std::string_view_literals;

    uint32_t dummy = 0;
    StringTable dummy_table;
    dummy_table.addString(0x000003B2, "foo");
    dummy_table.addString(0x000003AF, "bar");

    SECTION("default: load record with FULL")
    {
      const std::string_view data = "SHOU\x70\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0FULL\x04\0\xB2\x03\0\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ShoutRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00013E07 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0005 );
      // -- record data
      REQUIRE( record.editorID == "UnrelentingForceShout" );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x000003B2 );
      REQUIRE( record.menuDisplayObjectFormID == 0x000A59AC );
      REQUIRE( record.description.getType() == LocalizedString::Type::Index );
      REQUIRE( record.description.getIndex() == 0x000003AF );
      REQUIRE( record.words.size() == 3 );
      REQUIRE( record.words[0].wordFormID == 0x00013E22 );
      REQUIRE( record.words[0].spellFormID == 0x00013E09 );
      REQUIRE( record.words[0].recharge == 15.0f );
      REQUIRE( record.words[1].wordFormID == 0x00013E23 );
      REQUIRE( record.words[1].spellFormID == 0x00013F39 );
      REQUIRE( record.words[1].recharge == 20.0f );
      REQUIRE( record.words[2].wordFormID == 0x00013E24 );
      REQUIRE( record.words[2].spellFormID == 0x00013F3A );
      REQUIRE( record.words[2].recharge == 45.0f );
    }

    SECTION("default: load record without FULL")
    {
      const std::string_view data = "SHOU\x66\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ShoutRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00013E07 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0005 );
      // -- record data
      REQUIRE( record.editorID == "UnrelentingForceShout" );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.menuDisplayObjectFormID == 0x000A59AC );
      REQUIRE( record.description.getType() == LocalizedString::Type::Index );
      REQUIRE( record.description.getIndex() == 0x000003AF );
      REQUIRE( record.words.size() == 3 );
      REQUIRE( record.words[0].wordFormID == 0x00013E22 );
      REQUIRE( record.words[0].spellFormID == 0x00013E09 );
      REQUIRE( record.words[0].recharge == 15.0f );
      REQUIRE( record.words[1].wordFormID == 0x00013E23 );
      REQUIRE( record.words[1].spellFormID == 0x00013F39 );
      REQUIRE( record.words[1].recharge == 20.0f );
      REQUIRE( record.words[2].wordFormID == 0x00013E24 );
      REQUIRE( record.words[2].spellFormID == 0x00013F3A );
      REQUIRE( record.words[2].recharge == 45.0f );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "SHOU\x70\0\0\0\0\0\0\0\x07\x3E"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "SHOU\x70\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0FAIL\x16\0UnrelentingForceShout\0FULL\x04\0\xB2\x03\0\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "SHOU\x70\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\x02UnrelentingForceShout\0FULL\x04\0\xB2\x03\0\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "SHOU\x70\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x6B\0UnrelentingForceShout\0FULL\x04\0\xB2\x03\0\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no FULL")
    {
      const std::string_view data = "SHOU\x70\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0FAIL\x04\0\xB2\x03\0\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL")
    {
      const std::string_view data = "SHOU\x7A\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0FULL\x04\0\xB2\x03\0\0FULL\x04\0\xB2\x03\0\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before FULL can be read completely")
    {
      const std::string_view data = "SHOU\x70\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0FULL\x04\0\xB2\x03"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MDOB")
    {
      const std::string_view data = "SHOU\x70\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0MDOB\x04\0\xAC\x59\x0A\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before MDOB can be read completely")
    {
      const std::string_view data = "SHOU\x70\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0FULL\x04\0\xB2\x03\0\0MDOB\x04\0\xAC\x59"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: MDOB is zero")
    {
      const std::string_view data = "SHOU\x66\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0MDOB\x04\0\0\0\0\0DESC\x04\0\xAF\x03\0\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DESC")
    {
      const std::string_view data = "SHOU\x70\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0DESC\x04\0\xAF\x03\0\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no DESC")
    {
      const std::string_view data = "SHOU\x66\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0FULL\x04\0\xB2\x03\0\0MDOB\x04\0\xAC\x59\x0A\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before DESC can be read completely")
    {
      const std::string_view data = "SHOU\x66\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SNAM is not twelve")
    {
      {
        const std::string_view data = "SHOU\x65\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0SNAM\x0B\0\x22\x3E\x01\0\x09\x3E\x01\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SHOU, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ShoutRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "SHOU\x67\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0SNAM\x0D\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41\0SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SHOU, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ShoutRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before SNAM can be read completely")
    {
      const std::string_view data = "SHOU\x66\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: less than three words")
    {
      const std::string_view data = "SHOU\x5D\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0FULL\x04\0\xB2\x03\0\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SHOU, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ShoutRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    SECTION("save record with FULL")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      ShoutRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0;
      record.headerFormID = 0x00013E07;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0005;
      // -- record data
      record.editorID = "UnrelentingForceShout";
      record.name = LocalizedString(LocalizedString::Type::Index, 0x000003B2, "");
      record.menuDisplayObjectFormID = 0x000A59AC;
      record.description = LocalizedString(LocalizedString::Type::Index, 0x000003AF, "");
      record.words.clear();
      record.words.push_back({ 0x00013E22, 0x00013E09, 15.0f });
      record.words.push_back({ 0x00013E23, 0x00013F39, 20.0f });
      record.words.push_back({ 0x00013E24, 0x00013F3A, 45.0f });

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "SHOU\x70\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0FULL\x04\0\xB2\x03\0\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save record without FULL")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      ShoutRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0;
      record.headerFormID = 0x00013E07;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0005;
      // -- record data
      record.editorID = "UnrelentingForceShout";
      record.name = LocalizedString(LocalizedString::Type::None, 0, "");
      record.menuDisplayObjectFormID = 0x000A59AC;
      record.description = LocalizedString(LocalizedString::Type::Index, 0x000003AF, "");
      record.words.clear();
      record.words.push_back({ 0x00013E22, 0x00013E09, 15.0f });
      record.words.push_back({ 0x00013E23, 0x00013F39, 20.0f });
      record.words.push_back({ 0x00013E24, 0x00013F3A, 45.0f });

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "SHOU\x66\0\0\0\0\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0EDID\x16\0UnrelentingForceShout\0MDOB\x04\0\xAC\x59\x0A\0DESC\x04\0\xAF\x03\0\0SNAM\x0C\0\x22\x3E\x01\0\x09\x3E\x01\0\0\0\x70\x41SNAM\x0C\0\x23\x3E\x01\0\x39\x3F\x01\0\0\0\xA0\x41SNAM\x0C\0\x24\x3E\x01\0\x3A\x3F\x01\0\0\0\x34\x42"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save deleted record")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      ShoutRecord record;
      // Fill data.
      // -- header
      record.headerFlags = BasicRecord::cDeletedFlag;
      record.headerFormID = 0x00013E07;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0005;
      // -- record data
      record.editorID = "UnrelentingForceShout";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "SHOU\0\0\0\0\x20\0\0\0\x07\x3E\x01\0\x1B\x69\x55\0\x28\0\x05\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
