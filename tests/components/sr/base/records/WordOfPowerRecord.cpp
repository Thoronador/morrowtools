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
#include "../../../../../sr/base/records/WordOfPowerRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("WordOfPowerRecord")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    WordOfPowerRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE_FALSE( record.translated.isPresent() );
  }

  SECTION("equals")
  {
    WordOfPowerRecord a;
    WordOfPowerRecord b;

    SECTION("equal with FULL")
    {
      a.editorID = "foo";
      a.name = LocalizedString(LocalizedString::lsIndex, 1234, "bar");
      a.translated = LocalizedString(LocalizedString::lsIndex, 1234, "baz");

      b.editorID = "foo";
      b.name = LocalizedString(LocalizedString::lsIndex, 1234, "bar");
      b.translated = LocalizedString(LocalizedString::lsIndex, 1234, "baz");

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("equal without FULL")
    {
      a.editorID = "foo";
      a.name = LocalizedString();
      a.translated = LocalizedString(LocalizedString::lsIndex, 1234, "baz");

      b.editorID = "foo";
      b.name = LocalizedString();
      b.translated = LocalizedString(LocalizedString::lsIndex, 1234, "baz");

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal with FULL")
    {
      // editorID mismatch
      a.editorID = "foo";
      a.name = LocalizedString(LocalizedString::lsIndex, 1234, "bar");
      a.translated = LocalizedString(LocalizedString::lsIndex, 1234, "baz");

      b.editorID = "bar";
      b.name = LocalizedString(LocalizedString::lsIndex, 1234, "bar");
      b.translated = LocalizedString(LocalizedString::lsIndex, 1234, "baz");

      REQUIRE_FALSE( a.equals(b) );
      REQUIRE_FALSE( b.equals(a) );

      // FULL mismatch
      a.editorID = "foo";
      a.name = LocalizedString(LocalizedString::lsString, 1234, "bar");
      a.translated = LocalizedString(LocalizedString::lsIndex, 1234, "baz");

      b.editorID = "foo";
      b.name = LocalizedString(LocalizedString::lsString, 1234, "quux");
      b.translated = LocalizedString(LocalizedString::lsIndex, 1234, "baz");

      REQUIRE_FALSE( a.equals(b) );
      REQUIRE_FALSE( b.equals(a) );

      // TNAM mismatch
      a.editorID = "foo";
      a.name = LocalizedString(LocalizedString::lsIndex, 1234, "bar");
      a.translated = LocalizedString(LocalizedString::lsString, 1234, "baz");

      b.editorID = "foo";
      b.name = LocalizedString(LocalizedString::lsIndex, 1234, "bar");
      b.translated = LocalizedString(LocalizedString::lsString, 1234, "not the same");

      REQUIRE_FALSE( a.equals(b) );
      REQUIRE_FALSE( b.equals(a) );
    }

    SECTION("unequal without FULL")
    {
      // editorID mismatch
      a.editorID = "foo";
      a.name = LocalizedString();
      a.translated = LocalizedString(LocalizedString::lsIndex, 1234, "baz");

      b.editorID = "bar";
      b.name = LocalizedString();
      b.translated = LocalizedString(LocalizedString::lsIndex, 1234, "baz");

      REQUIRE_FALSE( a.equals(b) );
      REQUIRE_FALSE( b.equals(a) );

      // TNAM mismatch
      a.editorID = "foo";
      a.name = LocalizedString();
      a.translated = LocalizedString(LocalizedString::lsString, 1234, "baz");

      b.editorID = "foo";
      b.name = LocalizedString();
      b.translated = LocalizedString(LocalizedString::lsString, 1234, "not the same");

      REQUIRE_FALSE( a.equals(b) );
      REQUIRE_FALSE( b.equals(a) );
    }
  }

  SECTION("getRecordType")
  {
    WordOfPowerRecord record;

    REQUIRE( record.getRecordType() == cWOOP );
  }

  SECTION("getWriteSize")
  {
    WordOfPowerRecord record;

    SECTION("without FULL")
    {
      record.editorID = "foobarfoobarbaz"; // 15 characters
      record.name = LocalizedString();
      record.translated = LocalizedString(LocalizedString::lsIndex, 1234, "");
      REQUIRE( record.getWriteSize() == 32 );

      record.editorID = "foo"; // three characters
      record.name = LocalizedString();
      record.translated = LocalizedString(LocalizedString::lsIndex, 1234, "");
      REQUIRE( record.getWriteSize() == 20 );

      record.editorID = "foo"; // three characters
      record.name = LocalizedString();
      record.translated = LocalizedString(LocalizedString::lsString, 0, "test case string");
      REQUIRE( record.getWriteSize() == 33 );
    }

    SECTION("with FULL")
    {
      record.editorID = "foobarfoobarbaz"; // 15 characters
      record.name = LocalizedString(LocalizedString::lsIndex, 123, "");
      record.translated = LocalizedString(LocalizedString::lsIndex, 1234, "");
      REQUIRE( record.getWriteSize() == 42 );

      record.editorID = "foobar"; // six characters
      record.name = LocalizedString(LocalizedString::lsIndex, 123, "");
      record.translated = LocalizedString(LocalizedString::lsIndex, 1234, "");
      REQUIRE( record.getWriteSize() == 33 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      record.name = LocalizedString(LocalizedString::lsString, 0, "test case string");
      record.translated = LocalizedString(LocalizedString::lsIndex, 1234, "");
      REQUIRE( record.getWriteSize() == 55 );
    }
  }

  SECTION("loadFromStream")
  {
    using namespace std::string_view_literals;

    uint32_t dummy = 0;
    StringTable dummy_table;

    SECTION("default: load record with FULL")
    {
      const std::string_view data = "WOOP\x2D\0\0\0\0\0\0\0\x67\x5F\x0E\0\x1B\x69\x55\0\x28\0\x03\0EDID\x13\0HowlCallOfTheWild3\0FULL\x04\0\xA2\x05\0\0TNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      dummy_table.addString(0x000005A2, "foo bar");

      // read WOOP, because header is handled before loadFromStream.
      stream.read((char*) &dummy, 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      WordOfPowerRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x000E5F67 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0003 );
      // -- record data
      REQUIRE( record.editorID == "HowlCallOfTheWild3" );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::lsIndex );
      REQUIRE( record.name.getIndex() == 0x000005A2 );
      REQUIRE( record.translated.isPresent() );
      REQUIRE( record.translated.getType() == LocalizedString::lsIndex );
      REQUIRE( record.translated.getIndex() == 0 );
    }

    SECTION("default: load record without FULL")
    {
      const std::string_view data = "WOOP\x1F\0\0\0\0\0\0\0\xCB\x6A\x10\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0F\0DragonFakeWord\0TNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WOOP, because header is handled before loadFromStream.
      stream.read((char*) &dummy, 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      WordOfPowerRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00106ACB );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0002 );
      // -- record data
      REQUIRE( record.editorID == "DragonFakeWord" );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.translated.isPresent() );
      REQUIRE( record.translated.getType() == LocalizedString::lsIndex );
      REQUIRE( record.translated.getIndex() == 0 );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "WOOP\x1F\0\0\0\0\0\0\0\xCB\x6A\x10\0\x1B\x69\x55\0\x28\0\x02\0FAIL\x0F\0DragonFakeWord\0TNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WOOP, because header is handled before loadFromStream.
      stream.read((char*) &dummy, 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WordOfPowerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "WOOP\x1F\0\0\0\0\0\0\0\xCB\x6A\x10\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0F\x02DragonFakeWord\0TNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WOOP, because header is handled before loadFromStream.
      stream.read((char*) &dummy, 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WordOfPowerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "WOOP\x1F\0\0\0\0\0\0\0\xCB\x6A\x10\0\x1B\x69\x55\0\x28\0\x02\0EDID\x19\0DragonFakeWord\0TNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WOOP, because header is handled before loadFromStream.
      stream.read((char*) &dummy, 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WordOfPowerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: FULL is FAIL")
    {
      const std::string_view data = "WOOP\x2D\0\0\0\0\0\0\0\x67\x5F\x0E\0\x1B\x69\x55\0\x28\0\x03\0EDID\x13\0HowlCallOfTheWild3\0FAIL\x04\0\xA2\x05\0\0TNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      dummy_table.addString(0x000005A2, "foo bar");

      // read WOOP, because header is handled before loadFromStream.
      stream.read((char*) &dummy, 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WordOfPowerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: FULL is not four bytes")
    {
      const std::string_view data = "WOOP\x2D\0\0\0\0\0\0\0\x67\x5F\x0E\0\x1B\x69\x55\0\x28\0\x03\0EDID\x13\0HowlCallOfTheWild3\0FULL\x03\0\xA2\x05\0\0TNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      dummy_table.addString(0x000005A2, "foo bar");

      // read WOOP, because header is handled before loadFromStream.
      stream.read((char*) &dummy, 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WordOfPowerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: TNAM is FAIL")
    {
      const std::string_view data = "WOOP\x1F\0\0\0\0\0\0\0\xCB\x6A\x10\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0F\0DragonFakeWord\0FAIL\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WOOP, because header is handled before loadFromStream.
      stream.read((char*) &dummy, 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WordOfPowerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: TNAM length is not four bytes")
    {
      const std::string_view data = "WOOP\x1F\0\0\0\0\0\0\0\xCB\x6A\x10\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0F\0DragonFakeWord\0TNAM\x07\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WOOP, because header is handled before loadFromStream.
      stream.read((char*) &dummy, 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WordOfPowerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    uint32_t dummy = 0;
    StringTable dummy_table;

    SECTION("save record with FULL")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      WordOfPowerRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0;
      record.headerFormID = 0x000E5F67;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0003;
      // -- record data
      record.editorID = "HowlCallOfTheWild3";
      record.name = LocalizedString(LocalizedString::lsIndex, 0x000005A2, "");
      record.translated = LocalizedString(LocalizedString::lsIndex, 0, "");

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "WOOP\x2D\0\0\0\0\0\0\0\x67\x5F\x0E\0\x1B\x69\x55\0\x28\0\x03\0EDID\x13\0HowlCallOfTheWild3\0FULL\x04\0\xA2\x05\0\0TNAM\x04\0\0\0\0\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save record without FULL")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      WordOfPowerRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0;
      record.headerFormID = 0x00106ACB;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0002;
      // -- record data
      record.editorID = "DragonFakeWord";
      record.name = LocalizedString();
      record.translated = LocalizedString(LocalizedString::lsIndex, 0, "");

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "WOOP\x1F\0\0\0\0\0\0\0\xCB\x6A\x10\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0F\0DragonFakeWord\0TNAM\x04\0\0\0\0\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
