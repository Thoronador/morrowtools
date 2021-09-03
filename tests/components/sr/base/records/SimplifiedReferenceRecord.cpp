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
#include "../../../../../sr/base/records/SimplifiedReferenceRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("SimplifiedReferenceRecord")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    SimplifiedReferenceRecord record;

    REQUIRE( record.baseObjectFormID == 0 );
  }

  SECTION("equals")
  {
    SimplifiedReferenceRecord a;
    SimplifiedReferenceRecord b;

    SECTION("equal")
    {
      a.baseObjectFormID = 0xAFFEAFFE;
      b.baseObjectFormID = 0xAFFEAFFE;

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      // baseObjectFormID mismatch
      a.baseObjectFormID = 0xAFFEAFFE;
      b.baseObjectFormID = 0xDEADBEEF;

      REQUIRE_FALSE( a.equals(b) );
      REQUIRE_FALSE( b.equals(a) );
    }
  }

  SECTION("getRecordType")
  {
    SimplifiedReferenceRecord record;

    REQUIRE( record.getRecordType() == cREFR );
  }

  SECTION("getWriteSize")
  {
    SimplifiedReferenceRecord record;

    SECTION("size is irrelevant for non-deleted records, assume zero")
    {
      REQUIRE_FALSE( record.isDeleted() );
      REQUIRE( record.getWriteSize() == 0 );
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

    SECTION("default: load record with NAME")
    {
      const std::string_view data = "REFR\x28\0\0\0\0\x80\0\0\x0C\x44\x10\0\x18\x66\x47\0\x25\0\0\0NAME\x04\0\xF6\x01\x02\0DATA\x18\0\xEE\x25\xE1\xC4\x24\xAE\xC4\xC5\x9E\x7C\x0C\x45\x1A\x63\x4A\xBE\xD6\x2D\x80\xBE\x98\x56\xE8\x3E"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read REFR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      SimplifiedReferenceRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00008000 );
      REQUIRE( record.headerFormID == 0x0010440C );
      REQUIRE( record.headerRevision == 0x00476618 );
      REQUIRE( record.headerVersion == 37 );
      REQUIRE( record.headerUnknown5 == 0x0000 );
      // -- record data
      REQUIRE( record.baseObjectFormID == 0x000201F6 );
    }

    SECTION("default: load record with NAME not as first record")
    {
      const std::string_view data = "REFR\x28\0\0\0\0\x80\0\0\x0C\x44\x10\0\x18\x66\x47\0\x25\0\0\0DATA\x18\0\xEE\x25\xE1\xC4\x24\xAE\xC4\xC5\x9E\x7C\x0C\x45\x1A\x63\x4A\xBE\xD6\x2D\x80\xBE\x98\x56\xE8\x3ENAME\x04\0\xF6\x01\x02\x03"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read REFR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      SimplifiedReferenceRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00008000 );
      REQUIRE( record.headerFormID == 0x0010440C );
      REQUIRE( record.headerRevision == 0x00476618 );
      REQUIRE( record.headerVersion == 37 );
      REQUIRE( record.headerUnknown5 == 0x0000 );
      // -- record data
      REQUIRE( record.baseObjectFormID == 0x030201F6 );
    }

    SECTION("default: load record with NAME not as first record and entry after name")
    {
      const std::string_view data = "REFR\x30\0\0\0\0\x80\0\0\x0C\x44\x10\0\x18\x66\x47\0\x25\0\0\0DATA\x18\0\xEE\x25\xE1\xC4\x24\xAE\xC4\xC5\x9E\x7C\x0C\x45\x1A\x63\x4A\xBE\xD6\x2D\x80\xBE\x98\x56\xE8\x3ENAME\x04\0\xAA\x01\x02\x03NOOB\x02\0AR"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read REFR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      SimplifiedReferenceRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00008000 );
      REQUIRE( record.headerFormID == 0x0010440C );
      REQUIRE( record.headerRevision == 0x00476618 );
      REQUIRE( record.headerVersion == 37 );
      REQUIRE( record.headerUnknown5 == 0x0000 );
      // -- record data
      REQUIRE( record.baseObjectFormID == 0x030201AA );
    }

    SECTION("constructed example: load record with NAME only")
    {
      const std::string_view data = "REFR\x0A\0\0\0\0\x80\0\0\x0C\x44\x10\0\x18\x66\x47\0\x25\0\0\0NAME\x04\0\xF6\x01\x02\xAF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read REFR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      SimplifiedReferenceRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00008000 );
      REQUIRE( record.headerFormID == 0x0010440C );
      REQUIRE( record.headerRevision == 0x00476618 );
      REQUIRE( record.headerVersion == 37 );
      REQUIRE( record.headerUnknown5 == 0x0000 );
      // -- record data
      REQUIRE( record.baseObjectFormID == 0xAF0201F6 );
    }

    SECTION("corrupt data: no NAME")
    {
      const std::string_view data = "REFR\x28\0\0\0\0\x80\0\0\x0C\x44\x10\0\x18\x66\x47\0\x25\0\0\0FAIL\x04\0\xF6\x01\x02\0DATA\x18\0\xEE\x25\xE1\xC4\x24\xAE\xC4\xC5\x9E\x7C\x0C\x45\x1A\x63\x4A\xBE\xD6\x2D\x80\xBE\x98\x56\xE8\x3E"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read REFR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SimplifiedReferenceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of NAME is not four")
    {
      {
        const std::string_view data = "REFR\x28\0\0\0\0\x80\0\0\x0C\x44\x10\0\x18\x66\x47\0\x25\0\0\0NAME\x03\0\xF6\x01\x02\0DATA\x18\0\xEE\x25\xE1\xC4\x24\xAE\xC4\xC5\x9E\x7C\x0C\x45\x1A\x63\x4A\xBE\xD6\x2D\x80\xBE\x98\x56\xE8\x3E"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read REFR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SimplifiedReferenceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "REFR\x29\0\0\0\0\x80\0\0\x0C\x44\x10\0\x18\x66\x47\0\x25\0\0\0NAME\x05\0\xF6\x01\x02\0\0DATA\x18\0\xEE\x25\xE1\xC4\x24\xAE\xC4\xC5\x9E\x7C\x0C\x45\x1A\x63\x4A\xBE\xD6\x2D\x80\xBE\x98\x56\xE8\x3E"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read REFR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SimplifiedReferenceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    SECTION("save record with NAME")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      SimplifiedReferenceRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0x00008000;
      record.headerFormID = 0x0010440C;
      record.headerRevision = 0x00476618;
      record.headerVersion = 37;
      record.headerUnknown5 = 0x0000;
      // -- record data
      record.baseObjectFormID = 0x000201F6;

      // Writing should fail, because record is not saveable.
      REQUIRE_FALSE( record.saveToStream(stream) );
      // Check written data.
      REQUIRE( stream.str() == "" );
    }

    SECTION("save deleted record")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      SimplifiedReferenceRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0x00008000 | BasicRecord::cDeletedFlag;
      record.headerFormID = 0x0010440C;
      record.headerRevision = 0x00476618;
      record.headerVersion = 37;
      record.headerUnknown5 = 0x0000;
      // -- record data
      record.baseObjectFormID = 0x000201F6;

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "REFR\0\0\0\0\x20\x80\0\0\x0C\x44\x10\0\x18\x66\x47\0\x25\0\0\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
