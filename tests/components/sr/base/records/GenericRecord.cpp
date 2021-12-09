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
#include "../../../../../sr/base/records/GenericRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("GenericRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  uint32_t dummy = 0;
  StringTable dummy_table;

  SECTION("constructor")
  {
    GenericRecord record;

    REQUIRE( record.Header == 0 );
    REQUIRE( record.size() == 0 );
    REQUIRE( record.data() == nullptr );
  }

  SECTION("getRecordType")
  {
    GenericRecord record;

    REQUIRE( record.getRecordType() == 0 );
  }

  SECTION("getWriteSize")
  {
    GenericRecord record;

    SECTION("new record has size zero")
    {
      REQUIRE( record.getWriteSize() == 0 );
    }

    SECTION("size equals data size")
    {
      {
        const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0"sv;
        std::istringstream streamIn;
        streamIn.str(std::string(data));

        // read AACT, because header is handled before loadFromStream.
        streamIn.read(reinterpret_cast<char*>(&dummy), 4);
        // Reading should succeed.
        REQUIRE( record.loadFromStream(streamIn, true, dummy_table) );
        REQUIRE( record.getWriteSize() == record.size() );
        REQUIRE( record.getWriteSize() == 25 );
      }

      {
        const std::string_view data = "AACT\x1B\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x15\0ActionShieldChange12\0"sv;
        std::istringstream streamIn;
        streamIn.str(std::string(data));

        // read AACT, because header is handled before loadFromStream.
        streamIn.read(reinterpret_cast<char*>(&dummy), 4);
        // Reading should succeed.
        REQUIRE( record.loadFromStream(streamIn, true, dummy_table) );
        REQUIRE( record.getWriteSize() == record.size() );
        REQUIRE( record.getWriteSize() == 27 );
      }
    }

    SECTION("deleted record has size zero")
    {
      record.headerFlags = BasicRecord::cDeletedFlag;
      REQUIRE( record.getWriteSize() == 0 );
    }
  }

  SECTION("loadFromStream and saveToStream")
  {
    SECTION("default: load record")
    {
      const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0"sv;
      std::istringstream streamIn;
      streamIn.str(std::string(data));

      // read AACT, because header is handled before loadFromStream.
      streamIn.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( streamIn.good() );

      // Reading should succeed.
      GenericRecord record;
      REQUIRE( record.loadFromStream(streamIn, true, dummy_table) );
      record.Header = cAACT;
      // Check data.
      REQUIRE( record.size() == 25 );
      REQUIRE( record.data() != nullptr );
      const auto view = std::string_view(reinterpret_cast<const char*>(record.data()), record.size());
      REQUIRE( view == data.substr(24) );

      // Saving should succeed.
      std::ostringstream streamOut;
      // Writing should succeed.
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );

      // Saving with different header should succeed, too.
      std::ostringstream streamOutHeader;
      record.Header = cBOOK;
      // Writing should succeed.
      REQUIRE( record.saveToStream(streamOutHeader) );
      // Check written data.
      REQUIRE( streamOutHeader.str() == "BOOK\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0"sv );
    }

    SECTION("corrupt data: stream too short to even read the header")
    {
      const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read AACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GenericRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: record size is above 256 KB")
    {
      const std::string_view data = "AACT\x01\0\0x04\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read AACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GenericRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: record size is beyond stream end")
    {
      const std::string_view data = "AACT\x1A\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read AACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GenericRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("equality, copy constructor, etc.")
  {
    using namespace std::string_view_literals;

    SECTION("empty records")
    {
      GenericRecord a;
      GenericRecord b;

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );

      GenericRecord c(a);
      REQUIRE( a.equals(c) );
      REQUIRE( c.equals(a) );

      GenericRecord d;
      d = a;
      REQUIRE( a.equals(d) );
      REQUIRE( d.equals(a) );
    }

    SECTION("non-empty records with same content")
    {
      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      GenericRecord a;
      // read AACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );
      REQUIRE( a.loadFromStream(stream, true, dummy_table) );

      // Copy-construct b from a.
      GenericRecord b = a;
      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );

      // Size should be equal, but pointers should not be equal.
      REQUIRE( a.size() == b.size() );
      REQUIRE( a.data() != nullptr );
      REQUIRE( b.data() != nullptr );
      REQUIRE( a.data() != b.data() );
      // However, the pointed to content should be equal.
      for (unsigned int i = 0; i < a.size(); ++i)
      {
        REQUIRE( a.data()[i] == b.data()[i] );
      }
    }

    SECTION("non-empty records with different content")
    {
      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0SixSevenEightNineO\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      // read AACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      GenericRecord a;
      REQUIRE( a.loadFromStream(stream, true, dummy_table) );

      GenericRecord b;
      // read AACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      REQUIRE( b.loadFromStream(stream, true, dummy_table) );
      REQUIRE_FALSE( a.equals(b) );
      REQUIRE_FALSE( b.equals(a) );

      // Size should be equal, but pointers should not be equal.
      REQUIRE( a.size() == b.size() );
      REQUIRE( a.data() != nullptr );
      REQUIRE( b.data() != nullptr );
      REQUIRE( a.data() != b.data() );
      // However, the pointed to content should partially be equal.
      REQUIRE( a.data()[0] == b.data()[0] );
      REQUIRE( a.data()[1] == b.data()[1] );
      REQUIRE( a.data()[2] == b.data()[2] );
      REQUIRE( a.data()[3] == b.data()[3] );
      REQUIRE( a.data()[4] == b.data()[4] );
      REQUIRE( a.data()[5] == b.data()[5] );
    }

    SECTION("non-empty record and empty record")
    {
      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      GenericRecord a;
      // read AACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );
      REQUIRE( a.loadFromStream(stream, true, dummy_table) );

      GenericRecord b;
      REQUIRE_FALSE( a.equals(b) );
      REQUIRE_FALSE( b.equals(a) );

      REQUIRE( a.size() != b.size() );
      REQUIRE( a.data() != nullptr );
      REQUIRE( b.data() == nullptr );
    }
  }

  SECTION("buffer re-allocation happens when size changes")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "AACT\x0E\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x08\0FirstId\0AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0"sv;
    std::istringstream stream;
    stream.str(std::string(data));
    GenericRecord a;
    // read AACT, because header is handled before loadFromStream.
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE( stream.good() );

    // load first AACT
    REQUIRE( a.loadFromStream(stream, true, dummy_table) );
    const auto firstPointer = a.data();

    // load second AACT
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE( a.loadFromStream(stream, true, dummy_table) );
    const auto secondPointer = a.data();

    REQUIRE( firstPointer != secondPointer );
  }
}
