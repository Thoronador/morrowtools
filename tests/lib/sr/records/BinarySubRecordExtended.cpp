/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2021, 2024  Dirk Stolle

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
#include "../../../../lib/sr/records/BinarySubRecordExtended.hpp"
#include "../../../../lib/sr/SR_Constants.hpp"

TEST_CASE("BinarySubRecordExtended")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    BinarySubRecordExtended record;

    REQUIRE_FALSE( record.isPresent() );
    REQUIRE( record.size() == 0 );
    REQUIRE( record.data() == nullptr );
  }

  SECTION("presence")
  {
    BinarySubRecordExtended record;

    REQUIRE_FALSE( record.isPresent() );
    record.setPresence(true);
    REQUIRE( record.isPresent() );
    record.setPresence(false);
    REQUIRE_FALSE( record.isPresent() );
  }

  SECTION("loadFromStream and saveToStream")
  {
    using namespace std::string_view_literals;

    SECTION("default: load record with header")
    {
      const std::string_view data = "EDID\x13\0ActionShieldChange\0"sv;
      std::istringstream streamIn;
      streamIn.str(std::string(data));

      // Reading should succeed.
      BinarySubRecordExtended record;
      REQUIRE( record.loadFromStream(streamIn, cEDID, true) );
      // Check data.
      REQUIRE( record.isPresent() );
      REQUIRE( record.size() == 19 );
      REQUIRE( record.data() != nullptr );
      const auto view = std::string_view(reinterpret_cast<const char*>(record.data()), record.size());
      REQUIRE( view == data.substr(6) );

      // Saving should succeed.
      std::ostringstream streamOut;
      // Writing should succeed.
      REQUIRE( record.saveToStream(streamOut, cEDID) );
      // Check written data.
      REQUIRE( streamOut.str() == data );

      // Saving with different header should succeed, too.
      std::ostringstream streamOutHeader;
      // Writing should succeed.
      REQUIRE( record.saveToStream(streamOutHeader, cMODL) );
      // Check written data.
      REQUIRE( streamOutHeader.str() == "MODL\x13\0ActionShieldChange\0"sv );
    }

    SECTION("default: load record without header")
    {
      const std::string_view data = "\x13\0ActionShieldChange\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Reading should succeed.
      BinarySubRecordExtended record;
      REQUIRE( record.loadFromStream(stream, cEDID, false) );
      // Check data.
      REQUIRE( record.isPresent() );
      REQUIRE( record.size() == 19 );
      REQUIRE( record.data() != nullptr );
      const auto view = std::string_view(reinterpret_cast<const char*>(record.data()), record.size());
      REQUIRE( view == data.substr(2) );

      // Saving should succeed.
      std::ostringstream streamOut;
      // Writing should succeed.
      REQUIRE( record.saveToStream(streamOut, cEDID) );
      // Check written data.
      REQUIRE( streamOut.str() == "EDID\x13\0ActionShieldChange\0"sv );
    }

    SECTION("special: load record of size zero with header")
    {
      const std::string_view data = "DATA\0\0"sv;
      std::istringstream streamIn;
      streamIn.str(std::string(data));

      // Reading should succeed.
      BinarySubRecordExtended record;
      REQUIRE( record.loadFromStream(streamIn, cDATA, true) );
      // Check data.
      REQUIRE( record.isPresent() );
      REQUIRE( record.size() == 0 );
      REQUIRE( record.data() == nullptr );

      // Saving should succeed.
      std::ostringstream streamOut;
      // Writing should succeed.
      REQUIRE( record.saveToStream(streamOut, cDATA) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: wrong header")
    {
      const std::string_view data = "FAIL\x13\0ActionShieldChange\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Reading should fail.
      BinarySubRecordExtended record;
      REQUIRE_FALSE( record.loadFromStream(stream, cEDID, true) );
    }

    SECTION("corrupt data: length of subrecord is beyond stream")
    {
      const std::string_view data = "EDID\x15\0ActionShieldChange\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Reading should fail.
      BinarySubRecordExtended record;
      REQUIRE_FALSE( record.loadFromStream(stream, cEDID, true) );
    }

    SECTION("saving a record that is not present does not write anything")
    {
      BinarySubRecordExtended record;
      REQUIRE_FALSE( record.isPresent() );

      // Saving should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut, cEDID) );
      // Check written data.
      REQUIRE( streamOut.str().empty() );
    }

    SECTION("saving a record that is not present does not write anything even if it had previous data")
    {
      const std::string_view data = "\x13\0ActionShieldChange\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Reading should succeed.
      BinarySubRecordExtended record;
      REQUIRE( record.loadFromStream(stream, cEDID, false) );
      // Check data.
      REQUIRE( record.isPresent() );
      REQUIRE( record.data() != nullptr );
      const auto view = std::string_view(reinterpret_cast<const char*>(record.data()), record.size());
      REQUIRE( view == data.substr(2) );

      // Reset presence.
      record.setPresence(false);

      // Saving should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut, cEDID) );
      // Check written data.
      REQUIRE( streamOut.str().empty() );
    }
  }

  SECTION("equality, copy constructor, etc.")
  {
    using namespace std::string_view_literals;

    SECTION("empty records")
    {
      BinarySubRecordExtended a;
      BinarySubRecordExtended b;

      REQUIRE( a == b );
      REQUIRE( b == a );
      REQUIRE_FALSE( a != b );
      REQUIRE_FALSE( b != a );
    }

    SECTION("non-empty records with same content")
    {
      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      const std::string_view data = "SNAM\x04\0\x08\x5D\x0C\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      BinarySubRecordExtended a;
      REQUIRE( a.loadFromStream(stream, cSNAM, true) );

      // Copy-construct b from a.
      BinarySubRecordExtended b = a;
      REQUIRE( a == b );
      REQUIRE( b == a );
      REQUIRE_FALSE( a != b );
      REQUIRE_FALSE( b != a );

      // Size should be equal, but pointers should not be equal.
      REQUIRE( a.size() == b.size() );
      REQUIRE( a.data() != nullptr );
      REQUIRE( b.data() != nullptr );
      REQUIRE( a.data() != b.data() );
      // However, the pointed to content should be equal.
      REQUIRE( a.data()[0] == b.data()[0] );
      REQUIRE( a.data()[1] == b.data()[1] );
      REQUIRE( a.data()[2] == b.data()[2] );
      REQUIRE( a.data()[3] == b.data()[3] );
    }

    SECTION("non-empty records with different content")
    {
      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      const std::string_view data = "SNAM\x04\0\x08\x5D\x0C\0DNAM\x04\0\x08\x5D\x0C\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      BinarySubRecordExtended a;
      REQUIRE( a.loadFromStream(stream, cSNAM, true) );

      BinarySubRecordExtended b;
      REQUIRE( b.loadFromStream(stream, cDNAM, true) );
      REQUIRE_FALSE( a == b );
      REQUIRE_FALSE( b == a );
      REQUIRE( a != b );
      REQUIRE( b != a );

      // Size should be equal, but pointers should not be equal.
      REQUIRE( a.size() == b.size() );
      REQUIRE( a.data() != nullptr );
      REQUIRE( b.data() != nullptr );
      REQUIRE( a.data() != b.data() );
      // However, the pointed to content should partially be equal.
      REQUIRE( a.data()[0] == b.data()[0] );
      REQUIRE( a.data()[1] == b.data()[1] );
      REQUIRE( a.data()[2] == b.data()[2] );
      REQUIRE( a.data()[3] != b.data()[3] );
    }

    SECTION("non-empty record and empty record")
    {
      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      const std::string_view data = "SNAM\x04\0\x08\x5D\x0C\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      BinarySubRecordExtended a;
      REQUIRE( a.loadFromStream(stream, cSNAM, true) );

      BinarySubRecordExtended b;
      REQUIRE_FALSE( a == b );
      REQUIRE_FALSE( b == a );
      REQUIRE( a != b );
      REQUIRE( b != a );

      REQUIRE( a.size() != b.size() );
      REQUIRE( a.data() != nullptr );
      REQUIRE( b.data() == nullptr );
      REQUIRE( a.isPresent() );
      REQUIRE_FALSE( b.isPresent() );
    }
  }

  SECTION("buffer re-allocation happens when size changes")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "SNAM\x04\0\x08\x5D\x0C\0EDID\x2E\0IntRoomWoodLargeTempleOfKynarethAcousticSpace\0DNAM\x04\0\x08\x5D\x0C\x01"sv;
    std::istringstream stream;
    stream.str(std::string(data));
    BinarySubRecordExtended a;

    // load SNAM
    REQUIRE( a.loadFromStream(stream, cSNAM, true) );
    const auto firstPointer = a.data();

    // load EDID
    REQUIRE( a.loadFromStream(stream, cEDID, true) );
    const auto secondPointer = a.data();

    REQUIRE( firstPointer != secondPointer );

    // load DNAM
    REQUIRE( a.loadFromStream(stream, cDNAM, true) );
    const auto thirdPointer = a.data();

    REQUIRE( secondPointer != thirdPointer );
  }
}
