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
#include "../../../../../sr/base/records/KeywordRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("KeywordRecord")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    KeywordRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.unknownCNAM.has_value() );
    REQUIRE( record.unknownCNAM.value_or(42) == 42 );
  }

  SECTION("equals")
  {
    KeywordRecord a;
    KeywordRecord b;

    SECTION("equal with CNAM")
    {
      a.editorID = "foo";
      a.unknownCNAM = 0xDEADBEEF;

      b.editorID = "foo";
      b.unknownCNAM = 0xDEADBEEF;

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("equal without CNAM")
    {
      a.editorID = "foo";
      a.unknownCNAM = {};

      b.editorID = "foo";
      b.unknownCNAM = {};

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal with CNAM")
    {
      // editorID mismatch
      a.editorID = "foo";
      a.unknownCNAM = 0xDEADBEEF;

      b.editorID = "bar";
      b.unknownCNAM = 0xDEADBEEF;

      REQUIRE_FALSE( a.equals(b) );
      REQUIRE_FALSE( b.equals(a) );

      // CNAM mismatch
      a.editorID = "foo";
      a.unknownCNAM = 0xDEADBEEF;

      b.editorID = "foo";
      b.unknownCNAM = 0x0F00B412;

      REQUIRE_FALSE( a.equals(b) );
      REQUIRE_FALSE( b.equals(a) );
    }

    SECTION("unequal without CNAM")
    {
      // editorID mismatch
      a.editorID = "foo";
      a.unknownCNAM = {};

      b.editorID = "bar";
      b.unknownCNAM = {};

      REQUIRE_FALSE( a.equals(b) );
      REQUIRE_FALSE( b.equals(a) );
    }
  }

  SECTION("getRecordType")
  {
    KeywordRecord record;

    REQUIRE( record.getRecordType() == cKYWD );
  }

  SECTION("getWriteSize")
  {
    KeywordRecord record;

    SECTION("without CNAM")
    {
      record.editorID = "foobarfoobarbaz"; // 15 characters
      record.unknownCNAM = {};
      REQUIRE( record.getWriteSize() == 22 );

      record.editorID = "foo"; // three characters
      record.unknownCNAM = {};
      REQUIRE( record.getWriteSize() == 10 );
    }

    SECTION("with CNAM")
    {
      record.editorID = "foobarfoobarbaz"; // 15 characters
      record.unknownCNAM = 0xDEADBEEF;
      REQUIRE( record.getWriteSize() == 32 );

      record.editorID = "foobar"; // six characters
      record.unknownCNAM = 0xDEADBEEF;
      REQUIRE( record.getWriteSize() == 23 );
    }
  }

  SECTION("loadFromStream")
  {
    using namespace std::string_view_literals;

    uint32_t dummy = 0;
    StringTable dummy_table;

    SECTION("default: load record with CNAM")
    {
      const std::string_view data = "KYWD \0\0\0\0\0\0\0\x54\x0f\0\0\x08\x69\x2b\0\x28\0\x01\0EDID\x10\0PowerAttackBash\0CNAM\x04\0\x43\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KYWD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      KeywordRecord record;
      REQUIRE( record.loadFromStream(stream, false, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00000F54 );
      REQUIRE( record.headerRevision == 0x002B6908 );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID == "PowerAttackBash" );
      REQUIRE( record.unknownCNAM.has_value() );
      REQUIRE( record.unknownCNAM == 0x00000043 );
    }

    SECTION("default: load record without CNAM")
    {
      const std::string_view data = "KYWD\x10\0\0\0\0\0\0\0\xC3\xC6\x09\0\x06\x64\x61\0\x23\0\x01\0EDID\x0A\0isSmelter\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KYWD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      KeywordRecord record;
      REQUIRE( record.loadFromStream(stream, false, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0009C6C3 );
      REQUIRE( record.headerRevision == 0x00616406 );
      REQUIRE( record.headerVersion == 35 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID == "isSmelter" );
      REQUIRE_FALSE( record.unknownCNAM.has_value() );
      REQUIRE( record.unknownCNAM.value_or(0) == 0 );
    }

    SECTION("corrupt data: stream end before header can be read")
    {
      const std::string_view data = "KYWD\x10\0\0\0\0\0\0\0\xC3\xC6"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KYWD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeywordRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, false, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "KYWD\x10\0\0\0\0\0\0\0\xC3\xC6\x09\0\x06\x64\x61\0\x23\0\x01\0FAIL\x0A\0isSmelter\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KYWD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeywordRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, false, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "KYWD\x10\0\0\0\0\0\0\0\xC3\xC6\x09\0\x06\x64\x61\0\x23\0\x01\0EDID\x0A\x02isSmelter\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KYWD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeywordRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, false, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "KYWD\x10\0\0\0\0\0\0\0\xC3\xC6\x09\0\x06\x64\x61\0\x23\0\x01\0EDID\x0F\0isSmelter\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KYWD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeywordRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, false, dummy_table) );
    }

    SECTION("corrupt data: CNAM is FNAM")
    {
      const std::string_view data = "KYWD \0\0\0\0\0\0\0\x54\x0f\0\0\x08\x69\x2b\0\x28\0\x01\0EDID\x10\0PowerAttackBash\0FNAM\x04\0\x43\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KYWD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeywordRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, false, dummy_table) );
    }

    SECTION("corrupt data: CNAM length is not four bytes")
    {
      const std::string_view data = "KYWD \0\0\0\0\0\0\0\x54\x0f\0\0\x08\x69\x2b\0\x28\0\x01\0EDID\x10\0PowerAttackBash\0CNAM\x05\0\x43\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read KYWD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      KeywordRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, false, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    SECTION("save record with CNAM")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      KeywordRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0;
      record.headerFormID = 0x00000F54;
      record.headerRevision = 0x002B6908;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0001;
      // -- record data
      record.editorID = "PowerAttackBash";
      record.unknownCNAM = 0x00000043;

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "KYWD \0\0\0\0\0\0\0\x54\x0f\0\0\x08\x69\x2b\0\x28\0\x01\0EDID\x10\0PowerAttackBash\0CNAM\x04\0\x43\0\0\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save record without CNAM")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      KeywordRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0;
      record.headerFormID = 0x0009C6C3;
      record.headerRevision = 0x00616406;
      record.headerVersion = 35;
      record.headerUnknown5 = 0x0001;
      // -- record data
      record.editorID = "isSmelter";
      record.unknownCNAM = {};

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "KYWD\x10\0\0\0\0\0\0\0\xC3\xC6\x09\0\x06\x64\x61\0\x23\0\x01\0EDID\x0A\0isSmelter\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
