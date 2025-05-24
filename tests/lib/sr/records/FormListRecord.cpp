/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2022, 2025  Dirk Stolle

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
#include "../../../../lib/sr/records/FormListRecord.hpp"
#include "../../../../lib/sr/SR_Constants.hpp"
#include "../../../../lib/sr/StringTable.hpp"
#include "../../limited_streambuf.hpp"

TEST_CASE("FormListRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    FormListRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE( record.listFormIDs.empty() );
  }

  SECTION("equals")
  {
    FormListRecord a;
    FormListRecord b;

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

      SECTION("listFormIDs mismatch")
      {
        a.listFormIDs.push_back(0x0F00BA12);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    FormListRecord record;

    REQUIRE( record.getRecordType() == cFLST );
  }

  SECTION("getWriteSize")
  {
    FormListRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 10 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 22 );
    }

    SECTION("size adjusts with length of list form IDs")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 10 );

      record.listFormIDs = { 0x01234567 };
      REQUIRE( record.getWriteSize() == 20 );

      record.listFormIDs = { 0x01234567, 0x0FEDCBA9 };
      REQUIRE( record.getWriteSize() == 30 );
    }
  }

  SECTION("loadFromStream")
  {
    StringTable dummy_table;

    SECTION("default: load record with empty list")
    {
      const auto data = "FLST\x14\0\0\0\0\0\0\0\x14\x0D\0\0\x17\x67\x0E\0\x27\0\x02\0EDID\x0E\0DraugrWeapons\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip FLST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      FormListRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00000D14 );
      REQUIRE( record.headerRevision == 0x000E6717 );
      REQUIRE( record.headerVersion == 39 );
      REQUIRE( record.headerUnknown5 == 0x0002 );
      // -- record data
      REQUIRE( record.editorID == "DraugrWeapons" );
      REQUIRE( record.listFormIDs.size() == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with non-empty form ID list")
    {
      const auto data = "FLST\x59\0\0\0\0\0\0\0\x34\x36\x01\0\x1C\x4C\x3C\0\x0F\0\x01\0EDID\x17\0TownHelgenResidentList\0LNAM\x04\0>6\x01\0LNAM\x04\0?6\x01\0LNAM\x04\0@6\x01\0LNAM\x04\0A6\x01\0LNAM\x04\0B6\x01\0LNAM\x04\0C6\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip FLST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      FormListRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00013634 );
      REQUIRE( record.headerRevision == 0x003C4C1C );
      REQUIRE( record.headerVersion == 15 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID == "TownHelgenResidentList" );
      REQUIRE( record.listFormIDs.size() == 6 );
      REQUIRE( record.listFormIDs[0] == 0x0001363E );
      REQUIRE( record.listFormIDs[1] == 0x0001363F );
      REQUIRE( record.listFormIDs[2] == 0x00013640 );
      REQUIRE( record.listFormIDs[3] == 0x00013641 );
      REQUIRE( record.listFormIDs[4] == 0x00013642 );
      REQUIRE( record.listFormIDs[5] == 0x00013643 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "FLST\x14\0\0\0\0\0\0\0\x14\x0D"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip FLST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FormListRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const auto data = "FLST\x14\0\0\0\0\0\0\0\x14\x0D\0\0\x17\x67\x0E\0\x27\0\x02\0FAIL\x0E\0DraugrWeapons\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip FLST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FormListRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const auto data = "FLST\x14\0\0\0\0\0\0\0\x14\x0D\0\0\x17\x67\x0E\0\x27\0\x02\0EDID\x0E\x02DraugrWeapons\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip FLST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FormListRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before EDID can be read completely")
    {
      const auto data = "FLST\x14\0\0\0\0\0\0\0\x14\x0D\0\0\x17\x67\x0E\0\x27\0\x02\0EDID\x0E\0Drau"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip FLST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FormListRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: unknown / invalid subrecord")
    {
      const auto data = "FLST\x59\0\0\0\0\0\0\0\x34\x36\x01\0\x1C\x4C\x3C\0\x0F\0\x01\0EDID\x17\0TownHelgenResidentList\0LNAM\x04\0>6\x01\0FAIL\x04\0?6\x01\0LNAM\x04\0@6\x01\0LNAM\x04\0A6\x01\0LNAM\x04\0B6\x01\0LNAM\x04\0C6\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip FLST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FormListRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of LNAM is not four")
    {
      {
        const auto data = "FLST\x58\0\0\0\0\0\0\0\x34\x36\x01\0\x1C\x4C\x3C\0\x0F\0\x01\0EDID\x17\0TownHelgenResidentList\0LNAM\x03\0>6\x01LNAM\x04\0?6\x01\0LNAM\x04\0@6\x01\0LNAM\x04\0A6\x01\0LNAM\x04\0B6\x01\0LNAM\x04\0C6\x01\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip FLST, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FormListRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "FLST\x5A\0\0\0\0\0\0\0\x34\x36\x01\0\x1C\x4C\x3C\0\x0F\0\x01\0EDID\x17\0TownHelgenResidentList\0LNAM\x05\0>6\x01\0\0LNAM\x04\0?6\x01\0LNAM\x04\0@6\x01\0LNAM\x04\0A6\x01\0LNAM\x04\0B6\x01\0LNAM\x04\0C6\x01\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip FLST, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FormListRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of LNAM can be read")
    {
      const auto data = "FLST\x59\0\0\0\0\0\0\0\x34\x36\x01\0\x1C\x4C\x3C\0\x0F\0\x01\0EDID\x17\0TownHelgenResidentList\0LNAM\x04\0>6\x01\0LNAM\x04\0?6"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip FLST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FormListRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

    SECTION("saveToStream")
  {
    SECTION("failure: cannot write header data")
    {
      FormListRecord record;
      // Set some header data.
      record.headerFlags = 0;
      record.headerFormID = 0x00013634;
      record.headerRevision = 0x003C4C1C;
      record.headerVersion = 15;
      record.headerUnknown5 = 0x0001;

      // Writing should fail due to limited stream storage.
      MWTP::limited_streambuf<15> buffer;
      std::ostream stream(&buffer);
      REQUIRE( stream.good() );

      REQUIRE_FALSE( record.saveToStream(stream) );
    }
  }
}
