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
#include "../../../../../sr/base/records/AcousticSpaceRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("AcousticSpaceRecord")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    AcousticSpaceRecord record;

    REQUIRE( record.editorID.empty() );
    std::array<uint8_t, 12> obnd = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    REQUIRE( record.unknownOBND == obnd );
    REQUIRE( record.loopingSoundFormID == 0 );
    REQUIRE( record.regionFormID == 0 );
    REQUIRE( record.environmentTypeFormID == 0 );
  }

  SECTION("equals")
  {
    AcousticSpaceRecord a;
    AcousticSpaceRecord b;

    SECTION("equal")
    {
      a.editorID = "foo";
      a.unknownOBND = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C };
      a.loopingSoundFormID = 0xDEADBEEF;
      a.regionFormID = 0x0F00BA12;
      a.environmentTypeFormID = 0xB412BA77;

      b.editorID = "foo";
      b.unknownOBND = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C };
      b.loopingSoundFormID = 0xDEADBEEF;
      b.regionFormID = 0x0F00BA12;
      b.environmentTypeFormID = 0xB412BA77;

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("editor ID mismatch")
      {
        a.editorID = "foo";
        a.unknownOBND = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C };
        a.loopingSoundFormID = 0xDEADBEEF;
        a.regionFormID = 0x0F00BA12;
        a.environmentTypeFormID = 0xB412BA77;

        b.editorID = "bar";
        b.unknownOBND = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C };
        b.loopingSoundFormID = 0xDEADBEEF;
        b.regionFormID = 0x0F00BA12;
        b.environmentTypeFormID = 0xB412BA77;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("object bounds mismatch")
      {
        a.editorID = "foo";
        a.unknownOBND = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C };
        a.loopingSoundFormID = 0xDEADBEEF;
        a.regionFormID = 0x0F00BA12;
        a.environmentTypeFormID = 0xB412BA77;

        b.editorID = "foo";
        b.unknownOBND = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0xFF };
        b.loopingSoundFormID = 0xDEADBEEF;
        b.regionFormID = 0x0F00BA12;
        b.environmentTypeFormID = 0xB412BA77;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("loopingSoundFormID mismatch")
      {
        a.editorID = "foo";
        a.unknownOBND = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C };
        a.loopingSoundFormID = 0xDEADBEEF;
        a.regionFormID = 0x0F00BA12;
        a.environmentTypeFormID = 0xB412BA77;

        b.editorID = "foo";
        b.unknownOBND = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C };
        b.loopingSoundFormID = 0xAFFEAFFE;
        b.regionFormID = 0x0F00BA12;
        b.environmentTypeFormID = 0xB412BA77;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("region form ID mismatch")
      {
        a.editorID = "foo";
        a.unknownOBND = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C };
        a.loopingSoundFormID = 0xDEADBEEF;
        a.regionFormID = 0x0F00BA12;
        a.environmentTypeFormID = 0xB412BA77;

        b.editorID = "foo";
        b.unknownOBND = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C };
        b.loopingSoundFormID = 0xDEADBEEF;
        b.regionFormID = 0x0AFFEAFFE;
        b.environmentTypeFormID = 0xB412BA77;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("environmentTypeFormID mismatch")
      {
        a.editorID = "foo";
        a.unknownOBND = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C };
        a.loopingSoundFormID = 0xDEADBEEF;
        a.regionFormID = 0x0F00BA12;
        a.environmentTypeFormID = 0xB412BA77;

        b.editorID = "foo";
        b.unknownOBND = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C };
        b.loopingSoundFormID = 0xDEADBEEF;
        b.regionFormID = 0x0F00BA12;
        b.environmentTypeFormID = 0xAFFEAFFE;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    AcousticSpaceRecord record;

    REQUIRE( record.getRecordType() == cASPC );
  }

  SECTION("getWriteSize")
  {
    AcousticSpaceRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foobarfoobarbaz"; // 15 characters
      record.unknownOBND = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C };
      record.loopingSoundFormID = 0xDEADBEEF;
      record.regionFormID = 0x0F00BA12;
      record.environmentTypeFormID = 0xB412BA77;
      REQUIRE( record.getWriteSize() == 70 );

      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 58 );
    }

    SECTION("deleted record has size zero")
    {
      record.headerFlags = BasicRecord::cDeletedFlag;
      REQUIRE( record.getWriteSize() == 0 );
    }

    SECTION("write size declines with every unset form ID")
    {
      record.editorID = "foobarfoobarbaz"; // 15 characters
      record.unknownOBND = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C };
      record.loopingSoundFormID = 0xDEADBEEF;
      record.regionFormID = 0x0F00BA12;
      record.environmentTypeFormID = 0xB412BA77;
      REQUIRE( record.getWriteSize() == 70 );

      record.loopingSoundFormID = 0;
      REQUIRE( record.getWriteSize() == 60 );

      record.regionFormID = 0;
      REQUIRE( record.getWriteSize() == 50 );

      record.environmentTypeFormID = 0;
      REQUIRE( record.getWriteSize() == 40 );
    }
  }

  SECTION("loadFromStream")
  {
    using namespace std::string_view_literals;

    uint32_t dummy = 0;
    StringTable dummy_table;

    SECTION("default: load record with SNAM, RDAT and BNAM")
    {
      const std::string_view data = "ASPC\x64\0\0\0\0\0\0\0\x0D\xF9\x10\0\x05\x68\x27\x00\x27\x00\x02\0EDID\x2E\0IntRoomWoodLargeTempleOfKynarethAcousticSpace\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0SNAM\x04\0\x08\x5D\x0C\0RDAT\x04\0\x0E\xF9\x10\0BNAM\x04\0\x46\x32\x0E\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      AcousticSpaceRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0010F90D );
      REQUIRE( record.headerRevision == 0x00276805 );
      REQUIRE( record.headerVersion == 39 );
      REQUIRE( record.headerUnknown5 == 0x0002 );
      // -- record data
      REQUIRE( record.editorID == "IntRoomWoodLargeTempleOfKynarethAcousticSpace" );
      const std::array<uint8_t, 12> expected_obnd = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      REQUIRE( record.unknownOBND == expected_obnd );
      REQUIRE( record.loopingSoundFormID == 0x000C5D08 );
      REQUIRE( record.regionFormID == 0x0010F90E );
      REQUIRE( record.environmentTypeFormID == 0x000E3246 );
    }

    SECTION("default: load record with RDAT")
    {
      const std::string_view data = "ASPC\x34\0\0\0\0\0\0\0\xAA\xD8\x10\0\x16\x67\x27\0\x27\0\x02\0EDID\x12\0ExtHelgenAttackAS\0OBND\x0C\0\xBE\xF1\xD0\xF0\x26\xFA\x42\x0E\x30\x0F\xDA\x05RDAT\x04\0\xA9\xD8\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      AcousticSpaceRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0010D8AA );
      REQUIRE( record.headerRevision == 0x00276716 );
      REQUIRE( record.headerVersion == 39 );
      REQUIRE( record.headerUnknown5 == 0x0002 );
      // -- record data
      REQUIRE( record.editorID == "ExtHelgenAttackAS" );
      const std::array<uint8_t, 12> expected_obnd = { 0xBE, 0xF1, 0xD0, 0xF0, 0x26, 0xFA, 0x42, 0x0E, 0x30, 0x0F, 0xDA, 0x05 };
      REQUIRE( record.unknownOBND == expected_obnd );
      REQUIRE( record.loopingSoundFormID == 0 );
      REQUIRE( record.regionFormID == 0x0010D8A9 );
      REQUIRE( record.environmentTypeFormID == 0 );
    }

    SECTION("default: load record with BNAM")
    {
      const std::string_view data = "ASPC\x46\0\0\0\0\0\0\0\x9E\xFE\x10\0\x12\x68\x38\0\x28\0\x01\0EDID\x24\0ExtMQFlashbackInteriorAcousticSpace\0OBND\x0C\0\x24\xFA\x24\xFA\x59\xFE\xDC\x05\xDC\x05\xA7\x01\x42NAM\x04\0\x2A\xD9\x0F\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      AcousticSpaceRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0010FE9E );
      REQUIRE( record.headerRevision == 0x00386812 );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID == "ExtMQFlashbackInteriorAcousticSpace" );
      const std::array<uint8_t, 12> expected_obnd = { 0x24, 0xFA, 0x24, 0xFA, 0x59, 0xFE, 0xDC, 0x05, 0xDC, 0x05, 0xA7, 0x01 };
      REQUIRE( record.unknownOBND == expected_obnd );
      REQUIRE( record.loopingSoundFormID == 0 );
      REQUIRE( record.regionFormID == 0 );
      REQUIRE( record.environmentTypeFormID == 0x000FD92A );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "ASPC\x64\0\0\0\0\0\0\0\x0D\xF9\x10\0\x05"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "ASPC\x64\0\0\0\0\0\0\0\x0D\xF9\x10\0\x05\x68\x27\x00\x27\x00\x02\0FAIL\x2E\0IntRoomWoodLargeTempleOfKynarethAcousticSpace\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0SNAM\x04\0\x08\x5D\x0C\0RDAT\x04\0\x0E\xF9\x10\0BNAM\x04\0\x46\x32\x0E\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "ASPC\x64\0\0\0\0\0\0\0\x0D\xF9\x10\0\x05\x68\x27\x00\x27\x00\x02\0EDID\x2E\x02IntRoomWoodLargeTempleOfKynarethAcousticSpace\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0SNAM\x04\0\x08\x5D\x0C\0RDAT\x04\0\x0E\xF9\x10\0BNAM\x04\0\x46\x32\x0E\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "ASPC\x64\0\0\0\0\0\0\0\x0D\xF9\x10\0\x05\x68\x27\x00\x27\x00\x02\0EDID\x66\0IntRoomWoodLargeTempleOfKynarethAcousticSpace\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0SNAM\x04\0\x08\x5D\x0C\0RDAT\x04\0\x0E\xF9\x10\0BNAM\x04\0\x46\x32\x0E\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "ASPC\x46\0\0\0\0\0\0\0\x9E\xFE\x10\0\x12\x68\x38\0\x28\0\x01\0EDID\x24\0ExtMQFlashbackInteriorAcousticSpace\0FAIL\x0C\0\x24\xFA\x24\xFA\x59\xFE\xDC\x05\xDC\x05\xA7\x01\x42NAM\x04\0\x2A\xD9\x0F\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream end in the middle of OBND")
    {
      const std::string_view data = "ASPC\x46\0\0\0\0\0\0\0\x9E\xFE\x10\0\x12\x68\x38\0\x28\0\x01\0EDID\x24\0ExtMQFlashbackInteriorAcousticSpace\0OBND\x0C\0\x24\xFA\x24\xFA\x59\xFE"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: OBND length is not 12")
    {
      const std::string_view data = "ASPC\x46\0\0\0\0\0\0\0\x9E\xFE\x10\0\x12\x68\x38\0\x28\0\x01\0EDID\x24\0ExtMQFlashbackInteriorAcousticSpace\0OBND\x0D\0\x24\xFA\x24\xFA\x59\xFE\xDC\x05\xDC\x05\xA7\x01\x42NAM\x04\0\x2A\xD9\x0F\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: unknown record UNAM")
    {
      const std::string_view data = "ASPC\x46\0\0\0\0\0\0\0\x9E\xFE\x10\0\x12\x68\x38\0\x28\0\x01\0EDID\x24\0ExtMQFlashbackInteriorAcousticSpace\0OBND\x0C\0\x24\xFA\x24\xFA\x59\xFE\xDC\x05\xDC\x05\xA7\x01UNAM\x04\0\x2A\xD9\x0F\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SNAM is not four")
    {
      const std::string_view data = "ASPC\x34\0\0\0\0\0\0\0\xAA\xD8\x10\0\x16\x67\x27\0\x27\0\x02\0EDID\x12\0ExtHelgenAttackAS\0OBND\x0C\0\xBE\xF1\xD0\xF0\x26\xFA\x42\x0E\x30\x0F\xDA\x05SNAM\x02\0\xA9\xD8\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: SNAM is zero")
    {
      const std::string_view data = "ASPC\x64\0\0\0\0\0\0\0\x0D\xF9\x10\0\x05\x68\x27\x00\x27\x00\x02\0EDID\x2E\0IntRoomWoodLargeTempleOfKynarethAcousticSpace\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0SNAM\x04\0\0\0\0\0RDAT\x04\0\x0E\xF9\x10\0BNAM\x04\0\x46\x32\x0E\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: SNAM exists more than once")
    {
      const std::string_view data = "ASPC\x64\0\0\0\0\0\0\0\x0D\xF9\x10\0\x05\x68\x27\x00\x27\x00\x02\0EDID\x2E\0IntRoomWoodLargeTempleOfKynarethAcousticSpace\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0SNAM\x04\0\x08\x5D\x0C\0RDAT\x04\0\x0E\xF9\x10\0SNAM\x04\0\x46\x32\x0E\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of RDAT is not four")
    {
      const std::string_view data = "ASPC\x34\0\0\0\0\0\0\0\xAA\xD8\x10\0\x16\x67\x27\0\x27\0\x02\0EDID\x12\0ExtHelgenAttackAS\0OBND\x0C\0\xBE\xF1\xD0\xF0\x26\xFA\x42\x0E\x30\x0F\xDA\x05RDAT\x05\0\xA9\xD8\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: RDAT is zero")
    {
      const std::string_view data = "ASPC\x46\0\0\0\0\0\0\0\x9E\xFE\x10\0\x12\x68\x38\0\x28\0\x01\0EDID\x24\0ExtMQFlashbackInteriorAcousticSpace\0OBND\x0C\0\x24\xFA\x24\xFA\x59\xFE\xDC\x05\xDC\x05\xA7\x01RDAT\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: RDAT exists more than once")
    {
      const std::string_view data = "ASPC\x64\0\0\0\0\0\0\0\x0D\xF9\x10\0\x05\x68\x27\x00\x27\x00\x02\0EDID\x2E\0IntRoomWoodLargeTempleOfKynarethAcousticSpace\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0SNAM\x04\0\x08\x5D\x0C\0RDAT\x04\0\x0E\xF9\x10\0RDAT\x04\0\x46\x32\x0E\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of BNAM is not four")
    {
      const std::string_view data = "ASPC\x46\0\0\0\0\0\0\0\x9E\xFE\x10\0\x12\x68\x38\0\x28\0\x01\0EDID\x24\0ExtMQFlashbackInteriorAcousticSpace\0OBND\x0C\0\x24\xFA\x24\xFA\x59\xFE\xDC\x05\xDC\x05\xA7\x01\x42NAM\x03\0\x2A\xD9\x0F\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: BNAM is zero")
    {
      const std::string_view data = "ASPC\x46\0\0\0\0\0\0\0\x9E\xFE\x10\0\x12\x68\x38\0\x28\0\x01\0EDID\x24\0ExtMQFlashbackInteriorAcousticSpace\0OBND\x0C\0\x24\xFA\x24\xFA\x59\xFE\xDC\x05\xDC\x05\xA7\x01\x42NAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: BNAM exists more than once")
    {
      const std::string_view data = "ASPC\x64\0\0\0\0\0\0\0\x0D\xF9\x10\0\x05\x68\x27\x00\x27\x00\x02\0EDID\x2E\0IntRoomWoodLargeTempleOfKynarethAcousticSpace\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0BNAM\x04\0\x08\x5D\x0C\0RDAT\x04\0\x0E\xF9\x10\0BNAM\x04\0\x46\x32\x0E\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ASPC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AcousticSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    SECTION("save record with SNAM, RDAT and BNAM")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      AcousticSpaceRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0;
      record.headerFormID = 0x0010F90D;
      record.headerRevision = 0x00276805;
      record.headerVersion = 39;
      record.headerUnknown5 = 0x0002;
      // -- record data
      record.editorID = "IntRoomWoodLargeTempleOfKynarethAcousticSpace";
      record.unknownOBND = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      record.loopingSoundFormID = 0x000C5D08;
      record.regionFormID = 0x0010F90E;
      record.environmentTypeFormID = 0x000E3246;

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "ASPC\x64\0\0\0\0\0\0\0\x0D\xF9\x10\0\x05\x68\x27\x00\x27\x00\x02\0EDID\x2E\0IntRoomWoodLargeTempleOfKynarethAcousticSpace\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0SNAM\x04\0\x08\x5D\x0C\0RDAT\x04\0\x0E\xF9\x10\0BNAM\x04\0\x46\x32\x0E\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save record with RDAT")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      AcousticSpaceRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0;
      record.headerFormID = 0x0010D8AA;
      record.headerRevision = 0x00276716;
      record.headerVersion = 39;
      record.headerUnknown5 = 0x0002;
      // -- record data
      record.editorID = "ExtHelgenAttackAS";
      record.unknownOBND = { 0xBE, 0xF1, 0xD0, 0xF0, 0x26, 0xFA, 0x42, 0x0E, 0x30, 0x0F, 0xDA, 0x05 };
      record.loopingSoundFormID = 0;
      record.regionFormID = 0x0010D8A9;
      record.environmentTypeFormID = 0;

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "ASPC\x34\0\0\0\0\0\0\0\xAA\xD8\x10\0\x16\x67\x27\0\x27\0\x02\0EDID\x12\0ExtHelgenAttackAS\0OBND\x0C\0\xBE\xF1\xD0\xF0\x26\xFA\x42\x0E\x30\x0F\xDA\x05RDAT\x04\0\xA9\xD8\x10\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save record with BNAM")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      AcousticSpaceRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0;
      record.headerFormID = 0x0010FE9E;
      record.headerRevision = 0x00386812;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0001;
      // -- record data
      record.editorID = "ExtMQFlashbackInteriorAcousticSpace";
      record.unknownOBND = { 0x24, 0xFA, 0x24, 0xFA, 0x59, 0xFE, 0xDC, 0x05, 0xDC, 0x05, 0xA7, 0x01 };
      record.loopingSoundFormID = 0;
      record.regionFormID = 0;
      record.environmentTypeFormID = 0x000FD92A;

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "ASPC\x46\0\0\0\0\0\0\0\x9E\xFE\x10\0\x12\x68\x38\0\x28\0\x01\0EDID\x24\0ExtMQFlashbackInteriorAcousticSpace\0OBND\x0C\0\x24\xFA\x24\xFA\x59\xFE\xDC\x05\xDC\x05\xA7\x01\x42NAM\x04\0\x2A\xD9\x0F\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save deleted record")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      AcousticSpaceRecord record;
      // Fill data.
      // -- header
      record.headerFlags = BasicRecord::cDeletedFlag;
      record.headerFormID = 0x0010F90D;
      record.headerRevision = 0x00276805;
      record.headerVersion = 39;
      record.headerUnknown5 = 0x0002;
      // -- record data
      record.editorID = "IntRoomWoodLargeTempleOfKynarethAcousticSpace";
      record.unknownOBND = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      record.loopingSoundFormID = 0x000C5D08;
      record.regionFormID = 0x0010F90E;
      record.environmentTypeFormID = 0x000E3246;

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "ASPC\0\0\0\0\x20\0\0\0\x0D\xF9\x10\0\x05\x68\x27\x00\x27\x00\x02\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
