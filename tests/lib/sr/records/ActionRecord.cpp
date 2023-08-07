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

#include "../../locate_catch.hpp"
#include <sstream>
#include <string_view>
#include "../../../../lib/sr/records/ActionRecord.hpp"
#include "../../../../lib/sr/SR_Constants.hpp"
#include "../../../../lib/sr/StringTable.hpp"

TEST_CASE("ActionRecord")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    ActionRecord record;

    REQUIRE( record.editorID.empty() );
  }

  SECTION("equals")
  {
    ActionRecord a;
    ActionRecord b;

    SECTION("equal")
    {
      a.editorID = "foo";
      b.editorID = "foo";

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      // editorID mismatch
      a.editorID = "foo";
      b.editorID = "bar";

      REQUIRE_FALSE( a.equals(b) );
      REQUIRE_FALSE( b.equals(a) );
    }
  }

  SECTION("getRecordType")
  {
    ActionRecord record;

    REQUIRE( record.getRecordType() == cAACT );
  }

  SECTION("getWriteSize")
  {
    ActionRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 22 );

      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 10 );
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

    SECTION("default: load record with EDID")
    {
      const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read AACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ActionRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00094065 );
      REQUIRE( record.headerRevision == 0x000C6011 );
      REQUIRE( record.headerVersion == 31 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID == "ActionShieldChange" );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "AACT\0\0\0\0\x20\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read AACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ActionRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.isDeleted() );
      REQUIRE( record.headerFlags == 0x20 );
      REQUIRE( record.headerFormID == 0x00094065 );
      REQUIRE( record.headerRevision == 0x000C6011 );
      REQUIRE( record.headerVersion == 31 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID.empty() );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read AACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0FAIL\x13\0ActionShieldChange\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read AACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\002ActionShieldChange\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read AACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x15\0ActionShieldChange\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read AACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    SECTION("save record with EDID")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      ActionRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0;
      record.headerFormID = 0x00094065;
      record.headerRevision = 0x000C6011;
      record.headerVersion = 31;
      record.headerUnknown5 = 0x0001;
      // -- record data
      record.editorID = "ActionShieldChange";

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save deleted record")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      ActionRecord record;
      // Fill data.
      // -- header
      record.headerFlags = BasicRecord::cDeletedFlag;
      record.headerFormID = 0x00094065;
      record.headerRevision = 0x000C6011;
      record.headerVersion = 31;
      record.headerUnknown5 = 0x0001;
      // -- record data
      record.editorID = "ActionShieldChange";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "AACT\0\0\0\0\x20\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
