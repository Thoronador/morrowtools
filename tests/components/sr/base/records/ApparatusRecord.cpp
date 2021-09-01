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
#include "../../../../../sr/base/records/ApparatusRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("ApparatusRecord")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    ApparatusRecord record;

    REQUIRE( record.editorID.empty() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.quality == 0 );
    REQUIRE_FALSE( record.description.isPresent() );
    REQUIRE( record.value == 0 );
    REQUIRE( record.weight == 0 );
  }

  SECTION("equals")
  {
    ApparatusRecord a;
    ApparatusRecord b;

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
        a.unknownOBND[11] = 0xFF;
        b.unknownOBND[11] = 0x00;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 123, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.name = LocalizedString(LocalizedString::Type::Index, 456, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.name = LocalizedString(LocalizedString::Type::None, 0, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("quality mismatch")
      {
        a.quality = 1;
        b.quality = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("description mismatch")
      {
        a.description = LocalizedString(LocalizedString::Type::Index, 123, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.description = LocalizedString(LocalizedString::Type::Index, 456, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.description = LocalizedString(LocalizedString::Type::None, 0, "");

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
        b.weight = 1.2f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    ApparatusRecord record;

    REQUIRE( record.getRecordType() == cAPPA );
  }

  SECTION("getWriteSize")
  {
    ApparatusRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 52 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 64 );
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
    dummy_table.addString(0x0000BE31, "foo");

    SECTION("default: load record")
    {
      const std::string_view data = "APPA\x54\0\0\0\0\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0EDID\x10\0Alembic01Novice\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x31\xBE\0\0QUAL\x04\0\0\0\0\0DESC\x04\0\0\0\0\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ApparatusRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x000332A7 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0003 );
      // -- record data
      REQUIRE( record.editorID == "Alembic01Novice" );
      for (unsigned int i = 0; i < 12; ++i)
      {
        REQUIRE( record.unknownOBND[i] == 0 );
      }
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000BE31 );
      REQUIRE( record.quality == 0 );
      REQUIRE( record.description.isPresent() );
      REQUIRE( record.description.getIndex() == 0 );
      REQUIRE( record.description.getString().empty() );
      REQUIRE( record.value == 0 );
      REQUIRE( record.weight == 0 );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "APPA\x54\0\0\0\0\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0FAIL\x10\0Alembic01Novice\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x31\xBE\0\0QUAL\x04\0\0\0\0\0DESC\x04\0\0\0\0\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "APPA\x54\0\0\0\0\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0EDID\x10\x02Alembic01Novice\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x31\xBE\0\0QUAL\x04\0\0\0\0\0DESC\x04\0\0\0\0\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "APPA\x54\0\0\0\0\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0EDID\x4F\0Alembic01Novice\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x31\xBE\0\0QUAL\x04\0\0\0\0\0DESC\x04\0\0\0\0\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "APPA\x54\0\0\0\0\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0EDID\x10\0Alembic01Novice\0FAIL\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x31\xBE\0\0QUAL\x04\0\0\0\0\0DESC\x04\0\0\0\0\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "APPA\x54\0\0\0\0\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0EDID\x10\0Alembic01Novice\0OBND\x0B\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x31\xBE\0\0QUAL\x04\0\0\0\0\0DESC\x04\0\0\0\0\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read APPA, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ApparatusRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "APPA\x54\0\0\0\0\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0EDID\x10\0Alembic01Novice\0OBND\x0D\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x31\xBE\0\0QUAL\x04\0\0\0\0\0DESC\x04\0\0\0\0\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read APPA, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ApparatusRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: no QUAL")
    {
      const std::string_view data = "APPA\x54\0\0\0\0\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0EDID\x10\0Alembic01Novice\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x31\xBE\0\0FAIL\x04\0\0\0\0\0DESC\x04\0\0\0\0\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of QUAL is not four")
    {
      {
        const std::string_view data = "APPA\x54\0\0\0\0\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0EDID\x10\0Alembic01Novice\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x31\xBE\0\0QUAL\x03\0\0\0\0\0DESC\x04\0\0\0\0\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read APPA, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ApparatusRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "APPA\x54\0\0\0\0\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0EDID\x10\0Alembic01Novice\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x31\xBE\0\0QUAL\x05\0\0\0\0\0DESC\x04\0\0\0\0\0DATA\x08\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read APPA, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ApparatusRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: no DATA")
    {
      const std::string_view data = "APPA\x54\0\0\0\0\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0EDID\x10\0Alembic01Novice\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x31\xBE\0\0QUAL\x04\0\0\0\0\0DESC\x04\0\0\0\0\0FAIL\x08\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not eight")
    {
      {
        const std::string_view data = "APPA\x54\0\0\0\0\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0EDID\x10\0Alembic01Novice\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x31\xBE\0\0QUAL\x04\0\0\0\0\0DESC\x04\0\0\0\0\0DATA\x07\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read APPA, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ApparatusRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "APPA\x54\0\0\0\0\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0EDID\x10\0Alembic01Novice\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x31\xBE\0\0QUAL\x04\0\0\0\0\0DESC\x04\0\0\0\0\0DATA\x09\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read APPA, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ApparatusRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    SECTION("save record")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      ApparatusRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0;
      record.headerFormID = 0x000332A7;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0003;
      // -- record data
      record.editorID = "Alembic01Novice";
      for (unsigned int i = 0; i < 12; ++i)
      {
        record.unknownOBND[i] = i + 1;
      }
      record.name = LocalizedString(LocalizedString::Type::Index, 0x0000BE31, "" );
      record.quality = 5;
      record.description = LocalizedString(LocalizedString::Type::Index, 0, "" );
      record.value = 42;
      record.weight = 1.0f;

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "APPA\x54\0\0\0\0\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0EDID\x10\0Alembic01Novice\0OBND\x0C\0\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x46ULL\x04\0\x31\xBE\0\0QUAL\x04\0\x05\0\0\0DESC\x04\0\0\0\0\0DATA\x08\0\x2A\0\0\0\0\0\x80\x3F"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save deleted record")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      ApparatusRecord record;
      // Fill data.
      // -- header
      record.headerFlags = BasicRecord::cDeletedFlag;
      record.headerFormID = 0x000332A7;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0003;
      // -- record data
      record.editorID = "Alembic01Novice";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "APPA\0\0\0\0\x20\0\0\0\xA7\x32\x03\0\x1B\x69\x55\0\x28\0\x03\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
